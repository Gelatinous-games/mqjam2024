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
    // uhhh
    long randomSeed;
    // the list of bg objects
    int *backgroundObjectIDs;
    Vector2 *backgroundObjectPositions;
    float *backgroundObjectRotations;
    Vector2 *backgroundObjectScales;
} Background_DataStruct;

#define BACKGROUND_DATA ((Background_DataStruct *)(THIS->data_struct))


#define BACKGROUND_OBJECT_COUNT 100

#define MAX_BACKGROUND_SPAWN_DISTANCE_X 400.0f
#define MAX_BACKGROUND_SPAWN_DISTANCE_Y 10.0f


#define MIN_BACKGROUND_SPAWN_SCALE_X 0.5f
#define MAX_BACKGROUND_SPAWN_SCALE_X 3.0f
#define MIN_BACKGROUND_SPAWN_SCALE_Y 0.5f
#define MAX_BACKGROUND_SPAWN_SCALE_Y 3.0f



#define BACKGROUND_SPRITE_COUNT 3
Sprite **BACKGROUND_SPRITE_LIST;

int _BackgroundStars_Init(void *self, float DeltaTime)
{
    // printf("%s\n","initialising sprite list");
    
    // malloc
    BACKGROUND_SPRITE_LIST = (Sprite **)malloc(BACKGROUND_SPRITE_COUNT * sizeof(Sprite *));

    // load
    BACKGROUND_SPRITE_LIST[0] = CreateSprite("resources/background/bg0.png");
    BACKGROUND_SPRITE_LIST[1] = CreateSprite("resources/background/bg1.png");
    BACKGROUND_SPRITE_LIST[2] = CreateSprite("resources/background/bg2.png");
    // BACKGROUND_SPRITE_LIST[0] = CreateSprite("resources/kitr_temp.png");
    

    // printf("%s\n","making bg objects list");
    // object reference array
    BACKGROUND_DATA->backgroundObjectIDs = (int *)malloc(BACKGROUND_OBJECT_COUNT*sizeof(int));

    // printf("%s\n","making bg objects positions");
    // positions array
    BACKGROUND_DATA->backgroundObjectPositions = (Vector2 *)malloc(BACKGROUND_OBJECT_COUNT*sizeof(Vector2));
    // roations
    BACKGROUND_DATA->backgroundObjectRotations = (float *)malloc(BACKGROUND_OBJECT_COUNT*sizeof(float));
    // scale array
    BACKGROUND_DATA->backgroundObjectScales = (Vector2 *)malloc(BACKGROUND_OBJECT_COUNT*sizeof(Vector2));

    for (int i = 0; i < BACKGROUND_OBJECT_COUNT; i++){
        // roll
        // printf("rolling %d\n",i);
        BACKGROUND_DATA->backgroundObjectIDs[i] = abs((INT_RAND)%BACKGROUND_SPRITE_COUNT);
        BACKGROUND_DATA->backgroundObjectPositions[i] = (Vector2){
            (FLOAT_RAND*MAX_BACKGROUND_SPAWN_DISTANCE_X),
            (FLOAT_RAND*MAX_BACKGROUND_SPAWN_DISTANCE_Y-MAX_BACKGROUND_SPAWN_DISTANCE_Y/2.0f)
        };
        // rotater
        BACKGROUND_DATA->backgroundObjectRotations[i] = FLOAT_RAND*360.0f;

        // generate two random percentage values 
        float xScaleRand = FLOAT_RAND;
        float yScaleRand = FLOAT_RAND;
        // then lerp between the minimum and maximum
        BACKGROUND_DATA->backgroundObjectScales[i] = (Vector2){
            ((1.0f-xScaleRand)*MIN_BACKGROUND_SPAWN_SCALE_X+(xScaleRand)*MAX_BACKGROUND_SPAWN_SCALE_X),
            ((1.0f-yScaleRand)*MIN_BACKGROUND_SPAWN_SCALE_Y+(yScaleRand)*MAX_BACKGROUND_SPAWN_SCALE_Y)
        };
    }

    return 0;
}

int _BackgroundStars_Update(void *self, float DeltaTime)
{
    // parallax
    THIS->position.x = -0.1f*(cameraPosition.x);
    THIS->position.y = -0.1f*(cameraPosition.y);

    
    return 0;
}

int _BackgroundStars_Draw(void *self, float DeltaTime)
{

    for (int i = 0; i < BACKGROUND_OBJECT_COUNT; i++)
    {
        // printf("drawing %d\n",i);
        int currSpriteID = BACKGROUND_DATA->backgroundObjectIDs[i];
        Vector2 currSpritePosition = BACKGROUND_DATA->backgroundObjectPositions[i];
        // printf("attempting with [%d] - id of %d, x: %f, y: %f\n",i,currSpriteID,currSpritePosition.x,currSpritePosition.y);
        RenderSpriteRelative(
            BACKGROUND_SPRITE_LIST[currSpriteID],
            (Vector2){
                THIS->position.x + currSpritePosition.x,
                THIS->position.x + currSpritePosition.y
            },
            BACKGROUND_DATA->backgroundObjectScales[i],
            BACKGROUND_DATA->backgroundObjectRotations[i],
            WHITE
        );
    }

    

    return 0;
}

int _BackgroundStars_Destroy(void *self, float DeltaTime)
{
    // free our data struct here. free anything contained.

    for(int i = 0; i < BACKGROUND_SPRITE_COUNT; i++){
        DestroySprite(BACKGROUND_SPRITE_LIST[i]);
    }
    free(BACKGROUND_SPRITE_LIST);

    // scale/position data
    free(BACKGROUND_DATA->backgroundObjectScales);
    free(BACKGROUND_DATA->backgroundObjectRotations);
    free(BACKGROUND_DATA->backgroundObjectPositions);

    // object reference array
    free(BACKGROUND_DATA->backgroundObjectIDs);

    // the data sstruct
    free(THIS->data_struct);

    return 0;
}

GameObj_Base *CreateBackgroundStars()
{
    GameObj_Base *obj_ptr = malloc(sizeof(GameObj_Base));

    // ============================================================
    // ==== setup the data scruct data
    obj_ptr->data_struct = malloc(sizeof(Background_DataStruct));

    // ============================================================

    obj_ptr->Init_Func = &_BackgroundStars_Init;
    obj_ptr->Update_Func = &_BackgroundStars_Update;
    obj_ptr->Draw_Func = &_BackgroundStars_Draw;
    obj_ptr->Destroy_Func = &_BackgroundStars_Destroy;
    // ============================================================

    obj_ptr->position = (Vector2){ 0.0f, 0.0f };

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_BACKGROUND;

    obj_ptr->currentLayer = LAYER_BACKGROUND_STARSCAPE_0;

    return obj_ptr;
}
