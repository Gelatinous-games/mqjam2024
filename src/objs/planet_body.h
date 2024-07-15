#pragma once



#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "../base.h"
#include "../settings.h"
#include "../misc_util.h"


#ifndef _camera
    #define _camera
    #include "../camera.c"
#endif


#ifndef _obj_pool
    #define _obj_pool
    #include "../obj_pool.c"
#endif

#ifndef _obj_particle
    #define _obj_particle
    #include "./particle.c"
#endif

#include "../sprite.h"

#ifndef _gm
    #define _gm
    #include "gameManager.c"
#endif

#include "../SpriteLibrary.h"

typedef struct {
    int spriteID;
    float distanceFromStart;

    float rotate;
} Planet_DataStruct;


int _Planet_Init(void* self, float DeltaTime);
int _Planet_Update(void* self, float DeltaTime);
int _Planet_Draw(void* self, float DeltaTime);
int _Planet_Destroy(void* self, float DeltaTime);
