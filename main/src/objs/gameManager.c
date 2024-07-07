
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

// #define DEBUG_SPAMMER_PRINTF_PREFIX if(true) 

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
    DATA->spawnDistance = GAME_SPAWN_DIST;

    DATA->starCount = 2;
    DATA->asteroidCount = 1;

    PLAYER_OBJECT_REF = CreatePlayer();
    AddToPool(PLAYER_OBJECT_REF);

    // Shield object
    SHIELD_OBJECT_REF = CreateShieldObject();
    AddToPool(SHIELD_OBJECT_REF);

    AddToPool(CreateAsteroid());
    // AddToPool(CreateStarObject());
    AddToPool(CreateHealthBar());

     // Wormhole Object
    WORMHOLE_OBJECT_REF = CreateWormhole();
    AddToPool(WORMHOLE_OBJECT_REF);

    // Planet Object
    PLANET_OBJECT_REF = CreatePlanet();
    AddToPool(PLANET_OBJECT_REF);

    // DEBUG_SPAMMER_PRINTF_PREFIX printf("number of starfield layers: %d\n", NUMBER_OF_BACKGROUNDSTARFIELD_LAYERS);
     // Background Object
    BACKGROUNDSTARFIELD_EFFECT_REF_LIST = (GameObj_Base **)malloc(sizeof(GameObj_Base *) * NUMBER_OF_BACKGROUNDSTARFIELD_LAYERS);
    BACKGROUNDSPRITE_OBJECT_REF_LIST = (GameObj_Base **)malloc(sizeof(GameObj_Base *) * NUMBER_OF_BACKGROUNDSTARFIELD_LAYERS);
    for (int i = 0; i < NUMBER_OF_BACKGROUNDSTARFIELD_LAYERS; i++)
    {
        /* code */
        // Background Stars 3 layers
        // DEBUG_SPAMMER_PRINTF_PREFIX printf("creating BACKGROUNDSTARFIELD_EFFECT_REF_LIST[%d] on layer %d\n", i, 1+i);
        BACKGROUNDSTARFIELD_EFFECT_REF_LIST[i] = CreateBackgroundStars(i, 1+i);
        // DEBUG_SPAMMER_PRINTF_PREFIX printf("adding BACKGROUNDSTARFIELD_EFFECT_REF_LIST[%d] to pool\n", i);
        AddToPool(BACKGROUNDSTARFIELD_EFFECT_REF_LIST[i]);

        // DEBUG_SPAMMER_PRINTF_PREFIX printf("creating BACKGROUNDSPRITE_OBJECT_REF_LIST[%d] on layer %d\n", i, 1+i);
        // Background sprites 3 layers, Small/Medium/Large
        BACKGROUNDSPRITE_OBJECT_REF_LIST[i] = CreateBackgroundSprites(i);
        // DEBUG_SPAMMER_PRINTF_PREFIX printf("adding BACKGROUNDSPRITE_OBJECT_REF_LIST[%d] to pool\n", i);
        AddToPool(BACKGROUNDSPRITE_OBJECT_REF_LIST[i]);
    }
    return 0;
}

int _GameManager_Update(void* self, float DeltaTime) {
    // Perform some logic to check if the scene should end - if so, prepare for deletion on myself.

    GameObj_Base* obj;
    GetObjectWithFlagsExact(FLAG_PLAYER_OBJECT, 0, &obj);

    if (!obj) return -1; // NO PLAYER FOUND!!

    DATA->spawnDistance -= obj->velocity.x * DeltaTime;

    if (DATA->spawnDistance < 0) {
        DATA->spawnDistance = GAME_SPAWN_DIST;
        if (FLOAT_RAND < GAME_SPAWN_ASTEROID && DATA->asteroidCount < MAX_ASTEROIDS) {
            // DEBUG_SPAMMER_PRINTF_PREFIX printf("SPAWNING ASTEROID!\n");
            AddToPool(CreateAsteroid());
            DATA->asteroidCount++;
        }
        else if (DATA->starCount < MAX_ASTEROIDS) {
            // DEBUG_SPAMMER_PRINTF_PREFIX printf("SPAWNING STAR!\n");
            AddToPool(CreateStarObject());
            DATA->starCount++;
        }
    }
    return 0;
}

int _GameManager_Draw(void* self, float DeltaTime) {
    return 0;
}

int _GameManager_Destroy(void* self, float DeltaTime) {
    GameObj_Base* obj;

    // search for and delete all player, asteroid, star, wormhole, background stars, etc.
    for (int sIDX = 0; sIDX != -1; ) {
        sIDX = GetObjectWithFlagsAny(
            FLAG_ASTEROID | FLAG_PLAYER_OBJECT | FLAG_GRAVITY_WELL | FLAG_WORMHOLE | FLAG_BACKGROUND,
            sIDX,
            &obj
        );

        if (sIDX == -1 || !obj) break;

        obj->awaitDestroy = 1;
    }

    free(BACKGROUNDSTARFIELD_EFFECT_REF_LIST);
    free(BACKGROUNDSPRITE_OBJECT_REF_LIST);
    free(DATA);

    // ONLY ONCE DELETED can we add an instance of the manager for the next scene!
    AddToPool(CreateDeathManager());
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

// #undef DEBUG_SPAMMER_PRINTF_PREFIX