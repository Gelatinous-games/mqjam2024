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

#ifndef _background_spritelibrary
#define _background_spritelibrary
#include "background_spritelibrary.c"
#endif


#define DEBUG_SPAMMER_PRINTF_PREFIX if(true) 


typedef struct
{
    // for whether it's a background image we can tint or not
    int spriteType;
    // and the spot in the relevant list
    int spriteID;

    // for when it's not an artwork
    Color spriteTint;

    // transformation in background space
    Vector2 bgSpace_position;
    float bgSpace_rotation;
    Vector2 bgSpace_scale;
    // position offset
    float spritePositionOffset;

} BackgroundSprite_GenerationData;

typedef struct
{
    // the list of bg objects
    BackgroundSprite_GenerationData *backgroundGeneratedObjects;

} Background_DataStruct;


#define BACKGROUND_DATA ((Background_DataStruct *)(THIS->data_struct))




/**
 *  DECLARING ALL THE EXTRA FUNCTIONS FOR USABILITY
 */
void prepareBackgroundGenerationData(void *self, float DeltaTime);
void destroyBackgroundGenerationData(void *self, float DeltaTime);
void resetBackgroundSpritesPositionIfOutOfBounds(void *self, Vector2 position, int idx);

void rollForBackgroundObjectData(void *self, float DeltaTime, int backgroundObjectIndex);






int _BackgroundSprites_Init(void *self, float DeltaTime)
{
    // printf("%s\n","initialising sprite list");


    prepareBackgroundGenerationData(self, DeltaTime);

    return 0;
}

int _BackgroundSprites_Update(void *self, float DeltaTime)
{
    
    // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n", "background sprites update");
    // parallax
    THIS->position.x = -0.1f * (cameraPosition.x);
    THIS->position.y = -0.1f * (cameraPosition.y);

    return 0;
}


int _BackgroundSprites_Draw(void *self, float DeltaTime)
{
    printf("%s\n", "bg sprites draw call");

    setScaleFactorToLayer(self);
    for (int i = 0; i < BACKGROUND_OBJECT_COUNT; i++)
    {
        // printf("drawing %d\n",i);
        // Vector2 currSpritePosition = BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_position;

        // determine if we should index into our list
        // or if it was the star brush we rolled
        BackgroundSprite_SpriteData *spriteDataList = getSpriteListInCurrentLayer(self);
        Vector2 currentSpritePosition = Vector2Add(THIS->position, (Vector2){BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_position.x + BACKGROUND_DATA->backgroundGeneratedObjects[i].spritePositionOffset, BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_position.y});
        resetBackgroundSpritesPositionIfOutOfBounds(self, currentSpritePosition, i);
        // draw it
        RenderSpriteRelative(
            spriteDataList[BACKGROUND_DATA->backgroundGeneratedObjects[i].spriteID].sprite,
            currentSpritePosition,
            BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_scale,
            BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_rotation,
            BACKGROUND_DATA->backgroundGeneratedObjects[i].spriteTint);
    }

    printf("%s\n", "finished bg sprites draw call");

    return 0;
}

int _BackgroundSprites_Destroy(void *self, float DeltaTime)
{


    destroyBackgroundGenerationData(self, DeltaTime);

    // the data sstruct
    free(THIS->data_struct);

    return 0;
}

GameObj_Base *CreateBackgroundSprites(enum LAYER_ID layer)
{
    GameObj_Base *obj_ptr = malloc(sizeof(GameObj_Base));

    // ============================================================
    // ==== setup the data scruct data
    obj_ptr->data_struct = malloc(sizeof(Background_DataStruct));

    // ============================================================

    obj_ptr->Init_Func = &_BackgroundSprites_Init;
    obj_ptr->Update_Func = &_BackgroundSprites_Update;
    obj_ptr->Draw_Func = &_BackgroundSprites_Draw;
    obj_ptr->Destroy_Func = &_BackgroundSprites_Destroy;
    // ============================================================

    obj_ptr->position = (Vector2){0.0f, 0.0f};

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_BACKGROUND;

    obj_ptr->currentLayer = layer;

    return obj_ptr;
}

