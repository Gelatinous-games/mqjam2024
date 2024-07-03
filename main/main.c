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

#define invoken(a, b) a->b(a->self)
#define invoke(a, b, ...) a->b(a->self, __VA_ARGS__)

#define FRAMERATE 60

#ifndef _stdio
    #define _stdio
    #include <stdio.h>
#endif

#ifndef _stdlib
    #define _stdlib
    #include <stdlib.h>
#endif

#ifndef _malloc
    #define _malloc
    #include <malloc.h>
#endif

#ifndef _raylib
    #define _raylib
    #include "raylib.h"
#endif

#ifndef _raymath
    #define _raymath
    #include "raymath.h"
#endif

#ifndef _registry
    #define _registry
    #include "src/obj_register.c"
#endif

#ifndef _obj_pool
    #define _obj_pool
    #include "src/obj_pool.c"
#endif

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

static void UpdateDrawFrame(void);          // Update and draw one frame

int main()
{
    const int screenWidth = 1366;
    const int screenHeight = 768;

    cameraScreenQuarter.x = screenWidth/2;
    cameraScreenQuarter.y = screenHeight/2;

    cameraBounds.x = 16;
    cameraBounds.y = 9;

    layerCount = 8;
    
    GameObjPoolInit();

    InitWindow(screenWidth, screenHeight, "mqjam2024");

    GameObj_Base* obj = CreateExampleObject();
    AddToPool(obj);

    ProcessFreshAdd();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, FRAMERATE, 1);
#else
    SetTargetFPS(FRAMERATE);               // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    GameObjPoolDestroy();

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    float DeltaTime = GetFrameTime();
    ProcessAllUpdates(DeltaTime);

    BeginDrawing();
        ClearBackground(BLACK);
        ProcessAllDraws(DeltaTime);
    EndDrawing();

    ProcessFreshAdd();
    ProcessAllDestroys();
}
