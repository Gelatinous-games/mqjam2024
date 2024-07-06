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
    int *listOfStarsPosistion; // Pointer to a list of random generated postions.
    int minNumberOfStars;
    int maxNumberOfStars;
    int numberOfStars;
    float starSize;
    float tailLengthScaleX; // the player velocity.x to scale the tail length
    float tailLengthScaleY; // the player velocity.y to scale the up/down flick (not angle)
    float tailMinLength; // min length of tail
    float tailUpDownOffestScale;// effects how much the player velocity.y affects the tail's Y position
    float cameraObjectScale;          // what scale to draw (camera zoom)
    // int xVelocity;
} BackgroundStars_Data;

typedef struct
{
    Vector2 position;
    Color colour;
} BackgroundStar_Data;

#define BACKGROUNDSTARS_DATA ((BackgroundStars_Data *)(THIS->data_struct))
BackgroundStar_Data **BACKGROUNDSTARS_POOL;

GameObj_Base *player;
BackgroundStar_Data BGstars[100];
int offsetX = 0;
float minTailYVelocityClamp = 0.5f;//used when setting tail y pos

int _BackgroundStars_Init(void *self, float DeltaTime)
{
    // generate the seed
    SetRandomSeed(GAME_TIME);
    // BACKGROUNDSTARS_DATA->randomSeed =
    BACKGROUNDSTARS_DATA->maxNumberOfStars = BACKGROUNDSTARS_MIN_NUMBER_STARS;
    BACKGROUNDSTARS_DATA->minNumberOfStars = BACKGROUNDSTARS_MAX_NUMBER_STARS;
    BACKGROUNDSTARS_DATA->numberOfStars = GetRandomValue(BACKGROUNDSTARS_DATA->minNumberOfStars, BACKGROUNDSTARS_DATA->maxNumberOfStars);
    // get the random sequence for star position
    // index between 0->numberOfStars-1 is the x positions, index between numberOfStars -> numberOfStars*2-1 is the y positions
    BACKGROUNDSTARS_DATA->listOfStarsPosistion = LoadRandomSequence(BACKGROUNDSTARS_DATA->numberOfStars * 2, 100, BACKGROUNDSTARS_DATA->cameraObjectScale * (cameraBounds.x + 1) * 100);
    THIS->position = cameraPosition;
    BACKGROUNDSTARS_DATA->starSize = 0.1f;
    BACKGROUNDSTARS_DATA->tailLengthScaleX = 0.0f;
    BACKGROUNDSTARS_DATA->tailLengthScaleY = 0.0f;
    BACKGROUNDSTARS_DATA->tailMinLength = 1.0;
    BACKGROUNDSTARS_DATA->tailUpDownOffestScale = 1;

    GetObjectWithFlagsExact(FLAG_PLAYER_OBJECT, 0, &player); // getting the player.

    // Create the array of stars to hold their posistions.
    BACKGROUNDSTARS_POOL = (BackgroundStar_Data **)malloc(sizeof(BackgroundStar_Data *) * BACKGROUNDSTARS_DATA->numberOfStars);
    for (int i = 0; i < BACKGROUNDSTARS_DATA->numberOfStars; ++i)
    {
        BACKGROUNDSTARS_POOL[i] = malloc(sizeof(BackgroundStar_Data));
        BACKGROUNDSTARS_POOL[i]->colour = BACKGROUNDSTARS_COLOUR; // yellow
    }
    return 0;
}

int _BackgroundStars_Update(void *self, float DeltaTime)
{
    // get the player's velocity
    THIS->velocity = (Vector2){-1.0f * (PLAYER_OBJECT_REF->velocity.x), -0.5f * (PLAYER_OBJECT_REF->velocity.y)};

    // move the all the stars across the screen
    for (int i = 0; i < BACKGROUNDSTARS_DATA->numberOfStars; i++)
    {
        BACKGROUNDSTARS_POOL[i]->position = Vector2Add(BACKGROUNDSTARS_POOL[i]->position, Vector2Scale(THIS->velocity, DeltaTime * BACKGROUNDSTARS_DATA->cameraObjectScale));
    }
    return 0;
}

