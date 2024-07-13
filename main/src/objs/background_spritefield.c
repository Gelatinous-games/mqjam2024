#include "background_spritefield.h"

#define BACKGROUND_DATA ((Background_DataStruct *)(THIS->data_struct))

int _BackgroundSprites_Init(void *self, float DeltaTime)
{
    // printf("%s\n","initialising sprite list");
    BACKGROUND_DATA->spawnCount = 0;

    if(THIS->currentLayer == LAYER_BACKGROUND_STARSCAPE_3){
            printf("layer was %d, so using %d\n",THIS->currentLayer,BACKGROUND_SPRITE_LAYER3_SPAWN_MAX);
            BACKGROUND_DATA->spawnCount = BACKGROUND_SPRITE_LAYER3_SPAWN_MAX;
    }
    else {
        // ...
        printf("layer was %d, so using %d\n",THIS->currentLayer,BACKGROUND_OBJECT_COUNT);
        BACKGROUND_DATA->spawnCount = BACKGROUND_OBJECT_COUNT;
    }

    // loads our sprites ready for use
    prepareBackgroundSprites(self, DeltaTime);

    prepareBackgroundGenerationData(self, DeltaTime);

    return 0;
}

int _BackgroundSprites_Update(void *self, float DeltaTime)
{
    // parallax
    THIS->position.x = -0.1f * (cameraPosition.x);
    THIS->position.y = -0.1f * (cameraPosition.y);

    return 0;
}

void setScaleFactorToLayer(void *self)
{
    switch (THIS->currentLayer)
    {
    default:
    case LAYER_BACKGROUND_STARSCAPE_0:
        scaleFactor = (Vector2){5, 5};
        break;
    case LAYER_BACKGROUND_STARSCAPE_1:
        scaleFactor = (Vector2){3, 3};
        break;
    case LAYER_BACKGROUND_STARSCAPE_2:
        scaleFactor = (Vector2){1, 1};
        break;
    case LAYER_BACKGROUND_STARSCAPE_3:
        scaleFactor = (Vector2){1.0f, 1.0f};
        break;
    }
}

int _BackgroundSprites_Draw(void *self, float DeltaTime)
{
    int numberOfSprites = (BACKGROUND_DATA->spawnCount);
    setScaleFactorToLayer(self);
    for (int i = 0; i < numberOfSprites; i++)
    {
        // printf("drawing %d\n",i);
        Vector2 currSpritePosition = BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_position;

        // determine if we should index into our list
        // or if it was the star brush we rolled
        BackgroundSprite_SpriteData *spriteDataList = getSpriteListInCurrentLayer(self);
        Vector2 currentSpritePosition = Vector2Add(THIS->position, (Vector2){BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_position.x + BACKGROUND_DATA->backgroundGeneratedObjects[i].spritePositionOffset, BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_position.y});
        Vector2 currSpriteScale = GetScaledSize(BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_scale);
        resetBackgroundSpritesPositionIfOutOfBounds(self, currentSpritePosition, currSpriteScale, i);
        
        // draw it
        RenderSpriteRelative(
            spriteDataList[BACKGROUND_DATA->backgroundGeneratedObjects[i].spriteID].sprite,
            currentSpritePosition,
            BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_scale,
            BACKGROUND_DATA->backgroundGeneratedObjects[i].bgSpace_rotation,
            BACKGROUND_DATA->backgroundGeneratedObjects[i].spriteTint);
    }

    return 0;
}

