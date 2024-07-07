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
} BackgroundStarField_Data;

typedef struct
{
    Vector2 position;
    Color colour;
} BackgroundStar_Data;

#define BACKGROUNDSTARFIELD_DATA_LOCALREF ((BackgroundStarField_Data *)(THIS->data_struct))
BackgroundStar_Data **BACKGROUNDSTARDATA_POOL;

GameObj_Base *player;
BackgroundStar_Data BGstars[100];
int offsetX = 0;
float minTailYVelocityClamp = 0.5f;//used when setting tail y pos

int _BackgroundStarField_Init(void *self, float DeltaTime)
{
    // generate the seed
    SetRandomSeed(GAME_TIME);
    // BACKGROUNDSTARFIELD_DATA_LOCALREF->randomSeed =
    BACKGROUNDSTARFIELD_DATA_LOCALREF->maxNumberOfStars = BACKGROUNDSTARS_MIN_NUMBER_STARS;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->minNumberOfStars = BACKGROUNDSTARS_MAX_NUMBER_STARS;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars = GetRandomValue(BACKGROUNDSTARFIELD_DATA_LOCALREF->minNumberOfStars, BACKGROUNDSTARFIELD_DATA_LOCALREF->maxNumberOfStars);
    // get the random sequence for star position
    // index between 0->numberOfStars-1 is the x positions, index between numberOfStars -> numberOfStars*2-1 is the y positions
    BACKGROUNDSTARFIELD_DATA_LOCALREF->listOfStarsPosistion = LoadRandomSequence(BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars * 2, 100, BACKGROUNDSTARFIELD_DATA_LOCALREF->cameraObjectScale * (cameraBounds.x + 1) * 100);
    THIS->position = cameraPosition;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->starSize = 0.1f;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->tailLengthScaleX = 0.0f;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->tailLengthScaleY = 0.0f;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->tailMinLength = 1.0;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->tailUpDownOffestScale = 1;

    GetObjectWithFlagsExact(FLAG_PLAYER_OBJECT, 0, &player); // getting the player.

    // Create the array of stars to hold their posistions.
    BACKGROUNDSTARDATA_POOL = (BackgroundStar_Data **)malloc(sizeof(BackgroundStar_Data *) * BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars);
    for (int i = 0; i < BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars; ++i)
    {
        BACKGROUNDSTARDATA_POOL[i] = malloc(sizeof(BackgroundStar_Data));
        BACKGROUNDSTARDATA_POOL[i]->colour = BACKGROUNDSTARS_COLOUR; // yellow
    }
    return 0;
}

int _BackgroundStarField_Update(void *self, float DeltaTime)
{

    printf("delta time: %f\n", DeltaTime);

    printf("%s\n", "background stars update");
    // get the player's velocity
    THIS->velocity = (Vector2){-1.0f * (PLAYER_OBJECT_REF->velocity.x), -0.5f * (PLAYER_OBJECT_REF->velocity.y)};

    printf("%s\n", "velocity access done");



    printf("%d number of stars\n", BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars);
    // move the all the stars across the screen
    for (int i = 0; i < BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars; i++)
    {
        if(!BACKGROUNDSTARDATA_POOL){
            // ...
            printf("%s\n","why's it spicy");
            continue;
        }
        if(BACKGROUNDSTARDATA_POOL[i]){
            // ...
            Vector2 currPosition = BACKGROUNDSTARDATA_POOL[i]->position;
            printf("BACKGROUNDSTARDATA_POOL[%d]->position { %f, %f } \n", i, currPosition.x, currPosition.y);
            float currCamScale = BACKGROUNDSTARFIELD_DATA_LOCALREF->cameraObjectScale;
            printf("BACKGROUNDSTARDATA_POOL[%d]->cameraObjectScale { %f } \n", i, currCamScale);
            Vector2 scaledVelocity = Vector2Scale(THIS->velocity, DeltaTime * currCamScale);
            printf("BACKGROUNDSTARDATA_POOL[%d] scaled velocity { %f, %f } \n", i, scaledVelocity.x, scaledVelocity.y);
            Vector2 addedVelocity = Vector2Add(currPosition, scaledVelocity);
            BACKGROUNDSTARDATA_POOL[i]->position.x = addedVelocity.x;
            BACKGROUNDSTARDATA_POOL[i]->position.y = addedVelocity.y;
            printf("BACKGROUNDSTARDATA_POOL[%d] NEW POSITION { %f, %f } \n", i, BACKGROUNDSTARDATA_POOL[i]->position.x, BACKGROUNDSTARDATA_POOL[i]->position.y);
        }
        else {
            printf("BACKGROUNDSTARDATA_POOL[%d] DOESNT EXIST\n", i);
        }
    }
    return 0;
}

