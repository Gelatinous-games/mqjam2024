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

typedef struct {
    float a;
} Player_DataContainer;

#define PLAYERDATA ((Player_DataContainer *)(THIS->data_struct))


int Player_Init(void* self, float DeltaTime) {
    // we have a reference to our own gameobject from which we can do things.
    
    return 0;
}

int Player_Update(void* self, float DeltaTime) {
    // see above


    // use data here.

    PLAYERDATA->a += DeltaTime;

    THIS->position.x += THIS->velocity.x;
    THIS->position.y += THIS->velocity.y;

    for (int i = 0; i != -1; ) {
        GameObj_Base* obj;
        i = GetObjectWithFlagsAny(FLAG_NEUTRAL_OBJECT, i, &obj);

        // Check if obj is not null
        if (!obj || i == -1) break;

        // Do an operation with the result...
    }

    return 0;
}

int Player_Draw(void* self, float DeltaTime) {
    // ibid
    // Vector2 screenPosition = GetScreenspacePositionRelative(THIS->position, THIS->size);


    // DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color);
    DrawEllipse(THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y, WHITE);




    return 0;
}

int Player_Destroy(void* self, float DeltaTime) {
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.

    // free our data struct here. free anything contained.
    free(THIS->data_struct);

    return 0;
}

GameObj_Base* CreatePlayer(int windowWidth, int windowHeight) {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    // ============================================================
    // ==== setup the data scruct data
    obj_ptr->data_struct = malloc(sizeof(Player_DataContainer)); 

    // ============================================================
    // ==== assign the ufnctions
    obj_ptr->Init_Func = &Player_Init;
    obj_ptr->Update_Func = &Player_Update;
    obj_ptr->Draw_Func = &Player_Draw;
    obj_ptr->Destroy_Func = &Player_Destroy;
    // ============================================================
    // huh
    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_PLAYER_OBJECT;

    // FIXME
    obj_ptr->currentLayer = LAYER_GUI;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size.x = 50;
    obj_ptr->size.y = 50;


    // use this stuffs
    obj_ptr->position.x = 25;
    obj_ptr->position.y = (windowHeight)/2.0f;


    return obj_ptr;
}