int _BackgroundSprites_Destroy(void *self, float DeltaTime)
{

    destroyBackgroundSprites(self, DeltaTime);

    destroyBackgroundGenerationData(self, DeltaTime);

    // the data sstruct
    free(THIS->data_struct);
    THIS->data_struct = 0;

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

void resetBackgroundSpritesPositionIfOutOfBounds(void *self, Vector2 position, Vector2 scale, int idx)
{
    if (((position.x * scale.x)) < (cameraPosition.x - cameraBounds.x))
    {
        rollForBackgroundObjectData(self,0.0f,idx);
        // BACKGROUND_DATA->backgroundGeneratedObjects[idx].spritePositionOffset += 100+cameraBounds.x*scaleFactor.x;
    }
}

void prepareBackgroundSprites(void *self, float DeltaTime)
{

    // malloc the data
    // using the sprite pointer from our library at the given index

    int index = 0;
    switch (THIS->currentLayer)
    {
    case LAYER_BACKGROUND_STARSCAPE_0:
        /* code */
        index = 0;

        backgroundSpriteList_Layer0 = (BackgroundSprite_SpriteData *)malloc(_SPRITELIBRARY_BACKGROUNDLAYER0_SPRITELIST_LENGTH * sizeof(BackgroundSprite_SpriteData));
        // artwork 11
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_0}; index++;

        // medium 6
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_TINTABLE_MEDIUM, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_TINTABLE_MEDIUM, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_TINTABLE_MEDIUM, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_TINTABLE_MEDIUM, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_TINTABLE_MEDIUM, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_TINTABLE_MEDIUM, LAYER_BACKGROUND_STARSCAPE_0}; index++;

        // large 3
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        backgroundSpriteList_Layer0[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer0_spritelist[index], BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_0}; index++;
        break;
    case LAYER_BACKGROUND_STARSCAPE_1:
        index = 0;
        backgroundSpriteList_Layer1 = (BackgroundSprite_SpriteData *)malloc(_SPRITELIBRARY_BACKGROUNDLAYER1_SPRITELIST_LENGTH * sizeof(BackgroundSprite_SpriteData));

        // artwork 1
        backgroundSpriteList_Layer1[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer1_spritelist[index], BG_SPRITETYPE_ARTWORK, LAYER_BACKGROUND_STARSCAPE_1}; index++;
        // small 2
        backgroundSpriteList_Layer1[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer1_spritelist[index], BG_SPRITETYPE_TINTABLE_SMALL, LAYER_BACKGROUND_STARSCAPE_1}; index++;
        backgroundSpriteList_Layer1[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer1_spritelist[index], BG_SPRITETYPE_TINTABLE_SMALL, LAYER_BACKGROUND_STARSCAPE_1}; index++;
        // medium 1
        backgroundSpriteList_Layer1[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer1_spritelist[index], BG_SPRITETYPE_TINTABLE_MEDIUM, LAYER_BACKGROUND_STARSCAPE_1}; index++;
        // large 1
        backgroundSpriteList_Layer1[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer1_spritelist[index], BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_1}; index++;
        
        break;
    case LAYER_BACKGROUND_STARSCAPE_2:
        index = 0;

        backgroundSpriteList_Layer2 = (BackgroundSprite_SpriteData *)malloc(_SPRITELIBRARY_BACKGROUNDLAYER2_SPRITELIST_LENGTH * sizeof(BackgroundSprite_SpriteData));

        // small 3
        backgroundSpriteList_Layer2[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer2_spritelist[index], BG_SPRITETYPE_TINTABLE_SMALL, LAYER_BACKGROUND_STARSCAPE_2}; index++;
        backgroundSpriteList_Layer2[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer2_spritelist[index], BG_SPRITETYPE_TINTABLE_SMALL, LAYER_BACKGROUND_STARSCAPE_2}; index++;
        backgroundSpriteList_Layer2[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer2_spritelist[index], BG_SPRITETYPE_TINTABLE_SMALL, LAYER_BACKGROUND_STARSCAPE_2}; index++;
        // large 6
        backgroundSpriteList_Layer2[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer2_spritelist[index], BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_2}; index++;
        backgroundSpriteList_Layer2[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer2_spritelist[index], BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_2}; index++;
        backgroundSpriteList_Layer2[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer2_spritelist[index], BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_2}; index++;
        backgroundSpriteList_Layer2[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer2_spritelist[index], BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_2}; index++;
        backgroundSpriteList_Layer2[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer2_spritelist[index], BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_2}; index++;
        backgroundSpriteList_Layer2[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer2_spritelist[index], BG_SPRITETYPE_TINTABLE_LARGE, LAYER_BACKGROUND_STARSCAPE_2}; index++;

        break;
    case LAYER_BACKGROUND_STARSCAPE_3:
        index = 0;

        backgroundSpriteList_Layer3 = (BackgroundSprite_SpriteData *)malloc(_SPRITELIBRARY_BACKGROUNDLAYER3_SPRITELIST_LENGTH * sizeof(BackgroundSprite_SpriteData));
        backgroundSpriteList_Layer3[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer3_spritelist[index], BG_SPRITETYPE_ENVIRONMENT, LAYER_BACKGROUND_STARSCAPE_3}; index++;
        backgroundSpriteList_Layer3[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer3_spritelist[index], BG_SPRITETYPE_ENVIRONMENT, LAYER_BACKGROUND_STARSCAPE_3}; index++;
        backgroundSpriteList_Layer3[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer3_spritelist[index], BG_SPRITETYPE_ENVIRONMENT, LAYER_BACKGROUND_STARSCAPE_3}; index++;
        backgroundSpriteList_Layer3[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer3_spritelist[index], BG_SPRITETYPE_ENVIRONMENT, LAYER_BACKGROUND_STARSCAPE_3}; index++;
        backgroundSpriteList_Layer3[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer3_spritelist[index], BG_SPRITETYPE_ENVIRONMENT, LAYER_BACKGROUND_STARSCAPE_3}; index++;
        backgroundSpriteList_Layer3[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer3_spritelist[index], BG_SPRITETYPE_ENVIRONMENT, LAYER_BACKGROUND_STARSCAPE_3}; index++;
        backgroundSpriteList_Layer3[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer3_spritelist[index], BG_SPRITETYPE_ENVIRONMENT, LAYER_BACKGROUND_STARSCAPE_3}; index++;
        backgroundSpriteList_Layer3[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer3_spritelist[index], BG_SPRITETYPE_ENVIRONMENT, LAYER_BACKGROUND_STARSCAPE_3}; index++;
        backgroundSpriteList_Layer3[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer3_spritelist[index], BG_SPRITETYPE_ENVIRONMENT, LAYER_BACKGROUND_STARSCAPE_3}; index++;
        backgroundSpriteList_Layer3[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer3_spritelist[index], BG_SPRITETYPE_ENVIRONMENT, LAYER_BACKGROUND_STARSCAPE_3}; index++;
        backgroundSpriteList_Layer3[index] = (BackgroundSprite_SpriteData){ _SpriteLibrary_backgroundLayer3_spritelist[index], BG_SPRITETYPE_ENVIRONMENT, LAYER_BACKGROUND_STARSCAPE_3}; index++;

        break;
    default:
        break;
    }
}

