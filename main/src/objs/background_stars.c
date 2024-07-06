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
typedef struct
{
    // unsigned int randomSeed;
    int *listRandomSequence;
    int minStars;
    int maxStars;
    int numStars;
    float starSize;
    int tailLength;
    float ObScale; // what scale to draw
    // int xVelocity;
} BackgroundStars_Data;

typedef struct
{
    Vector2 position;
} BGStar_Data;

#define BACKGROUNDSTARS_DATA ((BackgroundStars_Data *)(THIS->data_struct))
BGStar_Data **BGSTART_POOL;

GameObj_Base *player;
BGStar_Data BGstars[100];
int offsetX = 0;
float scaleF = 1; // camera zoom

int _BackgroundStars_Init(void *self, float DeltaTime)
{

    SetRandomSeed(GAME_TIME);
    // BACKGROUNDSTARS_DATA->randomSeed =
    BACKGROUNDSTARS_DATA->maxStars = 50;
    BACKGROUNDSTARS_DATA->minStars = 30;
    BACKGROUNDSTARS_DATA->numStars = GetRandomValue(BACKGROUNDSTARS_DATA->minStars, BACKGROUNDSTARS_DATA->maxStars);
    BACKGROUNDSTARS_DATA->listRandomSequence = LoadRandomSequence(BACKGROUNDSTARS_DATA->numStars * 2, 100, (cameraBounds.x + 1) * 100);
    THIS->position = cameraPosition;
    BACKGROUNDSTARS_DATA->starSize = 0.1f;
    BACKGROUNDSTARS_DATA->tailLength = 30;

    // BACKGROUNDSTARS_DATA->ObScale = scaleF;

    GetObjectWithFlagsExact(FLAG_PLAYER_OBJECT, 0, &player); // getting the player.
   //Create the array of stars to hole their posistions. 
    BGSTART_POOL = (BGStar_Data **)malloc(sizeof(BGStar_Data *) * BACKGROUNDSTARS_DATA->numStars);
    for (int i = 0; i < BACKGROUNDSTARS_DATA->numStars; ++i)
    {
        BGSTART_POOL[i] = malloc(sizeof(BGStar_Data));
    }
    return 0;
}

int _BackgroundStars_Update(void *self, float DeltaTime)
{
    //get the player's velocity
    THIS->velocity = (Vector2){-1.0f * (PLAYER_OBJECT_REF->velocity.x), -0.5f * (PLAYER_OBJECT_REF->velocity.y)};

    //move the stars across the screen
    for (int i = 0; i < BACKGROUNDSTARS_DATA->numStars; i++)
    {
        BGSTART_POOL[i]->position = Vector2Add(BGSTART_POOL[i]->position, Vector2Scale(THIS->velocity, DeltaTime * BACKGROUNDSTARS_DATA->ObScale));
    }
    // THIS->position = Vector2Add(THIS->position, Vector2Scale(THIS->velocity, DeltaTime));

    return 0;
}

void _BackgroundStars_Populate(void *self){
// BackgroundStars_Data *data = THIS->data_struct;
    scaleFactor = (Vector2){BACKGROUNDSTARS_DATA->ObScale, BACKGROUNDSTARS_DATA->ObScale};
    // get the tail lenth with player velocity. ma
    int tailLength = player->velocity.x;
    tailLength = tailLength == 0 ? 1 : tailLength;
    tailLength = tailLength < 0 ? -1 * tailLength : tailLength;

    for (int j = 0; j < BACKGROUNDSTARS_DATA->numStars; j++) // populate the screen with stars at random posistion
    {
        //set star's random seeded posistions
        float xVal = 0.01 * (float)BACKGROUNDSTARS_DATA->listRandomSequence[j]; // add the tail
        float yVal = 0.01 * (float)BACKGROUNDSTARS_DATA->listRandomSequence[BACKGROUNDSTARS_DATA->numStars + j];

        // add the player velocity to the stars. make the stars move
        Vector2 pos = Vector2Subtract(BGSTART_POOL[j]->position, (Vector2){xVal, yVal});

        // camera left screen bounds reposition the start to the right
        if (pos.x < -16 * BACKGROUNDSTARS_DATA->ObScale)
        {
            BGSTART_POOL[j]->position = (Vector2){16, 0};
        }

        // scale the pos with the screenFactor
        // pos = Vector2Scale(pos, BACKGROUNDSTARS_DATA->ObScale);

        // draw STAR
        Vector2 v3 = (Vector2){pos.x, pos.y};
        Vector2 v2 = (Vector2){pos.x, pos.y - (BACKGROUNDSTARS_DATA->starSize)};
        Vector2 v1 = (Vector2){pos.x + 1 * tailLength, pos.y - (BACKGROUNDSTARS_DATA->starSize) * 3 * player->velocity.y}; // tail point;
        RenderTriangleAbsolute(v1, v2, v3, WHITE);
    }
}

int _BackgroundStars_Draw(void *self, float DeltaTime)
{
    _BackgroundStars_Populate(self);

    return 0;
}

int _BackgroundStars_Destroy(void *self, float DeltaTime)
{
    // free our data struct here. free anything contained.
    // free(BACKGROUNDSTARS_DATA->sprite);
    UnloadRandomSequence(BACKGROUNDSTARS_DATA->listRandomSequence);
    for (int i = 0; i < BACKGROUNDSTARS_DATA->numStars; ++i)
    {
        free(BGSTART_POOL[i]);
    }
    free(BGSTART_POOL);
    free(BACKGROUNDSTARS_DATA);

    return 0;
}

GameObj_Base *CreateBackgroundStars(enum LAYER_ID layer, float objectScale)
{
    // scaleFactor
    scaleF = objectScale;
    GameObj_Base *obj_ptr = malloc(sizeof(GameObj_Base));

    // ============================================================
    // ==== setup the data scruct data
    obj_ptr->data_struct = malloc(sizeof(BackgroundStars_Data));

    BackgroundStars_Data* tmp = obj_ptr->data_struct;
    tmp->ObScale = objectScale;

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

    obj_ptr->currentLayer = layer;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = (Vector2){2, 2};

    return obj_ptr;
}
