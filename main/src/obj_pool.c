

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"


#include "base.h"
#include "obj_register.h"

#ifndef _camera
    #define _camera
    #include "../camera.c"
#endif
// #define SLASH_THING /
// #define DEBUG_SPAMMER_PRINTF_PREFIX if(true)

#define OBJECT_POOL_CAPACITY 256

GameObj_Base** _gameObjPool;
int _gameObjPoolSize;
int _gameObjPoolDelta;
int _gameObjGlobalCounter;

/// @brief Initialize the object pool properly.
void GameObjPoolInit() {
    _gameObjPool = malloc(OBJECT_POOL_CAPACITY * sizeof(GameObj_Base*));
    _gameObjPoolSize = 0;
    _gameObjPoolDelta = 0;
    _gameObjGlobalCounter = 0;
}
/// @brief Frees the object pool.
void GameObjPoolDestroy() {
    // TODO: Destroy all active game objects.
    free(_gameObjPool);
}

/// @brief Adds a given game object to the object pool.
/// @param obj 
/// @return 
int AddToPool(GameObj_Base* obj) {
    if (_gameObjPoolSize + _gameObjPoolDelta >= OBJECT_POOL_CAPACITY) return -1;
    
    _gameObjPool[_gameObjPoolSize + _gameObjPoolDelta] = obj;
    _gameObjPoolDelta += 1;

    obj->_poolID = _gameObjGlobalCounter++;

    return 0;
}

/// @brief Processes the init function on all objects added to the pool since the last call, and formally adds them to the pool.
/// @return 
int ProcessFreshAdd() {
    for (int i = _gameObjPoolSize; i < _gameObjPoolDelta + _gameObjPoolSize; i++) {
        GameObj_Base* obj = _gameObjPool[i];
        obj->Init_Func(obj, 0);
    }

    _gameObjPoolSize += _gameObjPoolDelta;
    _gameObjPoolDelta = 0;

    return 0;
}

/// @brief Returns the next object in the object pool from idx sIDX that matches the given flag string. 
/// The object is stored under retobj.
/// The next index to check from is returned.
/// @param flags 
/// @param sIDX 
/// @param retobj 
/// @return 
int GetObjectWithFlagsExact(long unsigned int flags, int sIDX, GameObj_Base** retobj) {
    for (int i = sIDX; i < _gameObjPoolSize + _gameObjPoolDelta; i++) {
        GameObj_Base* obj = _gameObjPool[i];
        if (obj->flags == flags) {
            *retobj = obj;
            return i+1;
        }
    }
    
    *retobj = 0;
    return -1;
}

/// @brief Returns the next object in the object pool from idx sIDX that has any of the given flag string. 
/// The object is stored under retobj.
/// The next index to check from is returned.
/// @param flags 
/// @param sIDX 
/// @param retobj 
/// @return 
int GetObjectWithFlagsAny(long unsigned int flags, int sIDX, GameObj_Base** retobj) {
    for (int i = sIDX; i < _gameObjPoolSize + _gameObjPoolDelta; i++) {
        GameObj_Base* obj = _gameObjPool[i];
        if (obj->flags & flags) {
            *retobj = obj;
            return i+1;
        }
    }
    *retobj = 0;
    return -1;
}

/// @brief Processes the update function on all objects that are not waiting to be initialized.
/// @param DeltaTime 
void ProcessAllUpdates(float DeltaTime) {
    // DEBUG_SPAMMER_PRINTF_PREFIX printf("ProcessAllUpdates(%f){\n",DeltaTime);
    if(_gameObjPool){
        // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s not null\n", "_gameObjPool");
    }
    else{
        // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s IS NULL!!!\n", "_gameObjPool");
    }
    // DEBUG_SPAMMER_PRINTF_PREFIX printf("%d pool size\n", _gameObjPoolSize);
    for (int i = 0; i < _gameObjPoolSize; i++) {
        // DEBUG_SPAMMER_PRINTF_PREFIX printf("_gameObjPool[%d]\n",i);
        GameObj_Base* obj = _gameObjPool[i];
        if(obj) {
            // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s",".");
            obj->Update_Func(obj, DeltaTime);
        }
        else {
            // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s","!");
        }
    }
    // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n","}");
}

/// @brief Processes the draw function on all objects that are not waiting to be initialized.
/// @param DeltaTime 
void ProcessAllDraws(float DeltaTime) {
    // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n", "ProcessAllDraws() called");
    printf("layers to draw: %d\n",LAYER_BOOKEND);
    for (int l = 0; l < LAYER_BOOKEND; l++) {
        printf("========= DRAW LAYER %d ========= <START>\n",l);
        printf("==> OBJ COUNT: %d\n", _gameObjPoolSize);
        for (int i = 0; i < _gameObjPoolSize; i++) {
            printf(">> --- obj[%d] draw --- START\n",i);
            scaleFactor = Vector2One();
            GameObj_Base* obj = _gameObjPool[i];
            printf( "obj layer : %d\n", obj->currentLayer);

            if (obj->currentLayer == l){
                
                printf("> FLAG: %lu\n", obj->flags);
                obj->Draw_Func(obj, DeltaTime);
            }
            printf(">> --- obj[%d] draw --- END\n",i);
        }
        printf("========= DRAW LAYER %d ========= <DONE>\n",l);
    }
    printf("%s\n","FINISHED PROCESS ALL DRAWS");
}

/// @brief Searches for any gameobjects seeking to be destroyed and obliterates them. Call after freshAdd so that there are no pendings.
/// @param obj 
/// @return 
void ProcessAllDestroys() {
    for (int i = 0; i < _gameObjPoolSize; i++) {
        GameObj_Base* obj = _gameObjPool[i];

        if (obj->awaitDestroy) {
            // replace this index with the end of the active pool 
            _gameObjPool[i] = _gameObjPool[_gameObjPoolSize-1];
            // replace that index with the end of the awaiting creation buffer
            _gameObjPool[_gameObjPoolSize-1] = _gameObjPool[(_gameObjPoolSize + _gameObjPoolDelta)-1];
            // and replace THAT index with null (0)
            _gameObjPool[(_gameObjPoolSize + _gameObjPoolDelta)-1] = 0;
            
            _gameObjPoolSize -= 1; // decrement the number of objects in the pool by 1 as the one at the end is nulled.
            i -=1; // and since we've just bubbled the last down, decrease i so we can redo that step.

            // if (obj->flags & FLAG_MANAGER)
            // {
            //     /* code */
            // }
            obj->Destroy_Func(obj, 0);
            free(obj);
            obj = 0;
            
        }
    }
}

// #undef DEBUG_SPAMMER_PRINTF_PREFIX