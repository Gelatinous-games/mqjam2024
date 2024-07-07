
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

#ifndef _gm
    #define _gm
    #include "gameManager.c"
#endif

#include "../obj_register.h"
#include "../settings.h"



typedef struct {
    float darkRamp;
    float darkRampMult;

    float timeToCreateOptions;
    char doneCreatedOptions;
} _WinManager_Data;

#define DATA ((_WinManager_Data *)(THIS->data_struct))

// Create everything needed for a scene
int _WinManager_Init(void* self, float DeltaTime) {
    // Create any objects we need for the win screen HERE.
    // They should be destroyed in our destroy function.

    DATA->darkRamp = 0;
    DATA->darkRampMult = 0.4;

    DATA->timeToCreateOptions = 2.5;
    DATA->doneCreatedOptions = 0;

    GameObj_Base* obj = CreateTextObj();
    AddToPool(obj);

    _TextObj_Data* textData = (_TextObj_Data*) obj->data_struct;
    if (TO_WORMHOLE)
        textData->data = "You made it...";
    else
        textData->data = "You made it!";
    textData->doTextFlash = 0;
    textData->textFlash = 0;
    textData->doWidthSubtr = 1;

    obj->position = (Vector2) { 0, 0 };
    return 0;
}

int _WinManager_Update(void* self, float DeltaTime) {
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
        if (TO_WORMHOLE)
            textData->data = "Press [R] to re-enter the galaxy...";
        else
            textData->data = "Press [R] to do it again!";
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

int _WinManager_Draw(void* self, float DeltaTime) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color) { 0, 0, 0, (255 * DATA->darkRamp) } ); 
    return 0;
}

int _WinManager_Destroy(void* self, float DeltaTime) {
    // Destroy any objects we're using for the death screen.
    free(DATA);
    return 0;
}

GameObj_Base* CreateWinManager() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_WinManager_Init;
    obj_ptr->Update_Func = &_WinManager_Update;
    obj_ptr->Draw_Func = &_WinManager_Draw;
    obj_ptr->Destroy_Func = &_WinManager_Destroy;

    obj_ptr->data_struct = malloc(sizeof(_WinManager_Data));

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