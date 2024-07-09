
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "../base.h"
#include "../sound.h"


#ifndef _camera
    #define _camera
    #include "../camera.c"
#endif

#ifndef _obj_pool
    #define _obj_pool
    #include "../obj_pool.c"
#endif

#ifndef _textobj
    #define _textobj
    #include "textobj.c"
#endif

#include "../obj_register.h"
#include "../settings.h"



typedef struct {
    Sprite* shipSprite;
    Sprite* asteroidSprite;

    float shipSpriteWobble;
    float asteroidRotate;

    char doLoadGame;
    float sphereRadius;
} _TitleManager_Data;

#define DATA ((_TitleManager_Data *)(THIS->data_struct))

// Create everything needed for a scene
int _TitleManager_Init(void* self, float DeltaTime) {
    printf("%s\n","INIT TITLE MANAGER");
    // Create any objects we need for the title screen HERE.
    // They should be destroyed in our destroy function.


    SoundManager_EnableMenuMusic();


    DATA->shipSprite = CreateSprite(SPACESHIP_SPRITE_PATH);
    cameraPosition = Vector2Zero();
    THIS->size = (Vector2){2, 2};

    DATA->shipSpriteWobble = 0;

    DATA->asteroidSprite = CreateSprite("resources/asteroids/A3.png");

    DATA->doLoadGame = 0;
    DATA->sphereRadius = 0;

    GameObj_Base* obj = CreateTextObj();
    _TextObj_Data* textData = (_TextObj_Data*) obj->data_struct;

    textData->data = "LIGHTSHIFT";
    textData->doTextFlash = 0;
    textData->textFlash = 0;
    textData->doWidthSubtr = 1;

    obj->position = (Vector2) { 0, -2 };
    obj->size = (Vector2) { 1, 1 };

    obj->currentLayer = LAYER_GUI;

    AddToPool(obj);

    obj = CreateTextObj();
    textData = (_TextObj_Data*) obj->data_struct;

    textData->data = "Press space to start";
    textData->doTextFlash = 0;
    textData->textFlash = 0;
    textData->doWidthSubtr = 1;

    obj->position = (Vector2) { 0, 2 };
    obj->size = (Vector2) { 0.5, 0.5 };

    obj->currentLayer = LAYER_GUI;

    AddToPool(obj);

    setTrackVolume(STAR_PROXIMITY_LOOP_ID, 0);


    printf("%s\n","DONE INIT TITLE MANAGER");

    return 0;
}

int _TitleManager_Update(void* self, float DeltaTime) {
    // Perform some logic to determine if we should progress to the game state.
    // If we're progressing to the game state, we should flag this object to be destroyed.

    //THIS->awaitDestroy = 1; // Do this to move to next state

    DATA->shipSpriteWobble += DeltaTime / 4;

    DATA->asteroidRotate += DeltaTime;

    THIS->position.y = sin(DATA->shipSpriteWobble)*0.5;
    THIS->position.x = cos(DATA->shipSpriteWobble)*0.5;

    SpawnParticle(
            Vector2Add(THIS->position, Vector2Multiply((Vector2) {-0.5, -0.175}, THIS->size)),
            (Vector2) { (FLOAT_RAND * -4) - 4, FLOAT_RAND - 0.5 },
            Vector2Zero(),
            (Vector2) {0.125, 0.125},
            0.5,
            (Color) {255, 127, 0, 127 },
            (FLOAT_RAND * 1) + 0.5
        );

    SpawnParticle(
            Vector2Add(THIS->position, Vector2Multiply((Vector2) {-0.5, 0.225}, THIS->size)),
            (Vector2) { (FLOAT_RAND * -4) - 4, FLOAT_RAND - 0.5 },
            Vector2Zero(),
            (Vector2) {0.125, 0.125},
            0.5,
            (Color) {255, 127, 0, 127 },
            (FLOAT_RAND * 1) + 0.5
        );


    if (!DATA->doLoadGame) {
        if (IsKeyPressed(KEY_SPACE)) {
            playSoundOnce(BUTTON_SOUND_ID);
            DATA->doLoadGame = 1;
        }
    }
    else {
        DATA->sphereRadius += DeltaTime * 1.5;
        if (DATA->sphereRadius >= 1) {
            THIS->awaitDestroy = 1;
        }
    }
    return 0;
}

int _TitleManager_Draw(void* self, float DeltaTime) {
    RenderSpriteRelative(DATA->shipSprite, THIS->position, THIS->size, 0, WHITE);

    Vector2 siz = (Vector2) { 8, 8 };
    Vector2 pos = (Vector2) { 0, cameraBounds.y + 2};
    RenderSpriteRelative(DATA->asteroidSprite, pos, siz, DATA->asteroidRotate, WHITE);

    if (DATA->doLoadGame) {
        RenderCircleAbsolute(Vector2Zero(), DATA->sphereRadius * cameraBounds.x * 1.1, (Color) { 0, 0, 0, 255});
    }

    return 0;
}

int _TitleManager_Destroy(void* self, float DeltaTime) {
    printf("%s\n","KILLING TITLE MANAGER");
    // Destroy any objects we're using for the title screen.

    ClearParticles();
    GameObj_Base* obj;
    for (int sIDX = 0; sIDX != -1; ) {
        sIDX = GetObjectWithFlagsAny(
            FLAG_GUI,
            sIDX,
            &obj
        );

        if (sIDX == -1 || !obj) break;

        if (obj->awaitDestroy) continue;

        obj->awaitDestroy = 1;
    }

    DestroySprite(DATA->shipSprite);
    DATA->shipSprite = 0;
    DestroySprite(DATA->asteroidSprite);
    DATA->asteroidSprite = 0;
    free(DATA);
    THIS->data_struct = 0;

    TO_WORMHOLE = 1;
    AddToPool(CreateGameManager());
    printf("%s\n","DESTROYED TITLE MANAGER");
    return 0;
}

GameObj_Base* CreateTitleManager() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_TitleManager_Init;
    obj_ptr->Update_Func = &_TitleManager_Update;
    obj_ptr->Draw_Func = &_TitleManager_Draw;
    obj_ptr->Destroy_Func = &_TitleManager_Destroy;

    obj_ptr->data_struct = malloc(sizeof(_TitleManager_Data));

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_MANAGER;

    obj_ptr->currentLayer = LAYER_GUI_2;

    obj_ptr->radius = 0;
    obj_ptr->mass = 0;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = Vector2Zero();
    
    return obj_ptr;
}

#undef DATA