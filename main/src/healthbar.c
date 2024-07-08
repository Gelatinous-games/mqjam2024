#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "./sound.h"
#include "./base.h"
#include "./settings.h"

#include "objs/player.h"
#include "objs/shield.h"

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
#define HEALTHBAR_SHIELDLEVEL_SPRITE_COUNT 10

Sprite *HEALTHBAR_FRAME_SPRITE;
Sprite *HEALTHBAR_OVERLAY_SPRITE;
Sprite **HEALTHBAR_GASLEVEL_LIST;
Sprite **HEALTHBAR_SHIELDLEVEL_LIST;


typedef struct {
    int currentHealthBarIndex;
    int currentShieldBarIndex;
} HealthBar_DataStruct;

#define HEALTHBAR_DATA ((HealthBar_DataStruct *)(THIS->data_struct))


#define CURRENT_HEALTH_SPRITE (HEALTHBAR_GASLEVEL_LIST[HEALTHBAR_DATA->currentHealthBarIndex])
#define CURRENT_SHIELD_SPRITE (HEALTHBAR_GASLEVEL_LIST[HEALTHBAR_DATA->currentShieldBarIndex])


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

        HEALTHBAR_SHIELDLEVEL_LIST = (Sprite **)malloc(sizeof(Sprite*) * HEALTHBAR_SHIELDLEVEL_SPRITE_COUNT);

        HEALTHBAR_SHIELDLEVEL_LIST[0] = CreateSprite("resources/gasbar/gasbar_shieldlevel_01.png");
        HEALTHBAR_SHIELDLEVEL_LIST[1] = CreateSprite("resources/gasbar/gasbar_shieldlevel_02.png");
        HEALTHBAR_SHIELDLEVEL_LIST[2] = CreateSprite("resources/gasbar/gasbar_shieldlevel_03.png");
        HEALTHBAR_SHIELDLEVEL_LIST[3] = CreateSprite("resources/gasbar/gasbar_shieldlevel_04.png");
        HEALTHBAR_SHIELDLEVEL_LIST[4] = CreateSprite("resources/gasbar/gasbar_shieldlevel_05.png");
        HEALTHBAR_SHIELDLEVEL_LIST[5] = CreateSprite("resources/gasbar/gasbar_shieldlevel_06.png");
        HEALTHBAR_SHIELDLEVEL_LIST[6] = CreateSprite("resources/gasbar/gasbar_shieldlevel_07.png");
        HEALTHBAR_SHIELDLEVEL_LIST[7] = CreateSprite("resources/gasbar/gasbar_shieldlevel_08.png");
        HEALTHBAR_SHIELDLEVEL_LIST[8] = CreateSprite("resources/gasbar/gasbar_shieldlevel_09.png");
        HEALTHBAR_SHIELDLEVEL_LIST[9] = CreateSprite("resources/gasbar/gasbar_shieldlevel_10.png");
    }

    HEALTHBAR_DATA->currentHealthBarIndex = HEALTHBAR_GASLEVEL_SPRITE_COUNT-1;

    THIS->size.x = HEALTHBAR_FRAME_SPRITE->tex.width;
    THIS->size.y = HEALTHBAR_FRAME_SPRITE->tex.height;

    THIS->position.x = WINDOW_WIDTH/2;
    THIS->position.y = WINDOW_HEIGHT-(THIS->size.y);


    return 0;
}

int _HealthBar_Update(void* self, float DeltaTime) {
    


    // TOO LOW
    if(GetPlayerHullPercentage() <= 0.0f){ 
        // dont draw
        HEALTHBAR_DATA->currentHealthBarIndex = -1;
    }
    // TOO FAR
    else if(GetPlayerHullPercentage() >= 1.0f){ 
        HEALTHBAR_DATA->currentHealthBarIndex = HEALTHBAR_GASLEVEL_SPRITE_COUNT-1;
    }
    else {
        HEALTHBAR_DATA->currentHealthBarIndex = (int)(GetPlayerHullPercentage() * (float)(HEALTHBAR_GASLEVEL_SPRITE_COUNT));
    }

    if(_ShieldObject_GetPlayerShieldPercentage() <= 0.0f){
        // dont draw
        HEALTHBAR_DATA->currentShieldBarIndex = -1;
    }
    else if(_ShieldObject_GetPlayerShieldPercentage() >= 1.0f){ 
        HEALTHBAR_DATA->currentShieldBarIndex = HEALTHBAR_SHIELDLEVEL_SPRITE_COUNT-1;
    }
    else {
        // ...
        HEALTHBAR_DATA->currentShieldBarIndex = (int)(_ShieldObject_GetPlayerShieldPercentage() * (float)(HEALTHBAR_SHIELDLEVEL_SPRITE_COUNT));
    }
    




    

    return 0;
}

