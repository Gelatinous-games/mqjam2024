
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

#ifndef _obj_particle
    #define _obj_particle
    #include "./particle.c"
#endif

typedef struct {
    float a;
    float tmr;
} ExampleObject_Data;

int _ExampleObject_Init(void* self, float DeltaTime) {
    // we have a reference to our own gameobject from which we can do things.
    // here we should create a reference to our datastructure and store it in the data_struct pointer.

    ExampleObject_Data* data = malloc(sizeof(ExampleObject_Data));
    THIS->data_struct = (void *)data; 

    return 0;
}

int _ExampleObject_Update(void* self, float DeltaTime) {
    // see above

    // we can cast our data struct to the right data like so:

    ExampleObject_Data* data = THIS->data_struct;

    // use data here.


    THIS->position.x = (float)(sin(data->a * (PI / 2)) * 5);
    data->a += DeltaTime;

    data->tmr += DeltaTime;
    if (data->tmr >= 1) {
        data->tmr = 0;
        Color spawnColor;
        spawnColor.r = 255;
        spawnColor.g = 127;
        spawnColor.b = 0;
        spawnColor.a = 127;
        SpawnParticle(THIS->position, Vector2Zero(), Vector2Scale(Vector2One(), 0.125), 2, spawnColor, 0, 1);
    }

    for (int i = 0; i != -1; ) {
        GameObj_Base* obj;
        i = GetObjectWithFlagsAny(FLAG_NEUTRAL_OBJECT, i, &obj);

        // Check if obj is not null
        if (!obj || i == -1) break;

        // Do an operation with the result...
    }

    return 0;
}

int _ExampleObject_Draw(void* self, float DeltaTime) {
    // ibid

    Vector2 sp = GetScreenspacePositionRelative(THIS->position, THIS->size);
    Vector2 size = GetScaledSize(THIS->size);

    DrawRectangleV(sp, size, WHITE);

    return 0;
}

int _ExampleObject_Destroy(void* self, float DeltaTime) {
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.

    // free our data struct here. free anything contained.
    free(THIS->data_struct);

    return 0;
}

GameObj_Base* CreateExampleObject() {
    GameObj_Base* obj_ptr = malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_ExampleObject_Init;
    obj_ptr->Update_Func = &_ExampleObject_Update;
    obj_ptr->Draw_Func = &_ExampleObject_Draw;
    obj_ptr->Destroy_Func = &_ExampleObject_Destroy;

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_UNDEFINED_OBJ;

    obj_ptr->currentLayer = LAYER_GUI;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = Vector2One();
    return obj_ptr;
}
