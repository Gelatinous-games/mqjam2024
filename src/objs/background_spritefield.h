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
#include "../palette_lib.h"
#include "../SpriteLibrary.h"

enum SPRITE_LAYER
{
    SMALL = 0,
    MEDIUM = 1,
    LARGE = 2,
    MASSIVE = 3,
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
    int spawnCount;

} Background_DataStruct;

#define BACKGROUND_SPRITE_LAYER3_SPAWN_MAX 3
#define ENVIRONMENT_POSITION_GENERATION_BUFFER 50.0f

#define BACKGROUND_SPRITE_COUNT_ALL (BACKGROUND_SPRITE_COUNT_LAYER0 + BACKGROUND_SPRITE_COUNT_LAYER1 + BACKGROUND_SPRITE_COUNT_LAYER2 + BACKGROUND_SPRITE_COUNT_LAYER3)

BackgroundSprite_SpriteData *backgroundSpriteList_Layer0;
BackgroundSprite_SpriteData *backgroundSpriteList_Layer1;
BackgroundSprite_SpriteData *backgroundSpriteList_Layer2;
// TODO: dust art goes on layer 3/4/5
BackgroundSprite_SpriteData *backgroundSpriteList_Layer3;

#define ENVIRONMENT_SCALE 20.0f


int _BackgroundSprites_Init(void *self, float DeltaTime);
int _BackgroundSprites_Update(void *self, float DeltaTime);
int _BackgroundSprites_Draw(void *self, float DeltaTime);
int _BackgroundSprites_Destroy(void *self, float DeltaTime);


/**
 *  DECLARING ALL THE EXTRA FUNCTIONS FOR USABILITY
 */
void prepareBackgroundSprites(void *self, float DeltaTime);
void destroyBackgroundSprites(void *self, float DeltaTime);

void prepareBackgroundGenerationData(void *self, float DeltaTime);
void destroyBackgroundGenerationData(void *self, float DeltaTime);

void resetBackgroundSpritesPositionIfOutOfBounds(void *self, Vector2 position, Vector2 scale, int idx);
void rollForBackgroundObjectData(void *self, float DeltaTime, int backgroundObjectIndex);

BackgroundSprite_SpriteData *getSpriteListInCurrentLayer(void *self);

void setScaleFactorToLayer(void *self);
int getBackgroundSpriteCount(void *self);
