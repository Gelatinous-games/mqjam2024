#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "./sound.h"
#include "./base.h"
#include "./settings.h"

#include "objs/player.h"

#ifndef _camera
    #define _camera
    #include "./camera.c"
#endif


#ifndef _obj_pool
    #define _obj_pool
    #include "./obj_pool.c"
#endif

#ifndef _obj_particle
    #define _obj_particle
    #include "./objs/particle.c"
#endif

#ifndef _sprite
    #define _sprite
    #include "./sprite.c"
#endif



#define HEALTHBAR_GASLEVEL_SPRITE_COUNT 10

Sprite *HEALTHBAR_FRAME_SPRITE;
Sprite *HEALTHBAR_OVERLAY_SPRITE;
Sprite **HEALTHBAR_GASLEVEL_LIST;


typedef struct {
    int currentHealthBarIndex;
} HealthBar_DataStruct;

#define HEALTHBAR_DATA ((HealthBar_DataStruct *)(THIS->data_struct))


#define CURRENT_SPRITE (HEALTHBAR_GASLEVEL_LIST[HEALTHBAR_DATA->currentHealthBarIndex])


int _HealthBar_Init(void* self, float DeltaTime) {
    // we have a reference to our own gameobject from which we can do things.
    // here we should create a reference to our datastructure and store it in the data_struct pointer.

    THIS->data_struct = malloc(sizeof(HealthBar_DataStruct));

    // If they havent yet been loaded, load all the healthbar sprites.
    if (!HEALTHBAR_GASLEVEL_LIST) {
        HEALTHBAR_FRAME_SPRITE = CreateSprite("resources/gasbar/gasbar_frame.png");
        HEALTHBAR_OVERLAY_SPRITE = CreateSprite("resources/gasbar/gasbar_overlay.png");

        HEALTHBAR_GASLEVEL_LIST = (Sprite **)malloc(sizeof(Sprite*) * HEALTHBAR_GASLEVEL_SPRITE_COUNT);

        HEALTHBAR_GASLEVEL_LIST[0] = CreateSprite("resources/gasbar/gasbar_gaslevel_01.png");
        HEALTHBAR_GASLEVEL_LIST[1] = CreateSprite("resources/gasbar/gasbar_gaslevel_02.png");
        HEALTHBAR_GASLEVEL_LIST[2] = CreateSprite("resources/gasbar/gasbar_gaslevel_03.png");
        HEALTHBAR_GASLEVEL_LIST[3] = CreateSprite("resources/gasbar/gasbar_gaslevel_04.png");
        HEALTHBAR_GASLEVEL_LIST[4] = CreateSprite("resources/gasbar/gasbar_gaslevel_05.png");
        HEALTHBAR_GASLEVEL_LIST[5] = CreateSprite("resources/gasbar/gasbar_gaslevel_06.png");
        HEALTHBAR_GASLEVEL_LIST[6] = CreateSprite("resources/gasbar/gasbar_gaslevel_07.png");
        HEALTHBAR_GASLEVEL_LIST[7] = CreateSprite("resources/gasbar/gasbar_gaslevel_08.png");
        HEALTHBAR_GASLEVEL_LIST[8] = CreateSprite("resources/gasbar/gasbar_gaslevel_09.png");
        HEALTHBAR_GASLEVEL_LIST[9] = CreateSprite("resources/gasbar/gasbar_gaslevel_10.png");
    }

    HEALTHBAR_DATA->currentHealthBarIndex = HEALTHBAR_GASLEVEL_SPRITE_COUNT-1;


    THIS->position.x = WINDOW_WIDTH/2;
    THIS->position.y = WINDOW_HEIGHT-(CURRENT_SPRITE->tex.height);

    THIS->size.x = 256.0f;
    THIS->size.y = 128.0f;

    return 0;
}

