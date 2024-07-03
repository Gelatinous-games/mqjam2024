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

#define FRAMERATE 60

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
#include "src/base.h"
#include "src/obj_register.h"
#include "src/objs/asteroid.h"

// project source includes

#ifndef _object_example
    #define _object_example
    #include "src/objs/object_example.c"
#endif

#ifndef _particle
    #define _particle
    #include "src/objs/particle.c"
#endif

#ifndef _obj_pool
    #define _obj_pool
    #include "src/obj_pool.c"
#endif




static void UpdateDrawFrame(void);          // Update and draw one frame

// let C know this exists
void generateObjects();

int main()
{
    const int screenWidth = 1366;
    const int screenHeight = 768;

    cameraScreenQuarter.x = screenWidth/2;
    cameraScreenQuarter.y = screenHeight/2;

    cameraBounds.x = 16;
    cameraBounds.y = 9;

    // printf("%s\n",">> main() :: init game pool");
    layerCount = 8;
    
    GameObjPoolInit();

    // printf("%s\n",">> main() :: initialising window");
    InitWindow(screenWidth, screenHeight, "mqjam2024");
    
    generateObjects();
    
// Asteroid_Draw


    // printf("%s\n",">> main() :: process fresh add");
    ProcessFreshAdd();

    // printf("%s\n",">> main() :: try");
#ifndef PLATFORM_WEB
    // printf("%s\n",">> main() :: set fps");
    SetTargetFPS(FRAMERATE);               // Set our game to run at 60 frames-per-second

    // printf("%s\n",">> main() :: pre loop");
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#else
    // printf("%s\n",">> main() :: emscripten");
    emscripten_set_main_loop(UpdateDrawFrame, FRAMERATE, 1);
#endif

    // printf("%s\n",">> main() :: closing");
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


// define the thing we promised
void generateObjects(){
    // printf("%s\n",">> main() :: create example obj");
    GameObj_Base* obj = CreateExampleObject();
    // printf("%s\n",">> main() :: add example obj");
    AddToPool(obj);



    // printf("%s\n",">> main() :: create asteroid obj");
    GameObj_Base* asteroid = CreateAsteroid();
    // printf("%s\n",">> main() :: add asteroid obj");
    AddToPool(asteroid);
}