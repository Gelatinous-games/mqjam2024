
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

/** ====== PARENT OBJ DATA ======
    // a structure to refer to for all unique data on this object. void type, so will require casting to the particular func type.
    void* data_struct; 
    int _poolID;

    int currentLayer;

    /// @brief Set to 1 to flag it to be destroyed after the next frame.
    int awaitDestroy;

    // generic data; position, velocity, size.
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    long unsigned int flags;

    int (*Init_Func)(void* self, float DeltaTime);
    int (*Update_Func)(void* self, float DeltaTime);
    int (*Draw_Func)(void* self, float DeltaTime);
    int (*Destroy_Func)(void* self, float DeltaTime);
*/
typedef struct {
    float a;
} Asteroid_Data;

int Asteroid_Init(void* self, float DeltaTime) {
    // we have a reference to our own gameobject from which we can do things.
    // here we should create a reference to our datastructure and store it in the data_struct pointer.

    void* data = malloc(sizeof(Asteroid_Data));
    THIS->data_struct = data; 

    return 0;
}

int Asteroid_Update(void* self, float DeltaTime) {
    // see above

    // we can cast our data struct to the right data like so:

    Asteroid_Data* data = (Asteroid_Data *)(THIS->data_struct);

    // use data here.

    data->a += DeltaTime;

    THIS->position.x = THIS->velocity.x;
    THIS->position.y = THIS->velocity.y;

    for (int i = 0; i != -1; ) {
        GameObj_Base* obj;
        i = GetObjectWithFlagsAny(FLAG_NEUTRAL_OBJECT, i, &obj);

        // Check if obj is not null
        if (!obj || i == -1) break;

        // Do an operation with the result...
    }

    return 0;
}

int Asteroid_Draw(void* self, float DeltaTime) {
    // ibid
    Vector2 screenPosition = GetScreenspacePositionRelative(THIS->position, THIS->size);


    // DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color);
    DrawEllipse(screenPosition.x, screenPosition.y, THIS->size.x, THIS->size.y, WHITE);
    // DrawRectangle(screenPosition.x, screenPosition.y, THIS->size.x, THIS->size.y, WHITE);

    return 0;
}

int Asteroid_Destroy(void* self, float DeltaTime) {
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.

    // free our data struct here. free anything contained.
    free(THIS->data_struct);

    return 0;
}

GameObj_Base* CreateAsteroid() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &Asteroid_Init;
    obj_ptr->Update_Func = &Asteroid_Update;
    obj_ptr->Draw_Func = &Asteroid_Draw;
    obj_ptr->Destroy_Func = &Asteroid_Destroy;

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_UNDEFINED_OBJ;

    obj_ptr->currentLayer = LAYER_GUI;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size.x = 50;
    obj_ptr->size.y = 50;
    return obj_ptr;
}
