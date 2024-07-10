#pragma once



#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "../base.h"
#include "../settings.h"


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

#ifndef _sprite
    #define _sprite
    #include "../sprite.c"
#endif

#ifndef _gm
    #define _gm
    #include "gameManager.c"
#endif

#include "../settings.h"

typedef struct {
    int spriteID;
    float distanceFromStart;

    float rotate;
} Planet_DataStruct;

#define PLANET_DATA ((Planet_DataStruct *)(THIS->data_struct))

#define PLANET_SPRITE_COUNT 5

Sprite** _planetSprites;

int _Planet_Init(void* self, float DeltaTime);
int _Planet_Update(void* self, float DeltaTime);
int _Planet_Draw(void* self, float DeltaTime);
int _Planet_Destroy(void* self, float DeltaTime);