void destroyBackgroundSprites(void *self, float DeltaTime)
{
    switch (THIS->currentLayer)
    {
    default:
    case LAYER_BACKGROUND_STARSCAPE_0:
        // LAYER 0
        free(backgroundSpriteList_Layer0);
        backgroundSpriteList_Layer0 = 0;
        break;
    case LAYER_BACKGROUND_STARSCAPE_1:
        // LAYER 1
        free(backgroundSpriteList_Layer1);
        backgroundSpriteList_Layer1 = 0;
        break;

    case LAYER_BACKGROUND_STARSCAPE_2:
        // LAYER 2
        free(backgroundSpriteList_Layer2);
        backgroundSpriteList_Layer2 = 0;
        break;
    case LAYER_BACKGROUND_STARSCAPE_3:
        // LAYER 3
        free(backgroundSpriteList_Layer3);
        backgroundSpriteList_Layer3 = 0;
        break;
    }
}

void prepareBackgroundGenerationData(void *self, float DeltaTime)
{
    // ...
    int numberOfSprites = (BACKGROUND_DATA->spawnCount);

    // printf("%s\n","making bg objects list");
    // object reference array
    BACKGROUND_DATA->backgroundGeneratedObjects = (BackgroundSprite_GenerationData *)malloc(numberOfSprites * sizeof(BackgroundSprite_GenerationData));

    // roll each object
    for (int i = 0; i < numberOfSprites; i++)
    {
        rollForBackgroundObjectData(self, DeltaTime, i);
    }
}

