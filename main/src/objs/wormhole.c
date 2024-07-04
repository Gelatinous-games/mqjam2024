
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

#ifndef _sprite
    #define _sprite
    #include "../sprite.c"
#endif

typedef struct {
    Sprite* sprite;
    float distanceFromStart;


} Wormhole_Data;

int _Wormhole_Init(void* self, float DeltaTime) {
    // we have a reference to our own gameobject from which we can do things.
    // here we should create a reference to our datastructure and store it in the data_struct pointer.

    Wormhole_Data* data = (Wormhole_Data *)malloc(sizeof(Wormhole_Data));

    data->distanceFromStart = 100;
    data->sprite = CreateSprite("resources/kitr_temp.png");

    ((GameObj_Base *)self)->data_struct = (void *)data; 

    return 0;
}

int _Wormhole_Update(void* self, float DeltaTime) {
    // we can cast our data struct to the right data like so:
    Wormhole_Data* data = THIS->data_struct;

    THIS->position.x = (float)(sin(data->a * (PI / 2)) * 5);
    data->a += DeltaTime;

    // An example of spawning a particle
    data->tmr += DeltaTime;
    if (data->tmr >= 0.4) {
        data->tmr = 0;
        SpawnParticle(((GameObj_Base *)self)->position, (Vector2) {0, 0}, (Vector2) { 0, 1 }, 
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

    return 0;
}

int _Wormhole_Draw(void* self, float DeltaTime) {
    // ibid
    GameObj_Base* selfObj = ((GameObj_Base *)self);
    Wormhole_Data* data = ((GameObj_Base *)self)->data_struct;

    RenderSpriteRelative(data->sprite, selfObj->position, selfObj->size, sin(data->a)*90, WHITE);
    //RenderSquareRelative(selfObj->position, selfObj->size, 0, WHITE);

    return 0;
}

int _Wormhole_Destroy(void* self, float DeltaTime) {
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.

    // free our data struct here. free anything contained.
    Wormhole_Data* data = ((GameObj_Base *)self)->data_struct;
    free(data->sprite);
    free(data);

    return 0;
}

GameObj_Base* CreateWormholeObject() {
    GameObj_Base* obj_ptr = malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_WormholeObject_Init;
    obj_ptr->Update_Func = &_WormholeObject_Update;
    obj_ptr->Draw_Func = &_WormholeObject_Draw;
    obj_ptr->Destroy_Func = &_WormholeObject_Destroy;

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
