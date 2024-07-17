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
#include "src/SpriteLibrary.c"
#include "src/palette_lib.c"
#include "src/settings.h"

#ifndef _sprite
    #define _sprite
    #include "src/sprite.c"
#endif

#ifndef _misc_util
#define _misc_util
#include "src/misc_util.c"
#endif


#ifndef _sound_manager
#define _sound_manager
#include "src/sound_manager.c"
#endif


#include "src/base.h"
#include "src/obj_register.h"


#ifndef _asteroid
#define _asteroid
#include "src/objs/asteroid.c"
#endif


#ifndef _player
#define _player
#include "src/objs/player.c"
#endif

#ifndef _shield
#define _shield
#include "src/objs/shield.c"
#endif

#ifndef _timer
#define _timer
#include "src/timer.c"
#endif

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

#ifndef _textobj
    #define _textobj
    #include "src/objs/textobj.c"
#endif

#ifndef _wormhole_obj
#define _wormhole_obj
#include "src/objs/wormhole.c"
#endif
#ifndef _planet_body
#define _planet_body
#include "src/objs/planet_body.c"
#endif

// #ifndef _background_dustart
// #define _background_dustart
// #include "src/objs/background_dustart.c"
// #endif

#ifndef _background_starfield
#define _background_starfield
#include "src/objs/background_starfield.c"
#endif

#ifndef _background_spritefield
#define _background_spritefield
#include "src/objs/background_spritefield.c"
#endif
#ifndef _healthbar
#define _healthbar
#include "src/healthbar.c"
#endif

#ifndef _gm
    #define _gm
    #include "src/objs/gameManager.c"
#endif
#include "src/objs/titlemanager.c"
#include "src/objs/deathManager.c"
#include "src/objs/winManager.c"



static void UpdateDrawFrame(void); // Update and draw one frame

// let C know this exists
static void generateObjects();
static void prepareSounds();
static void cleanupSounds();

static int GameShouldRender(){
    return (CURRENT_GAME_SCENE_STATE == GAME_SCENE_STATE_INGAME);
}



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

    SetScaleFactor(1);

    // Initialization
    //--------------------------------------------------------------------------------------
    GameObjPoolInit();
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LIGHTSHIFT");
    InitAudioDevice(); // Initialize audio device
    //--------------------------------------------------------------------------------------

    _SpriteLibrary_LoadSprites();
    prepareSounds();
    //setAllTracksVolume(0.5f);
    scaleAllTracksVolume(0.5f);
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
    _SpriteLibrary_DestroySprites();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseAudioDevice(); // Close audio device
    CloseWindow();      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    GameObjPoolDestroy();

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    
    // grab it
    float DeltaTime = GetFrameTime();
    // check for render
    if(GameShouldRender()){
        ProcessAllUpdates(DeltaTime);
        soundUpdate();

        UpdateCamera3D();
        BeginDrawing();
        ClearBackground(BLACK);
        ProcessAllDraws(DeltaTime);
        EndDrawing();

        ProcessFreshAdd();
        ProcessAllDestroys();
    }
}

/// @brief Generates all objects for initial gamestate.
static void generateObjects()
{

    // Particle handler.
    AddToPool(CreateParticleObject());

    AddToPool(CreateTitleManager());

    // AddToPool(CreateExampleObject());
}