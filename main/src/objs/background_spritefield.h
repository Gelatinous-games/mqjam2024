#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "../base.h"
#include "../settings.h"
#include "../misc_util.h"

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

enum SPRITE_LAYER
{
    SMALL = 0,
    MEDIUM = 1,
    LARGE = 2,
};

typedef struct
{
    // the particular sprite
    Sprite *sprite;
    enum BACKGROUND_SPRITE_TYPE spriteType;
    int spriteLayer;
} BackgroundSprite_SpriteData;

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

#define BACKGROUND_SPRITE_COUNT_ARTWORKS 10
#define BACKGROUND_SPRITE_COUNT_SMALL 5
#define BACKGROUND_SPRITE_COUNT_MEDIUM 7
#define BACKGROUND_SPRITE_COUNT_LARGE 10

// ToDo: need to be updated with real num
#define BACKGROUND_SPRITE_COUNT_LAYER0 18
#define BACKGROUND_SPRITE_COUNT_LAYER1 9
#define BACKGROUND_SPRITE_COUNT_LAYER2 5

#define BACKGROUND_SPRITE_COUNT_ALL (BACKGROUND_SPRITE_COUNT_LAYER0 + BACKGROUND_SPRITE_COUNT_LAYER1 + BACKGROUND_SPRITE_COUNT_LAYER2)

BackgroundSprite_SpriteData *backgroundSpriteList_Layer0;
BackgroundSprite_SpriteData *backgroundSpriteList_Layer1;
BackgroundSprite_SpriteData *backgroundSpriteList_Layer2;

#define BACKGROUND_DATA ((Background_DataStruct *)(THIS->data_struct))

/**
 *  DECLARING ALL THE EXTRA FUNCTIONS FOR USABILITY
 */
void prepareBackgroundSprites(void *self, float DeltaTime);
void destroyBackgroundSprites(void *self, float DeltaTime);

void prepareBackgroundGenerationData(void *self, float DeltaTime);
void destroyBackgroundGenerationData(void *self, float DeltaTime);

void resetBackgroundSpritesPositionIfOutOfBounds(void *self, Vector2 position, int idx);
void rollForBackgroundObjectData(void *self, float DeltaTime, int backgroundObjectIndex);

BackgroundSprite_SpriteData *getSpriteListInCurrentLayer(void *self);
