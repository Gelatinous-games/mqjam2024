#pragma once


#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"


typedef struct {
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
} GameObj_Base;
