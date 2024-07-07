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

#define DEBUG_SPAMMER_PRINTF_PREFIX if(true)

typedef struct
{
    Vector2 position;
    Color colour;
} BackgroundStar_Data;


typedef struct
{
    // unsigned int randomSeed;
    int *listOfStarsPosistion; // Pointer to a list of random generated postions.
    BackgroundStar_Data **BACKGROUNDSTARDATA_POOL;
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


#define BACKGROUNDSTARFIELD_DATA_LOCALREF ((BackgroundStarField_Data *)(THIS->data_struct))


// BackgroundStar_Data BGstars[100];
int offsetX = 0;
float minTailYVelocityClamp = 0.5f;//used when setting tail y pos

int _BackgroundStarField_Init(void *self, float DeltaTime)
{
    printf("%s\n","starfield init");
    // generate the seed
    SetRandomSeed((unsigned int)(GAME_TIME));
    // BACKGROUNDSTARFIELD_DATA_LOCALREF->randomSeed =
    BACKGROUNDSTARFIELD_DATA_LOCALREF->maxNumberOfStars = BACKGROUNDSTARS_MIN_NUMBER_STARS;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->minNumberOfStars = BACKGROUNDSTARS_MAX_NUMBER_STARS;
    // BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars = GetRandomValue(BACKGROUNDSTARFIELD_DATA_LOCALREF->minNumberOfStars, BACKGROUNDSTARFIELD_DATA_LOCALREF->maxNumberOfStars);
    BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars = Lerp(BACKGROUNDSTARFIELD_DATA_LOCALREF->minNumberOfStars, BACKGROUNDSTARFIELD_DATA_LOCALREF->maxNumberOfStars, FLOAT_RAND);
    // get the random sequence for star position
    // index between 0->numberOfStars-1 is the x positions, index between numberOfStars -> numberOfStars*2-1 is the y positions
    BACKGROUNDSTARFIELD_DATA_LOCALREF->listOfStarsPosistion = LoadRandomSequence(BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars * 2 +4, 100, BACKGROUNDSTARFIELD_DATA_LOCALREF->cameraObjectScale * (cameraBounds.x + 1) * 100);
    
    THIS->position = cameraPosition;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->starSize = 0.1f;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->tailLengthScaleX = 0.0f;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->tailLengthScaleY = 0.0f;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->tailMinLength = 1.0;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->tailUpDownOffestScale = 1;

    // GetObjectWithFlagsExact(FLAG_PLAYER_OBJECT, 0, &player); // getting the player.

    // Create the array of stars to hold their posistions.
    BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL = (BackgroundStar_Data **)malloc(sizeof(BackgroundStar_Data *) * BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars);
    for (int i = 0; i < BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars; ++i)
    {
        BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[i] = (BackgroundStar_Data *)malloc(sizeof(BackgroundStar_Data));
        BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[i]->position = Vector2Zero(); // zero
        BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[i]->colour = BACKGROUNDSTARS_COLOUR; // yellow
    }

    printf("%s\n","starfield init done");
    return 0;
}

int _BackgroundStarField_Update(void *self, float DeltaTime)
{

    DEBUG_SPAMMER_PRINTF_PREFIX printf("delta time: %f\n", DeltaTime);

    DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n", "background stars update");
    // get the player's velocity
    THIS->velocity = (Vector2){-1.0f * (PLAYER_OBJECT_REF->velocity.x), -0.5f * (PLAYER_OBJECT_REF->velocity.y)};

    DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n", "velocity access done");



    DEBUG_SPAMMER_PRINTF_PREFIX printf("%d number of stars\n", BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars);
    // move the all the stars across the screen
    for (int i = 0; i < BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars; i++)
    {
        if(!BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL){
            // ...
            DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n","why's it spicy");
            continue;
        }
        if(BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[i]){
            // ...
            Vector2 currPosition = BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[i]->position;
            DEBUG_SPAMMER_PRINTF_PREFIX printf("BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[%d]->position { %f, %f } \n", i, currPosition.x, currPosition.y);
            float currCamScale = BACKGROUNDSTARFIELD_DATA_LOCALREF->cameraObjectScale;
            DEBUG_SPAMMER_PRINTF_PREFIX printf("BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[%d]->cameraObjectScale { %f } \n", i, currCamScale);
            Vector2 scaledVelocity = Vector2Scale(THIS->velocity, DeltaTime * currCamScale);
            DEBUG_SPAMMER_PRINTF_PREFIX printf("BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[%d] scaled velocity { %f, %f } \n", i, scaledVelocity.x, scaledVelocity.y);
            Vector2 addedVelocity = Vector2Add(currPosition, scaledVelocity);
            BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[i]->position.x = addedVelocity.x;
            BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[i]->position.y = addedVelocity.y;
            DEBUG_SPAMMER_PRINTF_PREFIX printf("BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[%d] NEW POSITION { %f, %f } \n", i, BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[i]->position.x, BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[i]->position.y);
        }
        else {
            DEBUG_SPAMMER_PRINTF_PREFIX printf("BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[%d] DOESNT EXIST\n", i);
        }
    }
    return 0;
}

void setCurrentTailLengthScale(void *self)
{
    int tailLength = PLAYER_OBJECT_REF->velocity.x;
    tailLength = tailLength == 0 ? 1 : tailLength;
    tailLength = tailLength < 0 ? -1 * tailLength : tailLength;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->tailLengthScaleX = tailLength;
    BACKGROUNDSTARFIELD_DATA_LOCALREF->tailLengthScaleY = (PLAYER_OBJECT_REF->velocity.y==0)? minTailYVelocityClamp: PLAYER_OBJECT_REF->velocity.y;
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
        Vector2 pos = Vector2Subtract(BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[j]->position, (Vector2){xPosition, yPosition});

        resetStarPostionIfOutOfBounds(self, BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[j]);

        // draw STAR
        //Star head positions
        Vector2 v3 = (Vector2){pos.x, pos.y};
        Vector2 v2 = (Vector2){pos.x, pos.y - (BACKGROUNDSTARFIELD_DATA_LOCALREF->starSize)};
        //Star tail position  
        Vector2 v1 = (Vector2){pos.x + BACKGROUNDSTARFIELD_DATA_LOCALREF->tailMinLength * BACKGROUNDSTARFIELD_DATA_LOCALREF->tailLengthScaleX, pos.y - BACKGROUNDSTARFIELD_DATA_LOCALREF->tailUpDownOffestScale * BACKGROUNDSTARFIELD_DATA_LOCALREF->tailLengthScaleY}; // tail point;
        //draw the STAR
        RenderTriangleAbsolute(v1, v2, v3, BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[j]->colour);
    }
}

