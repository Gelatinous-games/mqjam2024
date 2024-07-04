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

enum BACKGROUND_SPRITE_TYPE {
    // ...
    BG_SPRITETYPE_ARTWORK_SPRITE    = 0,
    BG_SPRITETYPE_TINTABLE_SMALL    = 1,
    BG_SPRITETYPE_TINTABLE_MEDIUM   = 2,
    BG_SPRITETYPE_TINTABLE_LARGE    = 3
};


typedef struct {
    // the particular sprite
    Sprite *sprite;
} BackgroundSprite_SpriteData;



typedef struct {
    // for whether it's a background image we can tint or not
    int spriteType;
    // and the spot in the relevant list
    int spriteID;

    // for when it's not an artwork
    Color spriteTint;
} BackgroundSprite_GenerationData;





typedef struct
{
    // the list of bg objects
    BackgroundSprite_GenerationData *backgroundGeneratedObjects;

    Vector2 *backgroundObjectPositions;
    float *backgroundObjectRotations;
    Vector2 *backgroundObjectScales;
} Background_DataStruct;


#define BACKGROUND_SPRITE_COUNT_ARTWORKS 6
#define BACKGROUND_SPRITE_COUNT_SMALL 5
#define BACKGROUND_SPRITE_COUNT_MEDIUM 7
#define BACKGROUND_SPRITE_COUNT_LARGE 10

#define BACKGROUND_SPRITE_COUNT_ALL (BACKGROUND_SPRITE_COUNT_ARTWORKS+BACKGROUND_SPRITE_COUNT_SMALL+BACKGROUND_SPRITE_COUNT_MEDIUM+BACKGROUND_SPRITE_COUNT_LARGE)

Sprite **backgroundSpriteList_artworks;
Sprite **backgroundSpriteList_small;
Sprite **backgroundSpriteList_medium;
Sprite **backgroundSpriteList_large;




#define BACKGROUND_DATA ((Background_DataStruct *)(THIS->data_struct))


#define BACKGROUND_OBJECT_COUNT 100

#define MAX_BACKGROUND_SPAWN_DISTANCE_X 400.0f
#define MAX_BACKGROUND_SPAWN_DISTANCE_Y 10.0f


#define MIN_BACKGROUND_SPAWN_SCALE_X 0.5f
#define MAX_BACKGROUND_SPAWN_SCALE_X 3.0f
#define MIN_BACKGROUND_SPAWN_SCALE_Y 0.5f
#define MAX_BACKGROUND_SPAWN_SCALE_Y 3.0f





/**
 *  DECLARING ALL THE EXTRA FUNCTIONS FOR USABILITY
 */
void prepareBackgroundSprites(void *self, float DeltaTime);
void destroyBackgroundSprites(void *self, float DeltaTime);
void prepareBackgroundGenerationData(void *self, float DeltaTime);
void destroyBackgroundGenerationData(void *self, float DeltaTime);

void rollForBackgroundObjectData(void *self, float DeltaTime, int backgroundObjectIndex);
Sprite *getCurrentSprite(void *self, float DeltaTime, BackgroundSprite_GenerationData indexOfObject);



int _BackgroundStars_Init(void *self, float DeltaTime)
{
    // printf("%s\n","initialising sprite list");
    
    // loads our sprites ready for use
    prepareBackgroundSprites(self,DeltaTime);

    prepareBackgroundGenerationData(self, DeltaTime);

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
        Vector2 currSpritePosition = BACKGROUND_DATA->backgroundObjectPositions[i];

        // determine if we should index into our list
        // or if it was the star brush we rolled
        Sprite *currSpritePointer = getCurrentSprite(self, DeltaTime, BACKGROUND_DATA->backgroundGeneratedObjects[i]);
        //.. ..
        RenderSpriteRelative(
            currSpritePointer,
            (Vector2){
                THIS->position.x + currSpritePosition.x,
                THIS->position.x + currSpritePosition.y
            },
            BACKGROUND_DATA->backgroundObjectScales[i],
            BACKGROUND_DATA->backgroundObjectRotations[i],
            // TODO: should mess with tint when it's star brush sprite
            WHITE
        );
    }

    

    return 0;
}

