
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

#ifndef _player
    #define _player
    #include "player.c"
#endif

#include "../obj_register.h"
#include "../settings.h"



typedef struct {
    int asteroidCount;
    int starCount;

    float deathTimeout;
    float spawnDistance;

    char madeDeathManager;
    char madeWinManager;

    char makeTitleScreen;

    float loadSphereRadius;
    char DoLoadIn;
    char DoLoadOut;
} _GameManager_Data;

#define DATA ((_GameManager_Data *)(THIS->data_struct))

#define INITIAL_SPAWN_DIST 25
#define MAX_ASTEROIDS 7
#define MAX_STARS 2

// Create everything needed for a scene
int _GameManager_Init(void* self, float DeltaTime) {


    SoundManager_EnableGameMusic();

    playerCanControl = 1;
    PLAYED_DEATH_SOUND_BEFORE = 0;
    setTrackVolume(HIT_SOUND_ID, 1);
    ClearParticles();
    doTick = 1;
    gettimeofday(&timerStart, NULL);

    DATA->DoLoadIn = 1;
    DATA->DoLoadOut = 0;
    DATA->loadSphereRadius = 1;

    DATA->deathTimeout = 5;
    // How far the player travels before it gets harder
    DATA->spawnDistance = INITIAL_SPAWN_DIST;

    DATA->starCount = 0;
    DATA->asteroidCount = 0;

    DATA->madeDeathManager = 0;
    DATA->madeWinManager = 0;

    DATA->makeTitleScreen = 0;

    PLAYER_OBJECT_REF = CreatePlayer();
    
    AddToPool(PLAYER_OBJECT_REF);

    // Shield object
    SHIELD_OBJECT_REF = CreateShieldObject();
    AddToPool(SHIELD_OBJECT_REF);

    // AddToPool(CreateAsteroid());
    // AddToPool(CreateStarObject());
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

        BACKGROUND_OBJECT_REF = CreateBackgroundSprites(i);
        AddToPool(BACKGROUND_OBJECT_REF);
    }



    
    // BACKGROUND_OBJECT_REF = CreateBackgroundSprites();
    // AddToPool(BACKGROUND_OBJECT_REF);
    return 0;
}

