
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

    DATA->starCount = 1;
    DATA->asteroidCount = 1;

    PLAYER_OBJECT_REF = CreatePlayer();
    AddToPool(PLAYER_OBJECT_REF);

    // Shield object
    SHIELD_OBJECT_REF = CreateShieldObject();
    AddToPool(SHIELD_OBJECT_REF);

    AddToPool(CreateAsteroid());
    AddToPool(CreateStarObject());
    AddToPool(CreateHealthBar());

     // Wormhole Object
    WORMHOLE_OBJECT_REF = CreateWormhole();
    AddToPool(WORMHOLE_OBJECT_REF);

    // Planet Object
    PLANET_OBJECT_REF = CreatePlanet();
    AddToPool(PLANET_OBJECT_REF);

    // Background Object
    //THIS DOES NOT LAYER THE BACKGROUND. IDK HOW TO FIX. I THINK IT IS TO DO WITH MY CODE.
    BACKGROUNDSTARS_EFFECT_REF_LIST = (GameObj_Base **)malloc(sizeof(GameObj_Base *) * NUMBER_OF_BACKGROUNDSTARS_LAYERS);
    for (int i = 0; i < NUMBER_OF_BACKGROUNDSTARS_LAYERS; i++)
    {
        /* code */
        BACKGROUNDSTARS_EFFECT_REF_LIST[i] = CreateBackgroundStars(i, 1+i);
        AddToPool(BACKGROUNDSTARS_EFFECT_REF_LIST[i]);
    }
    
    BACKGROUND_OBJECT_REF = CreateBackgroundSprites();
    AddToPool(BACKGROUND_OBJECT_REF);
    return 0;
}

int _GameManager_Update(void* self, float DeltaTime) {
    // Perform some logic to check if the scene should end - if so, prepare for deletion on myself.
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

    free(BACKGROUNDSTARS_EFFECT_REF_LIST);
    free(DATA);

    // ONLY ONCE DELETED can we add an instance of the manager for the next scene!
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