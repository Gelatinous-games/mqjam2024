#ifndef _stdio
    #define _stdio
    #include <stdio.h>
#endif

#ifndef _malloc
    #define _malloc
    #include <malloc.h>
#endif

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
    #include "src\base.c"
#endif

#ifndef _registry
    #define _registry
    #include "src/obj_register.c"
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

/// @brief Provided a pointer to an index integer and a flag string, identifies the next object in the pool 
/// that exactly matches the flag string. The integer value will be -1 if it has reached the end of the usable pool.
/// @param flags 
/// @param i 
/// @return 
GameObj_Base* GetObjectWithFlagsExact(long unsigned int flags, int* i) {
    if (*i < 0) return 0;
    for (; *i < _gameObjPoolSize + _gameObjPoolDelta; *i++) {
        GameObj_Base* obj = _gameObjPool[*i];
        if (obj->flags == flags) {
            return obj;
        }
    }
    *i = -1;
    return 0;
}

/// @brief Provided a flag string and a int pointer, identifies the next object in the pool that has any of 
/// the given flags. The index will be -1 if it has reached the end of the usable pool.
/// @param flags 
/// @param i 
/// @return 
GameObj_Base* GetObjectWithFlagsAny(long unsigned int flags, int* i) {
    if (*i < 0) return 0;
    for (; *i < _gameObjPoolSize + _gameObjPoolDelta; *i++) {
        GameObj_Base* obj = _gameObjPool[*i];
        if (obj->flags & flags) {
            return obj;
        }
    }
    *i = -1;
    return 0;
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