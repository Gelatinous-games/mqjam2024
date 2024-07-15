#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "../base.h"


#ifndef _camera
    #define _camera
    #include "../camera.c"
#endif

#ifndef _obj_pool
    #define _obj_pool
    #include "../obj_pool.c"
#endif

#include "../obj_register.h"
#include "../settings.h"
#include "../misc_util.h"



typedef struct {
    char* data;
    float textFlash;
    float textTmr;
    char doTextFlash;
    char visible;
    char doWidthSubtr;
} _TextObj_Data;