int _BackgroundStars_Destroy(void *self, float DeltaTime)
{

    destroyBackgroundSprites(self, DeltaTime);
    
    destroyBackgroundGenerationData(self, DeltaTime);

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



void prepareBackgroundSprites(void *self, float DeltaTime){

    int index = 0;

    // malloc the data
    index = 0;
    backgroundSpriteList_artworks = (Sprite **)malloc(BACKGROUND_SPRITE_COUNT_ARTWORKS * sizeof(Sprite *));
    backgroundSpriteList_artworks[index++] = CreateSprite("resources/background/bg0.png");
    backgroundSpriteList_artworks[index++] = CreateSprite("resources/background/bg1.png");
    backgroundSpriteList_artworks[index++] = CreateSprite("resources/background/bg2.png");
    backgroundSpriteList_artworks[index++] = CreateSprite("resources/background/bg3.png");
    backgroundSpriteList_artworks[index++] = CreateSprite("resources/background/bg4.png");
    backgroundSpriteList_artworks[index++] = CreateSprite("resources/background/bg5.png");

    index = 0;
    backgroundSpriteList_small = (Sprite **)malloc(BACKGROUND_SPRITE_COUNT_SMALL * sizeof(Sprite *));
    backgroundSpriteList_small[index++] = CreateSprite("resources/brushes/brush_small_glitter_01.png");
    backgroundSpriteList_small[index++] = CreateSprite("resources/brushes/brush_small_glitter_02.png");
    backgroundSpriteList_small[index++] = CreateSprite("resources/brushes/brush_small_star_01.png");
    backgroundSpriteList_small[index++] = CreateSprite("resources/brushes/brush_small_star_02.png");
    backgroundSpriteList_small[index++] = CreateSprite("resources/brushes/brush_small_star_03.png");

    index = 0;
    backgroundSpriteList_medium = (Sprite **)malloc(BACKGROUND_SPRITE_COUNT_MEDIUM * sizeof(Sprite *));
    backgroundSpriteList_medium[index++] = CreateSprite("resources/brushes/brush_medium_dust_01.png");
    backgroundSpriteList_medium[index++] = CreateSprite("resources/brushes/brush_medium_dust_02.png");
    backgroundSpriteList_medium[index++] = CreateSprite("resources/brushes/brush_medium_dust_03.png");
    backgroundSpriteList_medium[index++] = CreateSprite("resources/brushes/brush_medium_flare_01.png");
    backgroundSpriteList_medium[index++] = CreateSprite("resources/brushes/brush_medium_glitter_01.png");
    backgroundSpriteList_medium[index++] = CreateSprite("resources/brushes/brush_medium_glitter_02.png");
    backgroundSpriteList_medium[index++] = CreateSprite("resources/brushes/brush_medium_glow_01.png");

    index = 0;
    backgroundSpriteList_large = (Sprite **)malloc(BACKGROUND_SPRITE_COUNT_LARGE * sizeof(Sprite *));
    backgroundSpriteList_large[index++] = CreateSprite("resources/brushes/brush_large_constellation_01.png");
    backgroundSpriteList_large[index++] = CreateSprite("resources/brushes/brush_large_constellation_02.png");
    backgroundSpriteList_large[index++] = CreateSprite("resources/brushes/brush_large_constellation_03.png");
    backgroundSpriteList_large[index++] = CreateSprite("resources/brushes/brush_large_constellation_04.png");
    backgroundSpriteList_large[index++] = CreateSprite("resources/brushes/brush_large_constellation_05.png");
    backgroundSpriteList_large[index++] = CreateSprite("resources/brushes/brush_large_constellation_06.png");
    backgroundSpriteList_large[index++] = CreateSprite("resources/brushes/brush_large_dust_01.png");
    backgroundSpriteList_large[index++] = CreateSprite("resources/brushes/brush_large_dust_02.png");
    backgroundSpriteList_large[index++] = CreateSprite("resources/brushes/brush_large_dust_03.png");
    backgroundSpriteList_large[index++] = CreateSprite("resources/brushes/brush_large_flare_01.png");
    
}


void destroyBackgroundSprites(void *self, float DeltaTime){

    // ARTWORKS
    for (int i = 0; i < BACKGROUND_SPRITE_COUNT_ARTWORKS; i++) {
        DestroySprite(backgroundSpriteList_artworks[i]);
    }
    free(backgroundSpriteList_artworks);

    // SMALL
    for (int i = 0; i < BACKGROUND_SPRITE_COUNT_SMALL; i++) {
        DestroySprite(backgroundSpriteList_small[i]);
    }
    free(backgroundSpriteList_small);

    // MEDIUM
    for (int i = 0; i < BACKGROUND_SPRITE_COUNT_MEDIUM; i++) {
        DestroySprite(backgroundSpriteList_medium[i]);
    }
    free(backgroundSpriteList_medium);

    // LARGE
    for (int i = 0; i < BACKGROUND_SPRITE_COUNT_LARGE; i++) {
        DestroySprite(backgroundSpriteList_large[i]);
    }
    free(backgroundSpriteList_large);

}


void prepareBackgroundGenerationData(void *self, float DeltaTime){
    // ...

    // printf("%s\n","making bg objects list");
    // object reference array
    BACKGROUND_DATA->backgroundGeneratedObjects = (BackgroundSprite_GenerationData *)malloc(BACKGROUND_OBJECT_COUNT*sizeof(BackgroundSprite_GenerationData));

    // printf("%s\n","making bg objects positions");
    // positions array
    BACKGROUND_DATA->backgroundObjectPositions = (Vector2 *)malloc(BACKGROUND_OBJECT_COUNT*sizeof(Vector2));
    // roations
    BACKGROUND_DATA->backgroundObjectRotations = (float *)malloc(BACKGROUND_OBJECT_COUNT*sizeof(float));
    // scale array
    BACKGROUND_DATA->backgroundObjectScales = (Vector2 *)malloc(BACKGROUND_OBJECT_COUNT*sizeof(Vector2));


    // roll each object
    for (int i = 0; i < BACKGROUND_OBJECT_COUNT; i++){

        rollForBackgroundObjectData(self, DeltaTime, i);
    }
}


void destroyBackgroundGenerationData(void *self, float DeltaTime){

    // scale/position data
    free(BACKGROUND_DATA->backgroundObjectScales);
    free(BACKGROUND_DATA->backgroundObjectRotations);
    free(BACKGROUND_DATA->backgroundObjectPositions);

    // object reference array
    free(BACKGROUND_DATA->backgroundGeneratedObjects);

}




void rollForBackgroundObjectData(void *self, float DeltaTime, int backgroundObjectIndex){
    // === selection roll
    int selectionRoll = abs((INT_RAND)%(BACKGROUND_SPRITE_COUNT_ALL));


    // === assign selection
    // ------ ARTWORKS
    // TODO: implement selecting, but just mod for artwork count rn
    BACKGROUND_DATA->backgroundGeneratedObjects[backgroundObjectIndex] = (BackgroundSprite_GenerationData){
        // type
        BG_SPRITETYPE_ARTWORK_SPRITE,
        // ID
        selectionRoll%BACKGROUND_SPRITE_COUNT_ARTWORKS,
        // tint
        WHITE
    };

    // === positioning
    BACKGROUND_DATA->backgroundObjectPositions[backgroundObjectIndex] = (Vector2){
        (FLOAT_RAND*MAX_BACKGROUND_SPAWN_DISTANCE_X),
        (FLOAT_RAND*MAX_BACKGROUND_SPAWN_DISTANCE_Y-MAX_BACKGROUND_SPAWN_DISTANCE_Y/2.0f)
    };

    // === rotation
    BACKGROUND_DATA->backgroundObjectRotations[backgroundObjectIndex] = FLOAT_RAND*360.0f;

    // === scaling
    // generate two random percentage values 
    float xScaleRand = FLOAT_RAND;
    float yScaleRand = FLOAT_RAND;
    // then lerp between the minimum and maximum
    BACKGROUND_DATA->backgroundObjectScales[backgroundObjectIndex] = (Vector2){
        ((1.0f-xScaleRand)*MIN_BACKGROUND_SPAWN_SCALE_X+(xScaleRand)*MAX_BACKGROUND_SPAWN_SCALE_X),
        ((1.0f-yScaleRand)*MIN_BACKGROUND_SPAWN_SCALE_Y+(yScaleRand)*MAX_BACKGROUND_SPAWN_SCALE_Y)
    };
}

Sprite *getCurrentSprite(void *self, float DeltaTime, BackgroundSprite_GenerationData spriteGenerationData){
    Sprite **spriteListToUse;
    switch (spriteGenerationData.spriteType){
        case BG_SPRITETYPE_ARTWORK_SPRITE:
            spriteListToUse = backgroundSpriteList_artworks;
            break;
        case BG_SPRITETYPE_TINTABLE_SMALL:
            spriteListToUse = backgroundSpriteList_small;
            break;
        case BG_SPRITETYPE_TINTABLE_MEDIUM:
            spriteListToUse = backgroundSpriteList_medium;
            break;
        case BG_SPRITETYPE_TINTABLE_LARGE:
            spriteListToUse = backgroundSpriteList_large;
            break;
        default:
            printf("selection failure with type: %d\n", spriteGenerationData.spriteType);
            return backgroundSpriteList_artworks[0];
    }
    return spriteListToUse[spriteGenerationData.spriteID];
}