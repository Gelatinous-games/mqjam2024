#pragma once

#ifndef _malloc
    #define _malloc
#ifndef __APPLE__
    #include <malloc.h>
#endif
    #include <memory.h>
#else
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
    #include "obj_register.c"
#endif

typedef struct {
    // a structure to refer to for all unique data on this object. void type, so will require casting to the particular func type.
    void* data_struct; 
    int _poolID;

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


// typedef struct GameObj_BaseStruct ;
