
#ifndef _sprite
#define _sprite
#include "../sprite.c"
#endif


#ifndef _camera
#define _camera
#include "../camera.c"
#endif

#include "../obj_register.h"
#include "../settings.h"

enum SPRITE_LAYER
{
    SMALL = 0,
    MEDIUM = 1,
    LARGE = 2,
};



enum BACKGROUND_SPRITE_TYPE {
    // ...
    BG_SPRITETYPE_ARTWORK           = 0,
    BG_SPRITETYPE_TINTABLE_SMALL    = 1,
    BG_SPRITETYPE_TINTABLE_MEDIUM   = 2,
    BG_SPRITETYPE_TINTABLE_LARGE    = 3
};



typedef struct
{
    // the particular sprite
    Sprite *sprite;
    enum BACKGROUND_SPRITE_TYPE spriteType;
    int spriteLayer;
} BackgroundSprite_SpriteData;



#define BACKGROUND_SPRITE_COUNT_ARTWORKS 10
#define BACKGROUND_SPRITE_COUNT_SMALL 5
#define BACKGROUND_SPRITE_COUNT_MEDIUM 7
#define BACKGROUND_SPRITE_COUNT_LARGE 10

// ToDo: need to be updated with real num
#define BACKGROUND_SPRITE_COUNT_LAYER0 18
#define BACKGROUND_SPRITE_COUNT_LAYER1 9
#define BACKGROUND_SPRITE_COUNT_LAYER2 5

#define BACKGROUND_SPRITE_COUNT_ALL (BACKGROUND_SPRITE_COUNT_LAYER0 + BACKGROUND_SPRITE_COUNT_LAYER1 + BACKGROUND_SPRITE_COUNT_LAYER2)

// BackgroundSprite_SpriteData *backgroundSpriteList_artworks;
// BackgroundSprite_SpriteData *backgroundSpriteList_small;
// BackgroundSprite_SpriteData *backgroundSpriteList_medium;
// BackgroundSprite_SpriteData *backgroundSpriteList_large;

BackgroundSprite_SpriteData *backgroundSpriteList_Layer0;
BackgroundSprite_SpriteData *backgroundSpriteList_Layer1;
BackgroundSprite_SpriteData *backgroundSpriteList_Layer2;




void prepareBackgroundSprites();
void destroyBackgroundSprites();

void setScaleFactorToLayer(void *self);

int getBackgroundSpriteCount(void *self);
BackgroundSprite_SpriteData *getSpriteListInCurrentLayer(void *self);