void resetBackgroundSpritesPositionIfOutOfBounds(void *self, Vector2 position, int idx)
{
    if (abs(((int)position.x) < cameraPosition.x-100))
    {
        BACKGROUND_DATA->backgroundGeneratedObjects[idx].spritePositionOffset += 100+cameraBounds.x*scaleFactor.x;
    }
}

void prepareBackgroundGenerationData(void *self, float DeltaTime)
{
    // ...

    // printf("%s\n","making bg objects list");
    // object reference array
    BACKGROUND_DATA->backgroundGeneratedObjects = (BackgroundSprite_GenerationData *)malloc(BACKGROUND_OBJECT_COUNT * sizeof(BackgroundSprite_GenerationData));

    // roll each object
    for (int i = 0; i < BACKGROUND_OBJECT_COUNT; i++)
    {
        rollForBackgroundObjectData(self, DeltaTime, i);
    }
}

void destroyBackgroundGenerationData(void *self, float DeltaTime)
{
    // object reference array
    free(BACKGROUND_DATA->backgroundGeneratedObjects);
}


void rollForBackgroundObjectData(void *self, float DeltaTime, int backgroundObjectIndex)
{
    // === prepare values
    int generatedType = 0;
    int generatedIndex = 0;
    Color generatedTint = WHITE;
    Vector2 generatedPosition = Vector2Zero();
    float generatedRotation = 0.0f;
    Vector2 generatedScale = Vector2One();
    float positionOffsetX = 0;

    // === selection roll
    int selectionRoll = abs((INT_RAND) % (getBackgroundSpriteCount(self)));
    // === get the index
    generatedIndex = selectionRoll;

    // === get the sprite type
    BackgroundSprite_SpriteData *spriteDataListTemp = getSpriteListInCurrentLayer(self);
    generatedType = spriteDataListTemp->spriteType;

    // === tinting
    int colourCount = 5;
    switch (generatedType)
    {
    default:
    case BG_SPRITETYPE_ARTWORK:
        generatedTint = WHITE;
        break;
    case BG_SPRITETYPE_TINTABLE_SMALL:
    case BG_SPRITETYPE_TINTABLE_MEDIUM:
    case BG_SPRITETYPE_TINTABLE_LARGE:
        switch (INT_RAND % colourCount)
        {
        default:
        case 0:
            generatedTint = WHITE;
            break;
        case 1:
            // lavender
            generatedTint = (Color){0xdc, 0xd3, 0xff, 255};
            break;
        case 2:
            // light cyan
            generatedTint = (Color){0xc4, 0xfa, 0xf8, 255};
            break;
        case 3:
            // light orange
            generatedTint = (Color){0xff, 0xdb, 0xcc, 255};
            break;
        case 4:
            // light yellow
            generatedTint = (Color){0xf9, 0xff, 0xb5, 255};
            break;
        }
        break;
    }

    // === positioning

    generatedPosition = (Vector2){
        (FLOAT_RAND * BACKGROUND_SPAWN_MAX_DISTANCE_X),
        (FLOAT_RAND * BACKGROUND_SPAWN_MAX_DISTANCE_Y - BACKGROUND_SPAWN_MAX_DISTANCE_Y / 2.0f)};

    // === rotation

    generatedRotation = FLOAT_RAND * 360.0f;

    // === scaling

    // generate two random percentage values
    float scaleRand = FLOAT_RAND;

    // then lerp between the minimum and maximum
    generatedScale = (Vector2){
        ((1.0f - scaleRand) * BACKGROUND_SPAWN_MIN_SCALE_X + (scaleRand)*BACKGROUND_SPAWN_MAX_SCALE_X),
        ((1.0f - scaleRand) * BACKGROUND_SPAWN_MIN_SCALE_Y + (scaleRand)*BACKGROUND_SPAWN_MAX_SCALE_Y)};

    // === assign the data we generated

    BACKGROUND_DATA->backgroundGeneratedObjects[backgroundObjectIndex] = (BackgroundSprite_GenerationData){
        // type
        generatedType,
        // ID
        generatedIndex,
        // tint
        generatedTint,
        // position
        generatedPosition,
        // rotation
        generatedRotation,
        // scale
        generatedScale,
        // position offset
        positionOffsetX};
}



#undef DEBUG_SPAMMER_PRINTF_PREFIX