int _HealthBar_Update(void* self, float DeltaTime) {
    

    // grab the health, divide it by the numer of slides
    //  then cast to integer for picturing
    HEALTHBAR_DATA->currentHealthBarIndex = (int)((((Player_Data *)PLAYER_OBJECT_REF->data_struct)->health)/((float)(HEALTHBAR_GASLEVEL_SPRITE_COUNT-1)));

    // TOO LOW
    if(HEALTHBAR_DATA->currentHealthBarIndex <= 0){ 
        // set to 0
        HEALTHBAR_DATA->currentHealthBarIndex = -1;

        // also handle death
        PlayDeathSound();
    }
    // TOO FAR
    else if(HEALTHBAR_DATA->currentHealthBarIndex >= HEALTHBAR_GASLEVEL_SPRITE_COUNT){ 
        HEALTHBAR_DATA->currentHealthBarIndex = HEALTHBAR_GASLEVEL_SPRITE_COUNT-1;
    }

    // An example of searching for objects with neutral flag.
    for (int i = 0; i != -1; ) {
        GameObj_Base* obj;
        i = GetObjectWithFlagsAny(FLAG_NEUTRAL_OBJECT, i, &obj);

        // Check if obj is not null
        if (!obj || i == -1) break;

        // Do an operation with the result...
    }



    

    return 0;
}

int _HealthBar_Draw(void* self, float DeltaTime) {
    // ibid
    // ===========================================
    // === draw frame

    HEALTHBAR_FRAME_SPRITE->dst = (Rectangle) { THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y };
    HEALTHBAR_FRAME_SPRITE->origin = Vector2Scale(THIS->size, 0.5);

    DrawTexturePro(HEALTHBAR_FRAME_SPRITE->tex, HEALTHBAR_FRAME_SPRITE->src, HEALTHBAR_FRAME_SPRITE->dst, HEALTHBAR_FRAME_SPRITE->origin, 0, WHITE);


    // ===========================================
    // === draw level

    // RenderSpriteRelative(data->sprite, THIS->position, THIS->size, 0, WHITE);

    // not dead
    if(HEALTHBAR_DATA->currentHealthBarIndex >= 0){
        Sprite *sprite = HEALTHBAR_GASLEVEL_LIST[HEALTHBAR_DATA->currentHealthBarIndex];

        sprite->dst = (Rectangle) { THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y };
        sprite->origin = Vector2Scale(THIS->size, 0.5);

        DrawTexturePro(sprite->tex, sprite->src, sprite->dst, sprite->origin, 0, WHITE);
    }

    // ===========================================
    // === draw overlay

    HEALTHBAR_OVERLAY_SPRITE->dst = (Rectangle) { THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y };
    HEALTHBAR_OVERLAY_SPRITE->origin = Vector2Scale(THIS->size, 0.5);

    DrawTexturePro(HEALTHBAR_OVERLAY_SPRITE->tex, HEALTHBAR_OVERLAY_SPRITE->src, HEALTHBAR_OVERLAY_SPRITE->dst, HEALTHBAR_OVERLAY_SPRITE->origin, 0, WHITE);

    // ===========================================

    return 0;
}

int _HealthBar_Destroy(void* self, float DeltaTime) {
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.

    DestroySprite(HEALTHBAR_FRAME_SPRITE);
    DestroySprite(HEALTHBAR_OVERLAY_SPRITE);

    for(int i =  0; i < HEALTHBAR_GASLEVEL_SPRITE_COUNT; i++){
        DestroySprite(HEALTHBAR_GASLEVEL_LIST[i]);
    }

    free(HEALTHBAR_GASLEVEL_LIST);

    free(HEALTHBAR_DATA);

    return 0;
}

GameObj_Base* CreateHealthBar() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_HealthBar_Init;
    obj_ptr->Update_Func = &_HealthBar_Update;
    obj_ptr->Draw_Func = &_HealthBar_Draw;
    obj_ptr->Destroy_Func = &_HealthBar_Destroy;

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_BACKGROUND;

    obj_ptr->currentLayer = LAYER_GUI;

    obj_ptr->radius = 0;
    obj_ptr->mass = 0;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = Vector2One();
    return obj_ptr;
}