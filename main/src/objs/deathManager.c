
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "../base.h"


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
#include "../misc_util.h"



typedef struct {
    float darkRamp;
    float darkRampMult;

    float timeToCreateOptions;
    char doneCreatedOptions;
} _DeathManager_Data;

#define DATA ((_DeathManager_Data *)(THIS->data_struct))

// Create everything needed for a scene
int _DeathManager_Init(void* self, float DeltaTime) {
    // Create any objects we need for the death screen HERE.
    // They should be destroyed in our destroy function.

    SoundManager_EnableMenuMusic();
    
    GameObj_Base* obj = CreateTextObj();

    DATA->darkRamp = 0;
    DATA->darkRampMult = 0.1;

    DATA->timeToCreateOptions = 5;
    DATA->doneCreatedOptions = 0;

    AddToPool(obj);

    _TextObj_Data* textData = (_TextObj_Data*) obj->data_struct;
    textData->data = "YOU DIED";
    textData->doTextFlash = 1;
    textData->textFlash = 0.75;
    textData->doWidthSubtr = 1;

    obj->position = (Vector2) { 0, 0 };
    return 0;
}

int _DeathManager_Update(void* self, float DeltaTime) {
    // Perform some logic to determine if we should progress to the game state.
    // If we're progressing to the game state, we should flag this object to be destroyed.

    DATA->darkRamp += (DeltaTime * DATA->darkRampMult);
    if (DATA->darkRamp > 1) DATA->darkRamp = 1;

    DATA->timeToCreateOptions -= DeltaTime;

    if (DATA->timeToCreateOptions < 0 && !DATA->doneCreatedOptions) {
        DATA->doneCreatedOptions = 1;

        // create "Press [R] to respawn."
        GameObj_Base* obj = CreateTextObj();
        _TextObj_Data* textData = (_TextObj_Data*) obj->data_struct;
        AddToPool(obj);
        textData->data = "Press [R] to respawn.";
        textData->doTextFlash = 0;
        textData->textFlash = 1;
        textData->doWidthSubtr = 0;
        obj->position = (Vector2) { -cameraBounds.x+1, 1 };
        obj->size = (Vector2) { 0.5, 0.5 };

        // create "Press [E] to exit to menu."
        obj = CreateTextObj();
        textData = (_TextObj_Data*) obj->data_struct;
        AddToPool(obj);
        textData->data = "Press [E] to exit to menu.";
        textData->doTextFlash = 0;
        textData->textFlash = 1;
        textData->doWidthSubtr = 0;
        obj->position = (Vector2) { -cameraBounds.x+1, 2 };
        obj->size = (Vector2) { 0.5, 0.5 };
    }
    // THIS->awaitDestroy = 1; // Do this to move to next state
    return 0;
}

int _DeathManager_Draw(void* self, float DeltaTime) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color) { 0, 0, 0, (255 * DATA->darkRamp) } ); 
    return 0;
}

int _DeathManager_Destroy(void* self, float DeltaTime) {
    // Destroy any objects we're using for the death screen.
    free(DATA);
    THIS->data_struct = 0;
    return 0;
}

GameObj_Base* CreateDeathManager() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_DeathManager_Init;
    obj_ptr->Update_Func = &_DeathManager_Update;
    obj_ptr->Draw_Func = &_DeathManager_Draw;
    obj_ptr->Destroy_Func = &_DeathManager_Destroy;

    obj_ptr->data_struct = malloc(sizeof(_DeathManager_Data));

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_MANAGER;

    obj_ptr->currentLayer = LAYER_GUI;

    obj_ptr->radius = 0;
    obj_ptr->mass = 0;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = Vector2Zero();
    
    return obj_ptr;
}

#undef DATA