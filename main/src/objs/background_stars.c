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
#include "src/objs/particle.c"
#endif

#ifndef _misc
#define _misc
#include "../misc.c"
#endif

typedef struct
{
    long randomSeed;
    int minStars;
    int maxStars;
} BackgroundStars_Data;

#define BACKGROUND_STARS_DATA ((BackgroundStars_Data *)(THIS->data_struct))

int _BackgroundStars_Init(void *self, float DeltaTime)
{
    // declar data

    return 0;
}

int _BackgroundStars_Update(void *self, float DeltaTime)
{

    return 0;
}

int _BackgroundStars_Draw(void *self, float DeltaTime)
{
    // draw the random stars
    return 0;
}

int _BackgroundStars_Destroy(void *self, float DeltaTime)
{
    free(BACKGROUND_STARS_DATA);

    return 0;
}

GameObj_Base *CreateBackgroundStars()
{
    GameObj_Base *obj_ptr = malloc(sizeof(GameObj_Base));

     // ============================================================
    // ==== setup the data scruct data
    obj_ptr->data_struct = malloc(sizeof(BackgroundStars_Data)); 

 // ============================================================

    obj_ptr->Init_Func = &_BackgroundStars_Init;
    obj_ptr->Update_Func = &_BackgroundStars_Update;
    obj_ptr->Draw_Func = &_BackgroundStars_Draw;
    obj_ptr->Destroy_Func = &_BackgroundStars_Destroy;
 // ============================================================
   
    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_BACKGROUND;

    obj_ptr->currentLayer = LAYER_GUI;

    obj_ptr->radius = 0;
    obj_ptr->mass = 100;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = Vector2One();
    return obj_ptr;
}
