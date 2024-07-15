#pragma once

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


#define OBJECT_POOL_CAPACITY 256

GameObj_Base** _gameObjPool;
int _gameObjPoolSize;
int _gameObjPoolDelta;
int _gameObjGlobalCounter;