void setCurrentTailLengthScale(void *self)
{
    int tailLength = player->velocity.x;
    tailLength = tailLength == 0 ? 1 : tailLength;
    tailLength = tailLength < 0 ? -1 * tailLength : tailLength;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->tailLengthScaleX = tailLength;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->tailLengthScaleY = (player->velocity.y==0)?minTailYVelocityClamp:player->velocity.y;
}

void resetStarPostionIfOutOfBounds(void *self, BackgroundStar_Data* star){
        // camera left screen bounds reposition the start to the right
        if (star->position.x < -(cameraBounds.x*2) * BACKGROUNDSTARFIELD_DATA_LOCALREF->cameraObjectScale)
        {
            star->position = (Vector2){cameraBounds.x*2*BACKGROUNDSTARFIELD_DATA_LOCALREF->cameraObjectScale, 0};
        }
}

void _BackgroundStarField_Populate(void *self)
{
    // BackgroundStarField_Data *data = THIS->data_struct;
    scaleFactor = (Vector2){BACKGROUNDSTARFIELD_DATA_LOCALREF->cameraObjectScale, BACKGROUNDSTARFIELD_DATA_LOCALREF->cameraObjectScale};
    // get the tail lenth with player velocity.
    setCurrentTailLengthScale(self);

    //Set and Draw all the stars with tail
    for (int j = 0; j < BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars; ++j) // populate the screen with stars at random posistion
    {
        // set star's random seeded posistions. 0.01 * postion is due to the list of stars position is a 3 digit number. I want it to be 0.00f, the seed only generate in integer.
        float xPosition = 0.01 * (float)BACKGROUNDSTARFIELD_DATA_LOCALREF->listOfStarsPosistion[j*2]; // add the tail
        float yPosition = 0.01 * (float)BACKGROUNDSTARFIELD_DATA_LOCALREF->listOfStarsPosistion[j*2+1];

        // add the player velocity to the stars. make the stars move
        Vector2 pos = Vector2Subtract(BACKGROUNDSTARDATA_POOL[j]->position, (Vector2){xPosition, yPosition});

        resetStarPostionIfOutOfBounds(self, BACKGROUNDSTARDATA_POOL[j]);

        // draw STAR
        //Star head positions
        Vector2 v3 = (Vector2){pos.x, pos.y};
        Vector2 v2 = (Vector2){pos.x, pos.y - (BACKGROUNDSTARFIELD_DATA_LOCALREF->starSize)};
        //Star tail position  
        Vector2 v1 = (Vector2){pos.x + BACKGROUNDSTARFIELD_DATA_LOCALREF->tailMinLength * BACKGROUNDSTARFIELD_DATA_LOCALREF->tailLengthScaleX, pos.y - BACKGROUNDSTARFIELD_DATA_LOCALREF->tailUpDownOffestScale * BACKGROUNDSTARFIELD_DATA_LOCALREF->tailLengthScaleY}; // tail point;
        //draw the STAR
        RenderTriangleAbsolute(v1, v2, v3, BACKGROUNDSTARDATA_POOL[j]->colour);
    }
}

int _BackgroundStarField_Draw(void *self, float DeltaTime)
{
    _BackgroundStarField_Populate(self);

    return 0;
}

int _BackgroundStarField_Destroy(void *self, float DeltaTime)
{
    // free our data struct here. free anything contained.
    // POSSIBLE_SEGFAULT: check for seg ables
    printf(">> unloading positions\n");
    UnloadRandomSequence(BACKGROUNDSTARFIELD_DATA_LOCALREF->listOfStarsPosistion);
    if (BACKGROUNDSTARDATA_POOL) {
        printf(">> Destroying BACKGROUNDSTARDATA_POOL\n");
        for (int i = 0; i < BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars; ++i)
        {
            BackgroundStar_Data* obj = BACKGROUNDSTARDATA_POOL[i];
            if (obj)
               free(obj);
            BACKGROUNDSTARDATA_POOL[i] = 0;
        }
    
        free(BACKGROUNDSTARDATA_POOL);
        BACKGROUNDSTARDATA_POOL = 0;
    }
    free(BACKGROUNDSTARFIELD_DATA_LOCALREF);

    return 0;
}

GameObj_Base *CreateBackgroundStars(enum LAYER_ID layer, float objectScale)
{
   
    GameObj_Base *obj_ptr = malloc(sizeof(GameObj_Base));

    // ============================================================
    // ==== setup the data scruct data
    obj_ptr->data_struct = malloc(sizeof(BackgroundStarField_Data));

    // scaleFactor
    // get the camera zoom scale factor
    BackgroundStarField_Data *tmp = obj_ptr->data_struct;
    tmp->cameraObjectScale = objectScale;

    // ============================================================

    obj_ptr->Init_Func = &_BackgroundStarField_Init;
    obj_ptr->Update_Func = &_BackgroundStarField_Update;
    obj_ptr->Draw_Func = &_BackgroundStarField_Draw;
    obj_ptr->Destroy_Func = &_BackgroundStarField_Destroy;
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
