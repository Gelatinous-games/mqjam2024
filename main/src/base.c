#ifndef _malloc
    #include <malloc.h>
    #define _malloc
#endif 

#ifndef _raylib
    #include "raylib.h"
    #define _raylib
#endif

#ifndef _raymath
    #include "raymath.h"
    #define _raymath
#endif


typedef struct {
    // a structure to refer to for all unique data on this object. void type, so will require casting to the particular func type.
    void* data_struct; 

    // generic data; position, velocity, size.
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    long unsigned int flags;

    int (*init_func)(gameObj_Base* self, float DeltaTime);
    int (*update_func)(gameObj_Base* self, float DeltaTime);
    int (*draw_func)(gameObj_Base* self, float DeltaTime);
    int (*destroy_func)(gameObj_Base* self, float DeltaTime);
} gameObj_Base;