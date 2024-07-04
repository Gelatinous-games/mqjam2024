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
#ifndef _misc
#define _misc
#include "../misc.c"
#endif
int accel_delta;
typedef struct
{
    // unsigned int randomSeed;
    int *listRandomSequence;
    int minStars;
    int maxStars;
    int numStars;
    int tailLength;
    int xVelocity;
} BackgroundStars_Data;

typedef struct
{
    Vector2 position;
} BGStar_Data;

#define BACKGROUNDSTARS_DATA ((BackgroundStars_Data *)(THIS->data_struct))

GameObj_Base *player;
Vector2 camPos;

int _BackgroundStars_Init(void *self, float DeltaTime)
{
    SetRandomSeed(GetTime);
    // BACKGROUNDSTARS_DATA->randomSeed =
    BACKGROUNDSTARS_DATA->maxStars = 100;
    BACKGROUNDSTARS_DATA->minStars = 10;
    BACKGROUNDSTARS_DATA->numStars = GetRandomValue(BACKGROUNDSTARS_DATA->minStars, BACKGROUNDSTARS_DATA->maxStars);
    BACKGROUNDSTARS_DATA->listRandomSequence = LoadRandomSequence(BACKGROUNDSTARS_DATA->numStars * 2, -100, 100);
    THIS->position = cameraPosition;
    BACKGROUNDSTARS_DATA->tailLength = 30;
    GetObjectWithFlagsExact(FLAG_PLAYER_OBJECT, 0, &player); // getting the player.

    camPos = cameraPosition;
    // Horizontal velocity: between 1 and -1
    THIS->velocity = (Vector2){1, 0};
    THIS->velocity = Vector2Normalize(THIS->velocity);

    return 0;
}

int _BackgroundStars_Update(void *self, float DeltaTime)
{
    accel_delta = GetKeyDelta(KEY_W, KEY_S);
    THIS->position = Vector2Add(THIS->position, Vector2Scale(THIS->velocity, DeltaTime));

    return 0;
}

int _BackgroundStars_Draw(void *self, float DeltaTime)
{
    // BackgroundStars_Data *data = THIS->data_struct;
    // draw the random stars

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < BACKGROUNDSTARS_DATA->numStars; j++)
        {
            float xVal = (1 * (0.01 * BACKGROUNDSTARS_DATA->listRandomSequence[j]) + i * 0.05f);
            float yVal = 1 * (0.01 * BACKGROUNDSTARS_DATA->listRandomSequence[BACKGROUNDSTARS_DATA->numStars + j]);
            // printf("%d", yVal);
            Vector2 pos = Vector2Subtract(cameraPosition, (Vector2){xVal, yVal});
            pos = Vector2Add((Vector2){THIS->position.x, 0}, pos);
            RenderCircleAbsolute(pos, 0.1f - (0.01f * i), WHITE);
        }
    }
    return 0;
}

int _BackgroundStars_Destroy(void *self, float DeltaTime)
{
    // free our data struct here. free anything contained.
    // free(BACKGROUNDSTARS_DATA->sprite);
    free(BACKGROUNDSTARS_DATA);

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

    obj_ptr->currentLayer = LAYER_BACKGROUND_STARSCAPE_0;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = (Vector2){2, 2};

    return obj_ptr;
}
