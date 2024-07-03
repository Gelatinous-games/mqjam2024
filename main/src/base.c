
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

typedef struct {
    // a structure to refer to for all unique data on this object. void type, so will require casting to the particular func type.
    void* data_struct; 

    // generic data; position, velocity, size.
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    long unsigned int flags;

    int (*init_func)(void* self, float DeltaTime);
    int (*update_func)(void* self, float DeltaTime);
    int (*draw_func)(void* self, float DeltaTime);
    int (*destroy_func)(void* self, float DeltaTime);
} gameObj_Base;