
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
    int asteroidCount;
    int starCount;

    float deathTimeout;
    float spawnDistance;
} _GameManager_Data;

#define DATA ((_GameManager_Data *)(THIS->data_struct))

#define MAX_ASTEROIDS 5
#define MAX_STARS 2

// Create everything needed for a scene
int _GameManager_Init(void* self, float DeltaTime) {
    DATA->deathTimeout = 5;
    // How far the player travels before it gets harder
    DATA->spawnDistance = 100;

    DATA->starCount = 0;
    DATA->asteroidCount = 1;

    PLAYER_OBJECT_REF = CreatePlayer();
    AddToPool(PLAYER_OBJECT_REF);

    AddToPool(CreateAsteroid());
    AddToPool(CreateHealthBar());

     // Wormhole Object
    WORMHOLE_OBJECT_REF = CreateWormhole();
    AddToPool(WORMHOLE_OBJECT_REF);

    // Planet Object
    PLANET_OBJECT_REF = CreatePlanet();
    AddToPool(PLANET_OBJECT_REF);
    return 0;
}

int _GameManager_Update(void* self, float DeltaTime) {
    return 0;
}

int _GameManager_Draw(void* self, float DeltaTime) {
    return 0;
}

int _GameManager_Destroy(void* self, float DeltaTime) {
    return 0;
}

GameObj_Base* CreateGameManager() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_GameManager_Init;
    obj_ptr->Update_Func = &_GameManager_Update;
    obj_ptr->Draw_Func = &_GameManager_Draw;
    obj_ptr->Destroy_Func = &_GameManager_Destroy;

    obj_ptr->data_struct = malloc(sizeof(_GameManager_Data));

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