int _HealthBar_Draw(void* self, float DeltaTime) {
    // ibid
    // ===========================================
    // === draw frame

    HEALTHBAR_FRAME_SPRITE->dst = (Rectangle) { THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y };
    HEALTHBAR_FRAME_SPRITE->origin = Vector2Scale(THIS->size, 0.5);

    // frame tinter
    Color frameTint = ALIVE_HEALTHBAR_FRAME_TINT;
    if(CURRENT_PLAYER_LIFE_STATE == PLAYER_LIFE_STATUS_ISDEAD) frameTint = DEATH_HEALTHBAR_FRAME_TINT;

    DrawTexturePro(HEALTHBAR_FRAME_SPRITE->tex, HEALTHBAR_FRAME_SPRITE->src, HEALTHBAR_FRAME_SPRITE->dst, HEALTHBAR_FRAME_SPRITE->origin, 0, frameTint);


    // ===========================================
    // === draw level

    // not dead
    if(HEALTHBAR_DATA->currentHealthBarIndex >= 0){
        // grab the sprite
        Sprite *healthLevelSprite = HEALTHBAR_GASLEVEL_LIST[HEALTHBAR_DATA->currentHealthBarIndex];

        healthLevelSprite->dst = (Rectangle) { THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y };
        healthLevelSprite->origin = Vector2Scale(THIS->size, 0.5);

        DrawTexturePro(healthLevelSprite->tex, healthLevelSprite->src, healthLevelSprite->dst, healthLevelSprite->origin, 0, WHITE);
    }
    // has shields
    if(HEALTHBAR_DATA->currentShieldBarIndex >= 0){
        // deal with shield sprite

        // grab the sprite
        Sprite *shieldLevelSprite = HEALTHBAR_SHIELDLEVEL_LIST[HEALTHBAR_DATA->currentShieldBarIndex];

        shieldLevelSprite->dst = (Rectangle) { THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y };
        shieldLevelSprite->origin = Vector2Scale(THIS->size, 0.5);

        DrawTexturePro(shieldLevelSprite->tex, shieldLevelSprite->src, shieldLevelSprite->dst, shieldLevelSprite->origin, 0, (Color){255,255,255,127});
    }

    // ===========================================
    // === draw overlay

    HEALTHBAR_OVERLAY_SPRITE->dst = (Rectangle) { THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y };
    HEALTHBAR_OVERLAY_SPRITE->origin = Vector2Scale(THIS->size, 0.5);

    // overlay tintind
    Color overlayTint = WHITE;
    if(CURRENT_PLAYER_LIFE_STATE == PLAYER_LIFE_STATUS_ISDEAD) overlayTint = DEATH_HEALTHBAR_OVERLAY_TINT;

    // draw it
    DrawTexturePro(HEALTHBAR_OVERLAY_SPRITE->tex, HEALTHBAR_OVERLAY_SPRITE->src, HEALTHBAR_OVERLAY_SPRITE->dst, HEALTHBAR_OVERLAY_SPRITE->origin, 0, overlayTint);

    // ===========================================

    return 0;
}

int _HealthBar_Destroy(void* self, float DeltaTime) {
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.

    DestroySprite(HEALTHBAR_FRAME_SPRITE);
    HEALTHBAR_FRAME_SPRITE = 0;
    DestroySprite(HEALTHBAR_OVERLAY_SPRITE);
    HEALTHBAR_OVERLAY_SPRITE = 0;

    for(int i =  0; i < HEALTHBAR_GASLEVEL_SPRITE_COUNT; i++){
        DestroySprite(HEALTHBAR_GASLEVEL_LIST[i]);
        HEALTHBAR_GASLEVEL_LIST[i] = 0;
    }

    free(HEALTHBAR_GASLEVEL_LIST);
    HEALTHBAR_GASLEVEL_LIST = 0;

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