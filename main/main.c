/**
 *
 *
 *      DA BIG BAD MAIN FILE
 *
 *          ONLY BADDIES USE THIS FILE FOR MAJOR CODING
 *
 *         why are you still reading this
 *
 *
 *
 *
 *
 */

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
#include "src/sound.h"

#include "src/base.h"
#include "src/obj_register.h"
#include "src/objs/asteroid.h"

#ifndef _mainmenu
#define _mainmenu
#include "src/mainmenu.c"
#endif
#ifndef _deathmenu
#define _deathmenu
#include "src/deathmenu.c"
#endif

#ifndef _player
#define _player
#include "src/objs/player.c"
#endif

#ifndef _shield
#define _shield
#include "src/objs/shield.c"
#endif

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

#ifndef _background_stars
#define _background_star
#include "src/objs/background_stars.c"
#endif
#ifndef _background
#define _background
#include "src/objs/background.c"
#endif
#ifndef _healthbar
#define _healthbar
#include "src/healthbar.c"
#endif

#include "src/objs/gameManager.c"
#include "src/objs/titlemanager.c"
#include "src/objs/deathManager.c"

static void UpdateDrawFrame(void); // Update and draw one frame

// let C know this exists
static void generateObjects();
static void prepareSounds();
static void cleanupSounds();
void HandleDebuggingKillPlayerCheck(float DeltaTime);

static int GameShouldRender()
{
    return (CURRENT_GAME_SCENE_STATE != GAME_SCENE_STATE_MAINMENU);
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

    // Initialization
    //--------------------------------------------------------------------------------------
    GameObjPoolInit();
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "mqjam2024");
    InitAudioDevice(); // Initialize audio device
    //--------------------------------------------------------------------------------------

    _MainMenu_Init();
    _DeathMenu_Init();

    generateObjects();

    prepareSounds();
    scaleAllTracksVolume(0.5f);

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

    _MainMenu_Cleanup();
    _DeathMenu_Cleanup();

    cleanupSounds();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseAudioDevice(); // Close audio device
    CloseWindow();      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    GameObjPoolDestroy();

    free(ASTEROID_REF_LIST);

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // grab it
    float DeltaTime = GetFrameTime();

    // HandleDebuggingKillPlayerCheck(DeltaTime);

    // check for render
    if (GameShouldRender())
    {
        if (!SoundsStarted)
        {
            startSounds();
            SoundsStarted = true;
        }
        // ...
        soundUpdate();
        ProcessAllUpdates(DeltaTime);
        _DeathMenu_Update(DeltaTime);
    }
    // handle menus
    else
    {
        if (CURRENT_GAME_SCENE_STATE == GAME_SCENE_STATE_MAINMENU)
        {
            _MainMenu_Update(DeltaTime);
        }
    }

    UpdateCamera3D();
    BeginDrawing();
    ClearBackground(BLACK);

    // check for render
    if (GameShouldRender())
    {
        ProcessAllDraws(DeltaTime);
        drawTimer();
        // when dead scene
        if (CURRENT_GAME_SCENE_STATE == GAME_SCENE_STATE_DEAD)
        {
            // draw death menu over the top
            _DeathMenu_Draw();
        }
    }
    // handle menus
    else
    {
        if (CURRENT_GAME_SCENE_STATE == GAME_SCENE_STATE_MAINMENU)
        {
            _MainMenu_Draw();
        }
    }

    EndDrawing();

    ProcessFreshAdd();
    ProcessAllDestroys();

    if (NEXT_FRAME_GAME_STARTS)
    {

        if (__GAMEMANAGER_INITIALISED_BEFORE)
        {
            // __GAMEMANAGER_REF = CreateGameManager();
            // AddToPool(__GAMEMANAGER_REF);
            // aaaa
        }

        CURRENT_GAME_SCENE_STATE = GAME_SCENE_STATE_INGAME;
        NEXT_FRAME_GAME_STARTS = 0;
    }
}

/// @brief Generates all objects for initial gamestate.
static void generateObjects()
{

    // Particle handler.
    AddToPool(CreateParticleObject());

    __GAMEMANAGER_REF = CreateGameManager();
    AddToPool(__GAMEMANAGER_REF);

    AddToPool(CreateTitleManager());
    __GAMEMANAGER_INITIALISED_BEFORE = 1;
}

void HandleDebuggingKillPlayerCheck(float DeltaTime){
    // kill player for debugging
    if(IsKeyDown(KEY_K)){
        playSoundOnce(HIT_SOUND_ID);
        PlayerTakeDamage(PLAYER_OBJECT_REF,DeltaTime,5,5);
    }
}