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
    BG_SPRITETYPE_ARTWORK           = 0,
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

    // transformation in background space
    Vector2 bgSpace_position;
    float   bgSpace_rotation;
    Vector2 bgSpace_scale;
} BackgroundSprite_GenerationData;





typedef struct
{
    // the list of bg objects
    BackgroundSprite_GenerationData *backgroundGeneratedObjects;

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


/**
 *  DECLARING ALL THE EXTRA FUNCTIONS FOR USABILITY
 */
void prepareBackgroundSprites(void *self, float DeltaTime);
void destroyBackgroundSprites(void *self, float DeltaTime);
void prepareBackgroundGenerationData(void *self, float DeltaTime);
void destroyBackgroundGenerationData(void *self, float DeltaTime);

void rollForBackgroundObjectData(void *self, float DeltaTime, int backgroundObjectIndex);
Sprite **spriteTypeToSpriteList(void *self, float DeltaTime, int spriteType);



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
        Vector2 currSpritePosition = BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_position;

        // determine if we should index into our list
        // or if it was the star brush we rolled
        Sprite **spriteList = spriteTypeToSpriteList(self, DeltaTime, BACKGROUND_DATA->backgroundGeneratedObjects[i].spriteType);
        // draw it
        RenderSpriteRelative(
            spriteList[BACKGROUND_DATA->backgroundGeneratedObjects[i].spriteID],
            Vector2Add(THIS->position, BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_position),
            BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_scale,
            BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_rotation,
            BACKGROUND_DATA->backgroundGeneratedObjects[i].spriteTint
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

    // roll each object
    for (int i = 0; i < BACKGROUND_OBJECT_COUNT; i++){
        rollForBackgroundObjectData(self, DeltaTime, i);
    }
}


void destroyBackgroundGenerationData(void *self, float DeltaTime){

    // object reference array
    free(BACKGROUND_DATA->backgroundGeneratedObjects);

}




void rollForBackgroundObjectData(void *self, float DeltaTime, int backgroundObjectIndex){
    // === prepare values
    int generatedType = 0;
    int generatedIndex = 0;
    Color generatedTint = WHITE;
    Vector2 generatedPosition = Vector2Zero();
    float generatedRotation = 0.0f;
    Vector2 generatedScale = Vector2One();


    // === selection roll
    int selectionRoll = abs((INT_RAND)%(BACKGROUND_SPRITE_COUNT_ALL));

    // === determine type/index

    int firstSmall = BACKGROUND_SPRITE_COUNT_ARTWORKS;
    int firstMedium = BACKGROUND_SPRITE_COUNT_ARTWORKS+BACKGROUND_SPRITE_COUNT_SMALL;
    int firstLarge = BACKGROUND_SPRITE_COUNT_ARTWORKS+BACKGROUND_SPRITE_COUNT_SMALL+BACKGROUND_SPRITE_COUNT_MEDIUM;
    
    
    // ARTWORK WAS ROLLED
    if(selectionRoll < firstSmall){
        generatedType = BG_SPRITETYPE_ARTWORK;
        generatedIndex = selectionRoll;
        printf("rolled [%d]: which is %s[%d]\n", selectionRoll, "ARTWORK",generatedIndex);
    }
    // SMALL WAS ROLLED
    else if(selectionRoll < firstMedium){
        generatedType = BG_SPRITETYPE_TINTABLE_SMALL;
        generatedIndex = selectionRoll - firstSmall;
        printf("rolled [%d]: which is %s[%d]\n", selectionRoll, "SMALL", generatedIndex);
    }
    // MEDIUM WAS ROLLED
    else if(selectionRoll < firstLarge){
        generatedType = BG_SPRITETYPE_TINTABLE_MEDIUM;
        generatedIndex = selectionRoll - firstMedium;
        printf("rolled [%d]: which is %s[%d]\n", selectionRoll, "MEDIUM", generatedIndex);
    }
    // LARGE WAS ROLLED
    else {
        generatedType = BG_SPRITETYPE_TINTABLE_LARGE;
        generatedIndex = selectionRoll - firstLarge;
        printf("rolled [%d]: which is %s[%d]\n", selectionRoll, "LARGE", generatedIndex);
    }

    // === tinting
    int colourCount = 5;
    switch(generatedType){
        default:
        case BG_SPRITETYPE_ARTWORK:
            generatedTint = WHITE;
            break;
        case BG_SPRITETYPE_TINTABLE_SMALL:
        case BG_SPRITETYPE_TINTABLE_MEDIUM:
        case BG_SPRITETYPE_TINTABLE_LARGE:
            switch(INT_RAND%colourCount){
                default:
                case 0:
                    generatedTint = WHITE;
                    break;
                case 1:
                    // lavender
                    generatedTint = (Color){ 0xdc, 0xd3, 0xff, 255 };
                    break;
                case 2:
                    // light cyan
                    generatedTint = (Color){ 0xc4, 0xfa, 0xf8, 255 };
                    break;
                case 3:
                    // light orange
                    generatedTint = (Color){ 0xff, 0xdb, 0xcc, 255 };
                    break;
                case 4:
                    // light yellow
                    generatedTint = (Color){ 0xf9, 0xff, 0xb5, 255 };
                    break;
            }
            break;
    }
    
    
    // === positioning

    generatedPosition = (Vector2){
        (FLOAT_RAND*BACKGROUND_SPAWN_MAX_DISTANCE_X),
        (FLOAT_RAND*BACKGROUND_SPAWN_MAX_DISTANCE_Y-BACKGROUND_SPAWN_MAX_DISTANCE_Y/2.0f)
    };

    // === rotation

    generatedRotation = FLOAT_RAND*360.0f;

    // === scaling

    // generate two random percentage values 
    float scaleRand = FLOAT_RAND;

    // then lerp between the minimum and maximum
    generatedScale = (Vector2){
        ((1.0f-scaleRand)*BACKGROUND_SPAWN_MIN_SCALE_X+(scaleRand)*BACKGROUND_SPAWN_MAX_SCALE_X),
        ((1.0f-scaleRand)*BACKGROUND_SPAWN_MIN_SCALE_Y+(scaleRand)*BACKGROUND_SPAWN_MAX_SCALE_Y)
    };
    


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
        generatedScale
    };
}


Sprite **spriteTypeToSpriteList(void *self, float DeltaTime, int spriteType){
    switch (spriteType){
        default:
        case BG_SPRITETYPE_ARTWORK:
            return backgroundSpriteList_artworks;
        case BG_SPRITETYPE_TINTABLE_SMALL:
            return backgroundSpriteList_small;
        case BG_SPRITETYPE_TINTABLE_MEDIUM:
            return backgroundSpriteList_medium;
        case BG_SPRITETYPE_TINTABLE_LARGE:
            return backgroundSpriteList_large;
    }
}