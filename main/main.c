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

#ifndef _background_spritelibrary
#define _background_spritelibrary
#include "src/objs/background_spritelibrary.c"
#endif

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

#ifndef _textobj
    #define _textobj
    #include "src/objs/textobj.c"
#endif

#ifndef _wormhole_obj
#define _wormhole_obj
#include "src/objs/wormhole.c"
#endif
#ifndef _planet_obj
#define _planet_obj
#include "src/objs/planet.c"
#endif

#ifndef _healthbar
#define _healthbar
#include "src/healthbar.c"
#endif

#include "src/objs/gameManager.c"
#include "src/objs/titlemanager.c"
#include "src/objs/restartManager.c"

// #define DEBUG_SPAMMER_PRINTF_PREFIX if(true) 

// let C know this exists


void _MAIN_InitialiseGlobalGameEnvironment();
void _MAIN_DestroyGlobalGameEnvironment();

static void _MAIN_DrawGlobalGameEnvironment(void); // Update and draw one frame


static void _MAIN_PrepareMainMenu();


static void prepareSounds();
static void cleanupSounds();





int main()
{

    _MAIN_InitialiseGlobalGameEnvironment();

#ifndef PLATFORM_WEB
    SetTargetFPS(FRAMERATE);     // Set our game to run at 60 frames-per-second
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        _MAIN_DrawGlobalGameEnvironment();
    }
#else
    // printf("%s\n",">> main() :: emscripten");
    emscripten_set_main_loop(_MAIN_DrawGlobalGameEnvironment, FRAMERATE, 1);
#endif

    _MAIN_DestroyGlobalGameEnvironment();

    return 0;
}

// Update and draw game frame
static void _MAIN_DrawGlobalGameEnvironment(void)
{

    // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n","################## _MAIN_DrawGlobalGameEnvironment() ### START ###################");
    // grab it
    float DeltaTime = GetFrameTime();

    // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n","---->> _MAIN_DrawGlobalGameEnvironment() :: TRY RENDER");
    if (!SoundsStarted)
    {
        startSounds();
        SoundsStarted = true;
    }
    // ...
    // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n","---->> _MAIN_DrawGlobalGameEnvironment() :: SOUND UPDATE");
    soundUpdate();
    // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n","---->> _MAIN_DrawGlobalGameEnvironment() :: ALL UPDATE");
    ProcessAllUpdates(DeltaTime);

    // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n","---->> _MAIN_DrawGlobalGameEnvironment() :: UPDATE CAM");
    UpdateCamera3D();
    BeginDrawing();

    // -------------------

    ClearBackground(BLACK);

    // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n","---->> _MAIN_DrawGlobalGameEnvironment() :: PROCESS DRAWS");
    ProcessAllDraws(DeltaTime);
    // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n","---->> _MAIN_DrawGlobalGameEnvironment() :: DRAW TIMER");
    drawTimer();

    // -------------------

    // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n","---->> _MAIN_DrawGlobalGameEnvironment() :: END DRAW");
    EndDrawing();

    // DEBUG_SPAMMER_PRINTF_PREFIX printf("%s\n","---->> _MAIN_DrawGlobalGameEnvironment() :: PROCESS ADD/DESTROY");
    ProcessFreshAdd();
    ProcessAllDestroys();


}

/// @brief prepares all objects for initial main menu.
static void _MAIN_PrepareMainMenu()
{
    AddToPool( CreateTitleManager() );
    CURRENT_GAME_SCENE_STATE = GAME_SCENE_STATE_MAINMENU;
}


void _MAIN_InitialiseGlobalGameEnvironment(){

    gettimeofday(&timerStart, NULL);

    SetExitKey(KEY_F4); // Lets not make it *too* easy to leave lol

    cameraScreenQuarter.x = WINDOW_WIDTH / 2;
    cameraScreenQuarter.y = WINDOW_HEIGHT / 2;

    cameraBounds.x = 8;
    cameraBounds.y = 4.5;
    cameraVelocity = Vector2Zero();
    cameraUnitSize = Vector2Divide(cameraScreenQuarter, cameraBounds);

    // OpenGL initialisation
    //--------------------------------------------------------------------------------------
    GameObjPoolInit();
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "mqjam2024");
    InitAudioDevice(); // Initialize audio device
    //--------------------------------------------------------------------------------------
    

    // loads our sprites ready for use
    prepareBackgroundSprites();

    // Particle handler, so it's available during the main menu
    AddToPool(CreateParticleObject());

    prepareSounds();


    _MAIN_PrepareMainMenu();

    scaleAllTracksVolume(0.5f);

    ProcessFreshAdd();
}



void _MAIN_DestroyGlobalGameEnvironment(){
    // ...

    cleanupSounds();

    destroyBackgroundSprites();


    // OpenGL De-Initialization
    //--------------------------------------------------------------------------------------
    CloseAudioDevice(); // Close audio device
    CloseWindow();      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    GameObjPoolDestroy();

}





// #undef DEBUG_SPAMMER_PRINTF_PREFIX