int _GameManager_Update(void* self, float DeltaTime) {
    // Perform some logic to check if the scene should end - if so, prepare for deletion on myself.
    if (DATA->DoLoadIn) {
        DATA->loadSphereRadius -= DeltaTime;
        if (DATA->loadSphereRadius <= 0) {
            DATA->DoLoadIn = 0;
        }
    }

    GameObj_Base* obj;
    GetObjectWithFlagsExact(FLAG_PLAYER_OBJECT, 0, &obj);

    if (!obj) return -1; // NO PLAYER FOUND!!

    THIS->position = obj->position;

    DATA->spawnDistance -= obj->velocity.x * DeltaTime;

    if (DATA->spawnDistance < 0) {
        DATA->spawnDistance = GAME_SPAWN_DIST;
        if (!TO_WORMHOLE) {
            DATA->spawnDistance = GAME_SPAWN_DIST / 2;
        }
        int capMult = TO_WORMHOLE ? 1 : 2;
        if (FLOAT_RAND < GAME_SPAWN_ASTEROID && DATA->asteroidCount < MAX_ASTEROIDS * capMult) {
            AddToPool(CreateAsteroid());
            DATA->asteroidCount++;
        }
        else if (DATA->starCount < MAX_ASTEROIDS *capMult) {
            AddToPool(CreateStarObject());
            DATA->starCount++;
        }
    }

    Player_Data* pData = (Player_Data*) obj->data_struct;
    // player death stuff
    if (!IsPlayerAlive() && !DATA->madeDeathManager) {
        AddToPool(CreateDeathManager());
        DATA->madeDeathManager = 1;
        doTick = 0;
        setTrackVolume(STAR_PROXIMITY_LOOP_ID, 0);
    }

    if (!IsPlayerAlive()) {
        // observe for R or E commands and destroy/create appropriate scene
        if (IsKeyPressed(KEY_R)) {
            playSoundOnce(BUTTON_SOUND_ID);
            // restart, create fresh gamemanager.
            DATA->makeTitleScreen = 0;
            THIS->awaitDestroy = 1;
        }
        else if (IsKeyPressed(KEY_E)) {
            playSoundOnce(BUTTON_SOUND_ID);
            TO_WORMHOLE = 1;
            DATA->makeTitleScreen = 1;
            THIS->awaitDestroy = 1;
        }
    }
    
    Vector2 a, b;
    // if the player collides with wormhole
    if (TO_WORMHOLE && GetCollided(PLAYER_OBJECT_REF, WORMHOLE_OBJECT_REF, &a, &b)) {
        playerCanControl = 0;
        // printf("<%f, %f> to <%f, %f>\n", PLAYER_OBJECT_REF->position.x, PLAYER_OBJECT_REF->position.y, WORMHOLE_OBJECT_REF->position.x, WORMHOLE_OBJECT_REF->position.y);
        DATA->DoLoadOut = 1;
    }
    else if (!TO_WORMHOLE && GetCollided(PLAYER_OBJECT_REF, PLANET_OBJECT_REF, &a, &b)) {
        playerCanControl = 0;
        DATA->DoLoadOut = 1;
    }

    if (DATA->DoLoadOut) {
        DATA->loadSphereRadius += DeltaTime;
        if (DATA->loadSphereRadius >= 1) {
            DATA->loadSphereRadius = 1;
            if (!DATA->madeWinManager) {
                DATA->madeWinManager = 1;
                AddToPool(CreateWinManager());
            }
        }
    }
    
    if (DATA->madeWinManager) {
        if (IsKeyPressed(KEY_R)) {
            playSoundOnce(BUTTON_SOUND_ID);
            // restart, create fresh gamemanager.
            TO_WORMHOLE = 1-TO_WORMHOLE;
            DATA->makeTitleScreen = 0;
            THIS->awaitDestroy = 1;
        }
        else if (IsKeyPressed(KEY_E)) {
            playSoundOnce(BUTTON_SOUND_ID);
            TO_WORMHOLE = 1;
            DATA->makeTitleScreen = 1;
            THIS->awaitDestroy = 1;
        }
    }
    return 0;
}

int _GameManager_Draw(void* self, float DeltaTime) {
    drawTimer();

    if (DATA->DoLoadIn) {
        RenderCircleRelative(THIS->position, DATA->loadSphereRadius * cameraBounds.x * 2, BLACK);
    }
    if (DATA->DoLoadOut) {
        if (TO_WORMHOLE) {
            RenderCircleRelative(THIS->position, DATA->loadSphereRadius * cameraBounds.x * 2, DARKPURPLE);
        }
        else {
            RenderCircleRelative(THIS->position, DATA->loadSphereRadius * cameraBounds.x * 2, GRAY);
        }
    }
    return 0;
}

int _GameManager_Destroy(void* self, float DeltaTime) {
    ClearParticles();

    GameObj_Base* obj;

    // search for and delete all player, asteroid, star, wormhole, background stars, etc.
    for (int sIDX = 0; sIDX != -1; ) {
        sIDX = GetObjectWithFlagsAny(
            FLAG_ASTEROID | FLAG_PLAYER_OBJECT | FLAG_GRAVITY_WELL | FLAG_WORMHOLE | FLAG_BACKGROUND | FLAG_MANAGER | FLAG_GUI,
            sIDX,
            &obj
        );

        if (sIDX == -1 || !obj) break;

        if (obj->awaitDestroy) continue;

        obj->awaitDestroy = 1;
    }

    WORMHOLE_OBJECT_REF = 0;
    PLANET_OBJECT_REF = 0;
    STAR_OBJECT_REF = 0;
    

    free(BACKGROUNDSTARS_EFFECT_REF_LIST);

    if (DATA->makeTitleScreen) {
        AddToPool(CreateTitleManager());
    }
    else {
        AddToPool(CreateGameManager());
    }


    free(DATA);

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
    obj_ptr->flags = FLAG_GAME_MANAGER;

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