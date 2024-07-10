
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

#include "../sprite.h"

typedef struct {
    float a;
    float sin;
    float tmr;
    float spawnCountdown;
    float deathCountdown;
    char spawned;
    Sprite* sprite;
} ExampleObject_Data;


#define EXAMPLEOBJECT_DATA ((ExampleObject_Data *)(THIS->data_struct))

int _ExampleObject_Init(void* self, float DeltaTime) {
    // we have a reference to our own gameobject from which we can do things.
    // here we should create a reference to our datastructure and store it in the data_struct pointer.

    THIS->data_struct = malloc(sizeof(ExampleObject_Data)); 

    EXAMPLEOBJECT_DATA->a = 0;
    EXAMPLEOBJECT_DATA->tmr = 0;
    EXAMPLEOBJECT_DATA->sprite = CreateSprite("resources/kitr_temp.png");

    THIS->position.y = (FLOAT_RAND * 2 * cameraBounds.y) - cameraBounds.y; 

    EXAMPLEOBJECT_DATA->deathCountdown = (FLOAT_RAND * 5) + 5;
    EXAMPLEOBJECT_DATA->spawnCountdown = (FLOAT_RAND * EXAMPLEOBJECT_DATA->deathCountdown * 0.5) + 2;
    EXAMPLEOBJECT_DATA->spawned = 0;


    return 0;
}

int _ExampleObject_Update(void* self, float DeltaTime) {
    // we can cast our data struct to the right data like so:

    EXAMPLEOBJECT_DATA->sin = (float)(sin(EXAMPLEOBJECT_DATA->a * (PI / 2)) * 5);
    THIS->position.x = EXAMPLEOBJECT_DATA->sin;
    EXAMPLEOBJECT_DATA->a += DeltaTime;

    // An example of spawning a particle
    EXAMPLEOBJECT_DATA->tmr += DeltaTime;
    if (EXAMPLEOBJECT_DATA->tmr >= 0.4) {
        EXAMPLEOBJECT_DATA->tmr = 0;
        SpawnParticle(THIS->position, (Vector2) {0, 0}, (Vector2) { 0, 1 }, 
                        (Vector2) {0.125, 0.125}, 2, 
                        (Color) { 255, 127, 0, 127 }, 1);
    }

    // An example of searching for objects with neutral flag.
    for (int i = 0; i != -1; ) {
        GameObj_Base* obj;
        i = GetObjectWithFlagsAny(FLAG_NEUTRAL_OBJECT, i, &obj);

        // Check if obj is not null
        if (!obj || i == -1) break;

        // Do an operation with the result...
    }

    EXAMPLEOBJECT_DATA->spawnCountdown -= DeltaTime;
    EXAMPLEOBJECT_DATA->deathCountdown -= DeltaTime;

    if (!EXAMPLEOBJECT_DATA->spawned && EXAMPLEOBJECT_DATA->spawnCountdown <= 0) {
        AddToPool(CreateExampleObject());
        EXAMPLEOBJECT_DATA->spawned = 1;
    }

    if (EXAMPLEOBJECT_DATA->deathCountdown <= 0) {
        THIS->awaitDestroy = 1;
    }

    return 0;
}

int _ExampleObject_Draw(void* self, float DeltaTime) {
    // ibid

    RenderSpriteRelative(EXAMPLEOBJECT_DATA->sprite, THIS->position, THIS->size, RAD2DEG * EXAMPLEOBJECT_DATA->sin, WHITE);
    //RenderSquareRelative(THIS->position, THIS->size, 0, WHITE);

    return 0;
}

int _ExampleObject_Destroy(void* self, float DeltaTime) {
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.

    // free our data struct here. free anything contained.
    free(EXAMPLEOBJECT_DATA->sprite);
    free(EXAMPLEOBJECT_DATA);

    return 0;
}

GameObj_Base* CreateExampleObject() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_ExampleObject_Init;
    obj_ptr->Update_Func = &_ExampleObject_Update;
    obj_ptr->Draw_Func = &_ExampleObject_Draw;
    obj_ptr->Destroy_Func = &_ExampleObject_Destroy;

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_UNDEFINED_OBJ;

    obj_ptr->currentLayer = LAYER_GUI;

    obj_ptr->radius = 0;
    obj_ptr->mass = 100;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = Vector2One();
    
    return obj_ptr;
}
