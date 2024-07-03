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

#ifndef _game_base
    #include "base.c"
    #define _game_base
#endif

gameObj_Base* CreateExampleObject() {
    gameObj_Base* obj_ptr = malloc(sizeof(gameObj_Base));

    obj_ptr->init_func = ExampleObject_Init;
    obj_ptr->update_func = ExampleObject_Update;
    obj_ptr->draw_func = ExampleObject_Draw;
    obj_ptr->destroy_func = ExampleObject_Destroy;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = 0;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = Vector2One();

    // also a pointer to main scene / list of objects.
    // this hasn't been added yet, check back later.
}

typedef struct {
    int a;
    int b;
    int c;
    int d;
} ExampleObject_Data;

int ExampleObject_Init(gameObj_Base* self, float DeltaTime) {
    // we have a reference to our own gameobject from which we can do things.
    // here we should create a reference to our datastructure and store it in the data_struct pointer.

    ExampleObject_Data* data = malloc(sizeof(ExampleObject_Data));
    self->data_struct = (void*)data; 

    return 0;
}

int ExampleObject_Update(gameObj_Base* self, float DeltaTime) {
    // see above

    // we can cast our data struct to the right data like so:

    ExampleObject_Data* data = self->data_struct;

    return 0;
}

int ExampleObject_Draw(gameObj_Base* self, float DeltaTime) {
    // ibid
    return 0;
}

int ExampleObject_Destroy(gameObj_Base* self, float DeltaTime) {
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.

    // free our data struct here. free anything contained.
    free(self->data_struct);

    return 0;
}