void destroyBackgroundGenerationData(void *self, float DeltaTime)
{
    // object reference array
    free(BACKGROUND_DATA->backgroundGeneratedObjects);
    BACKGROUND_DATA->backgroundGeneratedObjects = 0;
}

int getBackgroundSpriteCount(void *self)
{
    switch (THIS->currentLayer)
    {
    default:
    case LAYER_BACKGROUND_STARSCAPE_0:
        return _SPRITELIBRARY_BACKGROUNDLAYER0_SPRITELIST_LENGTH;
    case LAYER_BACKGROUND_STARSCAPE_1:
        return _SPRITELIBRARY_BACKGROUNDLAYER1_SPRITELIST_LENGTH;
    case LAYER_BACKGROUND_STARSCAPE_2:
        return _SPRITELIBRARY_BACKGROUNDLAYER2_SPRITELIST_LENGTH;
    case LAYER_BACKGROUND_STARSCAPE_3:
        return _SPRITELIBRARY_BACKGROUNDLAYER3_SPRITELIST_LENGTH;
    }
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
    generatedType = spriteDataListTemp[selectionRoll].spriteType;

    // === tinting
    int colourCount = 5;
    switch (generatedType)
    {
    default:
    case BG_SPRITETYPE_ENVIRONMENT:
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


    // === rotation
    switch(generatedType){
        default:
        generatedRotation = FLOAT_RAND * 360.0f;
        break;
        case BG_SPRITETYPE_ENVIRONMENT:
        generatedRotation = 0.0f;
        break;
    }

    // generate two random percentage values
    float scaleRand = FLOAT_RAND;
    float aspect = (spriteDataListTemp[generatedIndex].sprite->tex.width) / (spriteDataListTemp[generatedIndex].sprite->tex.height);
    // === scaling
    switch (generatedType){
        case BG_SPRITETYPE_ENVIRONMENT:
            generatedScale = (Vector2){ENVIRONMENT_SCALE*aspect, ENVIRONMENT_SCALE/aspect*2.0f};
            break;
        default:
            // then lerp between the minimum and maximum
            generatedScale = (Vector2){
                ((1.0f - scaleRand) * BACKGROUND_SPAWN_MIN_SCALE_X + (scaleRand)*BACKGROUND_SPAWN_MAX_SCALE_X),
                ((1.0f - scaleRand) * BACKGROUND_SPAWN_MIN_SCALE_Y + (scaleRand)*BACKGROUND_SPAWN_MAX_SCALE_Y)};
            break;
    }


    // === positioning
    switch (generatedType){
        case BG_SPRITETYPE_ENVIRONMENT:
            // SetScaleFactor(1.0f);
            generatedPosition = (Vector2){
                (PLAYER_OBJECT_REF->position.x + PLAYER_OBJECT_REF->velocity.x) + (FLOAT_RAND * WORMHOLE_TRAVEL_DISTANCE) + (2*cameraBounds.x) + generatedScale.x + ENVIRONMENT_POSITION_GENERATION_BUFFER,
                (0.0f)};
            break;
        default:
            generatedPosition = (Vector2){
                ((PLAYER_OBJECT_REF->position.x + PLAYER_OBJECT_REF->velocity.x) + (FLOAT_RAND * BACKGROUND_SPAWN_MAX_DISTANCE_X*generatedScale.x) + (2*cameraBounds.x)),
                (FLOAT_RAND * BACKGROUND_SPAWN_MAX_DISTANCE_Y - BACKGROUND_SPAWN_MAX_DISTANCE_Y / 2.0f)};
            break;
    }

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
        positionOffsetX
    };
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
    case LAYER_BACKGROUND_STARSCAPE_3:
        return backgroundSpriteList_Layer3;
    default:
        printf("ERROR: background_spritefield.c currentLayer is not a background layer");
        return backgroundSpriteList_Layer0;
    }
}



#undef BACKGROUND_DATA