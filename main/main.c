/*******************************************************************************************
 *
 *   raylib [core] example - Basic 3d example
 *
 *   Welcome to raylib!
 *
 *   To compile example, just press F5.
 *   Note that compiled executable is placed in the same folder as .c file
 *
 *   You can find all basic examples on C:\raylib\raylib\examples folder or
 *   raylib official webpage: www.raylib.com
 *
 *   Enjoy using raylib. :)
 *
 *   This example has been created using raylib 1.0 (www.raylib.com)
 *   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
 *
 *   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

// what da henk
#define invoken(a, b) a->b(a->self)
#define invoke(a, b, ...) a->b(a->self, __VA_ARGS__)

// standard library includes
#include <stdio.h>
#include <stdlib.h>

// library includes
#include "raylib.h"
#include "raymath.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

// project header includes
#include "src/settings.h"
#include "src/base.h"
#include "src/sound.h"
#include "src/obj_register.h"
#include "src/objs/asteroid.h"
#include "src/objs/player.h"

#include "src/timer.c"

#ifndef _obj_example
#define _obj_example
#include "src/objs/Object_Example.c"
#endif

#ifndef _obj_particle
#define _obj_particle
#include "src/objs/particle.c"
#endif

#ifndef _obj_pool
#define _obj_pool
#include "src/obj_pool.c"
#endif

#ifndef _star_obj
    #define _star_obj
    #include "src/objs/star.c"
#endif


#ifndef _wormhole_obj
#define _wormhole_obj
#include "src/objs/wormhole.c"
#endif
#ifndef _planet_obj
#define _planet_obj
#include "src/objs/planet.c"
#endif

#ifndef _background
#define _background
#include "src/objs/background.c"
#endif
#ifndef _healthbar
#define _healthbar
#include "src/healthbar.c"
#endif

static void UpdateDrawFrame(void); // Update and draw one frame

// let C know this exists
static void generateObjects();
static void prepareSounds();
static void cleanupSounds();

int main()
{
    gettimeofday(&timerStart, NULL);

    SetExitKey(KEY_F4); // Lets not make it *too* easy to leave lol

    cameraScreenQuarter.x = WINDOW_WIDTH / 2;
    cameraScreenQuarter.y = WINDOW_HEIGHT / 2;

    cameraBounds.x = 8;
    cameraBounds.y = 4.5;
    cameraVelocity = Vector2Zero();
    cameraUnitSize = Vector2Divide(cameraScreenQuarter, cameraBounds);
    layerCount = 10;

    // Initialization
    //--------------------------------------------------------------------------------------
    GameObjPoolInit();
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "mqjam2024");
    InitAudioDevice();      // Initialize audio device
    //--------------------------------------------------------------------------------------

    prepareSounds();
    setAllTracksVolume(0.5f);
    startSounds();

    generateObjects();

    ProcessFreshAdd();

#ifndef PLATFORM_WEB
    SetTargetFPS(FRAMERATE);     // Set our game to run at 60 frames-per-second
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#else
    // printf("%s\n",">> main() :: emscripten");
    emscripten_set_main_loop(UpdateDrawFrame, FRAMERATE, 1);
#endif

    cleanupSounds();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();     // Close audio device
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    GameObjPoolDestroy();

    free(ASTEROID_REF_LIST);

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    float DeltaTime = GetFrameTime();
    ProcessAllUpdates(DeltaTime);
    soundUpdate();

    UpdateCamera3D();
    BeginDrawing();
    ClearBackground(BLACK);
    ProcessAllDraws(DeltaTime);
    drawTimer();
    EndDrawing();

    ProcessFreshAdd();
    ProcessAllDestroys();
}

/// @brief Generates all objects for initial gamestate.
static void generateObjects()
{

    // Particle handler.
    AddToPool(CreateParticleObject());

    // Player object.
    PLAYER_OBJECT_REF = CreatePlayer();
    AddToPool(PLAYER_OBJECT_REF);

    // Asteroid object.
    // TODO: succeed with an asteroid handler that can scale up & down asteroids.
    // Decide upon some criteria for how asteroid handler should work - should it control the scene?
    // Place stars and other elements down?
    ASTEROID_REF_LIST = (GameObj_Base **)malloc(sizeof(GameObj_Base *)*NUMBER_OF_ASTEROIDS);
    for (int i = 0; i < NUMBER_OF_ASTEROIDS; i++)
    {
        /* code */
        ASTEROID_REF_LIST[i] = CreateAsteroid();
        AddToPool(ASTEROID_REF_LIST[i]);
    }
    
    //Background Object
    BACKGROUND_OBJECT_REF = CreateBackgroundStars();
    AddToPool(BACKGROUND_OBJECT_REF);


    AddToPool(CreateHealthBar());

    //Wormhole Object
    WORMHOLE_OBJECT_REF = CreateWormhole();
    AddToPool(WORMHOLE_OBJECT_REF);
    
    //Planet Object
    PLANET_OBJECT_REF = CreatePlanet();
    AddToPool(PLANET_OBJECT_REF);

    //Star Object
    STAR_OBJECT_REF = CreateStarObject();
    AddToPool(STAR_OBJECT_REF);
}