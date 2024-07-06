
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

#include "../obj_register.h"
#include "../settings.h"



typedef struct {
    int temp;
} _TitleManager_Data;

#define DATA ((_TitleManager_Data *)(THIS->data_struct))

// Create everything needed for a scene
int _TitleManager_Init(void* self, float DeltaTime) {
    // Create any objects we need for the title screen HERE.
    // They should be destroyed in our destroy function.
    return 0;
}

int _TitleManager_Update(void* self, float DeltaTime) {
    // Perform some logic to determine if we should progress to the game state.
    // If we're progressing to the game state, we should flag this object to be destroyed.

    THIS->awaitDestroy = 1; // Do this to move to next state
    return 0;
}

int _TitleManager_Draw(void* self, float DeltaTime) {
    return 0;
}

int _TitleManager_Destroy(void* self, float DeltaTime) {
    // Destroy any objects we're using for the title screen.

    free(DATA);
    AddToPool(CreateGameManager());
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