void prepareBackgroundSprites()
{
    // malloc the data
    int index = 0;

    backgroundSpriteList_Layer0 = (BackgroundSprite_SpriteData *)malloc(BACKGROUND_SPRITE_COUNT_LAYER0 * sizeof(BackgroundSprite_SpriteData));
    // artwork 9
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/background/bg_art_01.png"), BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/background/bg_art_02.png"), BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/background/bg_art_03.png"), BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/background/bg_art_04.png"), BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/background/bg_art_05.png"), BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/background/bg_art_06.png"), BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_2};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/background/bg_art_07.png"), BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/background/bg_art_08.png"), BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/background/bg_art_09.png"), BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0};

    // medium 6
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_medium_dust_01.png"), BG_SPRITETYPE_TINTABLE_MEDIUM, LAYER_BACKGROUND_STARSCAPE_0};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_medium_dust_02.png"), BG_SPRITETYPE_TINTABLE_MEDIUM, LAYER_BACKGROUND_STARSCAPE_0};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_medium_dust_03.png"), BG_SPRITETYPE_TINTABLE_MEDIUM, LAYER_BACKGROUND_STARSCAPE_0};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_medium_glitter_01.png"), BG_SPRITETYPE_TINTABLE_MEDIUM, LAYER_BACKGROUND_STARSCAPE_0};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_medium_glitter_02.png"), BG_SPRITETYPE_TINTABLE_MEDIUM, LAYER_BACKGROUND_STARSCAPE_0};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_medium_glow_01.png"), BG_SPRITETYPE_TINTABLE_MEDIUM, LAYER_BACKGROUND_STARSCAPE_0};

    // large 3
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_large_dust_01.png"), BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_0};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_large_dust_02.png"), BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_0};
    backgroundSpriteList_Layer0[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_large_dust_03.png"), BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_0};
    



    index = 0;
    backgroundSpriteList_Layer1 = (BackgroundSprite_SpriteData *)malloc(BACKGROUND_SPRITE_COUNT_LAYER1 * sizeof(BackgroundSprite_SpriteData));

    // small 3
    backgroundSpriteList_Layer1[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_small_star_01.png"), BG_SPRITETYPE_TINTABLE_SMALL, LAYER_BACKGROUND_STARSCAPE_1};
    backgroundSpriteList_Layer1[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_small_star_02.png"), BG_SPRITETYPE_TINTABLE_SMALL, LAYER_BACKGROUND_STARSCAPE_1};
    backgroundSpriteList_Layer1[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_small_star_03.png"), BG_SPRITETYPE_TINTABLE_SMALL, LAYER_BACKGROUND_STARSCAPE_1};
    // large 6
    backgroundSpriteList_Layer1[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_large_constellation_01.png"), BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_1};
    backgroundSpriteList_Layer1[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_large_constellation_02.png"), BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_1};
    backgroundSpriteList_Layer1[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_large_constellation_03.png"), BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_1};
    backgroundSpriteList_Layer1[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_large_constellation_04.png"), BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_1};
    backgroundSpriteList_Layer1[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_large_constellation_05.png"), BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_1};
    backgroundSpriteList_Layer1[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_large_constellation_06.png"), BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_1};
    




    index = 0;

    backgroundSpriteList_Layer2 = (BackgroundSprite_SpriteData *)malloc(BACKGROUND_SPRITE_COUNT_LAYER2 * sizeof(BackgroundSprite_SpriteData));
    // artwork 1
    backgroundSpriteList_Layer2[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/background/bg_art_10.png"), BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_2};
    // small 2
    backgroundSpriteList_Layer2[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_small_glitter_01.png"), BG_SPRITETYPE_TINTABLE_SMALL, LAYER_BACKGROUND_STARSCAPE_2};
    backgroundSpriteList_Layer2[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_small_glitter_02.png"), BG_SPRITETYPE_TINTABLE_SMALL, LAYER_BACKGROUND_STARSCAPE_2};
    // medium 1
    backgroundSpriteList_Layer2[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_medium_flare_01.png"), BG_SPRITETYPE_TINTABLE_MEDIUM, LAYER_BACKGROUND_STARSCAPE_2};
    // large 1
    backgroundSpriteList_Layer2[index++] = (BackgroundSprite_SpriteData){CreateSprite("resources/brushes/brush_large_flare_01.png"), BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_2};
}

void destroyBackgroundSprites()
{
    // LAYER 0
    for (int i = 0; i < BACKGROUND_SPRITE_COUNT_LAYER0; i++)
    {
        DestroySprite(backgroundSpriteList_Layer0[i].sprite);
    }
    free(backgroundSpriteList_Layer0);
    backgroundSpriteList_Layer0 = 0;

    // LAYER 1
    for (int i = 0; i < BACKGROUND_SPRITE_COUNT_LAYER1; i++)
    {
        DestroySprite(backgroundSpriteList_Layer1[i].sprite);
    }
    free(backgroundSpriteList_Layer1);
    backgroundSpriteList_Layer1 = 0;

    // LAYER 2
    for (int i = 0; i < BACKGROUND_SPRITE_COUNT_LAYER2; i++)
    {
        DestroySprite(backgroundSpriteList_Layer2[i].sprite);
    }
    free(backgroundSpriteList_Layer2);
    backgroundSpriteList_Layer2 = 0;
}








void setScaleFactorToLayer(void *self)
{
    switch (THIS->currentLayer)
    {
    default:
    case LAYER_BACKGROUND_STARSCAPE_0:
        scaleFactor = (Vector2){3, 3};
        break;
    case LAYER_BACKGROUND_STARSCAPE_1:
        scaleFactor = (Vector2){2, 2};
        break;
    case LAYER_BACKGROUND_STARSCAPE_2:
        scaleFactor = (Vector2){1, 1};
        break;
    }
}


int getBackgroundSpriteCount(void *self)
{
    switch (THIS->currentLayer)
    {
    default:
    case LAYER_BACKGROUND_STARSCAPE_0:
        return BACKGROUND_SPRITE_COUNT_LAYER0;
    case LAYER_BACKGROUND_STARSCAPE_1:
        return BACKGROUND_SPRITE_COUNT_LAYER1;
    case LAYER_BACKGROUND_STARSCAPE_2:
        return BACKGROUND_SPRITE_COUNT_LAYER2;
    }
}


BackgroundSprite_SpriteData *getSpriteListInCurrentLayer(void *self)
{
    switch (THIS->currentLayer)
    {
    case LAYER_BACKGROUND_STARSCAPE_0:
        return backgroundSpriteList_Layer0;
    case LAYER_BACKGROUND_STARSCAPE_1:
        return backgroundSpriteList_Layer1;
    case LAYER_BACKGROUND_STARSCAPE_2:
        return backgroundSpriteList_Layer2;
    default:
        printf("ERROR: background.c currentLayer is not a background layer");
        return backgroundSpriteList_Layer0;
    }
}