void setCurrentTailLengthScale(void *self)
{
    int tailLength = player->velocity.x;
    tailLength = tailLength == 0 ? 1 : tailLength;
    tailLength = tailLength < 0 ? -1 * tailLength : tailLength;
    BACKGROUNDSTARS_DATA->tailLengthScaleX = tailLength;
    BACKGROUNDSTARS_DATA->tailLengthScaleY = (player->velocity.y==0)?minTailYVelocityClamp:player->velocity.y;
}

void resetStarPostionIfOutOfBounds(void *self, BackgroundStar_Data* star){
        // camera left screen bounds reposition the start to the right
        if (star->position.x < -(cameraBounds.x*2) * BACKGROUNDSTARS_DATA->cameraObjectScale)
        {
            star->position = (Vector2){cameraBounds.x*2*BACKGROUNDSTARS_DATA->cameraObjectScale, 0};
        }
}

void _BackgroundStars_Populate(void *self)
{
    // BackgroundStars_Data *data = THIS->data_struct;
    scaleFactor = (Vector2){BACKGROUNDSTARS_DATA->cameraObjectScale, BACKGROUNDSTARS_DATA->cameraObjectScale};
    // get the tail lenth with player velocity.
    setCurrentTailLengthScale(self);

    //Set and Draw all the stars with tail
    for (int j = 0; j < BACKGROUNDSTARS_DATA->numberOfStars; ++j) // populate the screen with stars at random posistion
    {
        // set star's random seeded posistions. 0.01 * postion is due to the list of stars position is a 3 digit number. I want it to be 0.00f, the seed only generate in integer.
        float xPosition = 0.01 * (float)BACKGROUNDSTARS_DATA->listOfStarsPosistion[j]; // add the tail
        float yPosition = 0.01 * (float)BACKGROUNDSTARS_DATA->listOfStarsPosistion[BACKGROUNDSTARS_DATA->numberOfStars + j];

        // add the player velocity to the stars. make the stars move
        Vector2 pos = Vector2Subtract(BACKGROUNDSTARS_POOL[j]->position, (Vector2){xPosition, yPosition});

        resetStarPostionIfOutOfBounds(self, BACKGROUNDSTARS_POOL[j]);

        // draw STAR
        //Star head positions
        Vector2 v3 = (Vector2){pos.x, pos.y};
        Vector2 v2 = (Vector2){pos.x, pos.y - (BACKGROUNDSTARS_DATA->starSize)};
        //Star tail position  
        Vector2 v1 = (Vector2){pos.x + BACKGROUNDSTARS_DATA->tailMinLength * BACKGROUNDSTARS_DATA->tailLengthScaleX, pos.y - BACKGROUNDSTARS_DATA->tailUpDownOffestScale * BACKGROUNDSTARS_DATA->tailLengthScaleY}; // tail point;
        //draw the STAR
        RenderTriangleAbsolute(v1, v2, v3, BACKGROUNDSTARS_POOL[j]->colour);
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
    UnloadRandomSequence(BACKGROUNDSTARS_DATA->listOfStarsPosistion);
    for (int i = 0; i < BACKGROUNDSTARS_DATA->numberOfStars; ++i)
    {
        free(BACKGROUNDSTARS_POOL[i]);
    }
    free(BACKGROUNDSTARS_POOL);
    free(BACKGROUNDSTARS_DATA);

    return 0;
}

GameObj_Base *CreateBackgroundStars(enum LAYER_ID layer, float objectScale)
{
   
    GameObj_Base *obj_ptr = malloc(sizeof(GameObj_Base));

    // ============================================================
    // ==== setup the data scruct data
    obj_ptr->data_struct = malloc(sizeof(BackgroundStars_Data));

    // scaleFactor
    // get the camera zoom scale factor
    BackgroundStars_Data *tmp = obj_ptr->data_struct;
    tmp->cameraObjectScale = objectScale;

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
