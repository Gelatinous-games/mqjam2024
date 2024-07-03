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

#ifndef _stdio
    #define _stdio
    #include <stdio.h>
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

#ifndef _game_base
    #define _game_base
    #include "src\base.c"
#endif

#ifndef _camera
    #define _camera
    #include "src\camera.c"
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

gameObj_Base* exampleObject;

int main()
{
    const int screenWidth = 1366;
    const int screenHeight = 768;

    CameraScreenQuarter.x = screenWidth/2;
    CameraScreenQuarter.y = screenHeight/2;

    exampleObject = CreateExampleObject();

    CameraBounds.x = 16;
    CameraBounds.y = 9;

    InitWindow(screenWidth, screenHeight, "mqjam2024");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    exampleObject->update_func(exampleObject, 1/(float)60);

    BeginDrawing();
        ClearBackground(BLACK);
        exampleObject->draw_func(exampleObject, 1);
    EndDrawing();
}
