#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "../base.h"
#include "../settings.h"
#include "../misc_util.h"
#include "../palette_lib.h"

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

#ifndef _camera
#define _camera
#include "../camera.c"
#endif



typedef struct
{
    Vector2 position;
    Color colour;
} BGStar_Data;


typedef struct
{
    BGStar_Data **BGSTAR_POOL;
    // unsigned int randomSeed;
    int *listRandomSequence;
    int minStars;
    int maxStars;
    int numStars;
    float starSize;
    int tailLength;
    float ObScale; // what scale to draw
    // int xVelocity;
} BackgroundStarField_Data;


#define BACKGROUNDSTARFIELD_DATA ((BackgroundStarField_Data *)(THIS->data_struct))


GameObj_Base *player;
// BGStar_Data BGstars[100];
int offsetX = 0;
float scaleF = 1; // camera zoom

int _BackgroundStars_Init(void *self, float DeltaTime);
int _BackgroundStars_Update(void *self, float DeltaTime);
int _BackgroundStars_Draw(void *self, float DeltaTime);
int _BackgroundStars_Destroy(void *self, float DeltaTime);

void _BackgroundStars_Populate(void *self);