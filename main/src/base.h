#pragma once


#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"
#include "settings.h"

typedef struct {
    // a structure to refer to for all unique data on this object. void type, so will require casting to the particular func type.
    void* data_struct; 
    int _poolID;

    int currentLayer;

    /// @brief Set to 1 to flag it to be destroyed after the next frame.
    int awaitDestroy;

    /// @brief Whether or not it has been created. Defaults to 0.
    char isCreated;

    // generic data; position, velocity, size.
    Vector2 position;
    Vector2 velocity;
    Vector2 size;

    // to be used for calculating collisions, generally.
    float radius;

    // to be used to calculate gravitational attraction.
    float mass;

    // flag string.
    long unsigned int flags;

    int (*Init_Func)(void* self, float DeltaTime);
    int (*Update_Func)(void* self, float DeltaTime);
    int (*Draw_Func)(void* self, float DeltaTime);
    int (*Destroy_Func)(void* self, float DeltaTime);
} GameObj_Base;