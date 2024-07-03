

#include <stdio.h>
#include <stdlib.h>

#ifndef _raylib
    #define _raylib
    #include "raylib.h"
#endif

#ifndef _raymath
    #define _raymath
    #include "raymath.h"
#endif

#ifndef _game_base
    #define _game_base
    #include "base.h"
#endif

#ifndef _registry
    #define _registry
    #include "obj_register.c"
#endif

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
    for (int i = 0; i < _gameObjPoolSize; i++) {
        GameObj_Base* obj = _gameObjPool[i];
        obj->Update_Func(obj, DeltaTime);
    }
}

/// @brief Processes the draw function on all objects that are not waiting to be initialized.
/// @param DeltaTime 
void ProcessAllDraws(float DeltaTime) {
    for (int i = 0; i < _gameObjPoolSize; i++) {
        GameObj_Base* obj = _gameObjPool[i];
        obj->Draw_Func(obj, DeltaTime);
    }
}