int _BackgroundStarField_Draw(void *self, float DeltaTime)
{
    printf("%s\n", "starfield draw call");
    _BackgroundStarField_Populate(self);

    printf("%s\n", "starfield draw call finished populate");
    return 0;
}

int _BackgroundStarField_Destroy(void *self, float DeltaTime)
{
    // free our data struct here. free anything contained.
    // POSSIBLE_SEGFAULT: check for seg ables
    DEBUG_SPAMMER_PRINTF_PREFIX printf(">> unloading positions\n");
    UnloadRandomSequence(BACKGROUNDSTARFIELD_DATA_LOCALREF->listOfStarsPosistion);
    if (BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL) {
        DEBUG_SPAMMER_PRINTF_PREFIX printf(">> Destroying BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL\n");
        for (int i = 0; i < BACKGROUNDSTARFIELD_DATA_LOCALREF->numberOfStars; ++i)
        {
            BackgroundStar_Data* obj = BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[i];
            if (obj)
               free(obj);
            BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL[i] = 0;
        }
    
        free(BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL);
        BACKGROUNDSTARFIELD_DATA_LOCALREF->BACKGROUNDSTARDATA_POOL = 0;
    }
    free( ((GameObj_Base *)self)->data_struct );
    ((GameObj_Base *)self)->data_struct = 0;

    
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


#undef DEBUG_SPAMMER_PRINTF_PREFIX