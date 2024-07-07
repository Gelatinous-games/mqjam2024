
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "../base.h"
#include "../settings.h"


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

#ifndef _gm
    #define _gm
    #include "src/objs/gameManager.c"
#endif

typedef struct {
    Sprite* sprite;
    float distanceFromStart;
    float rotate;
    char active;
} Wormhole_Data;

#define WORMHOLE_DATA ((Wormhole_Data *)(THIS->data_struct))

int _Wormhole_Init(void* self, float DeltaTime) {
    // we have a reference to our own gameobject from which we can do things.
    // here we should create a reference to our datastructure and store it in the data_struct pointer.

    THIS->data_struct = malloc(sizeof(Wormhole_Data));

    WORMHOLE_DATA->distanceFromStart = WORMHOLE_TRAVEL_DISTANCE;
    WORMHOLE_DATA->sprite = CreateSprite("resources/wormhole/w1.png");

    // default to active
    WORMHOLE_DATA->active = 1;
    WORMHOLE_DATA->rotate = 0;

    if (TO_WORMHOLE)
        THIS->position.x = WORMHOLE_DATA->distanceFromStart;
    else 
        THIS->position.x = -cameraBounds.x;

    THIS->position.y = 0;

    THIS->size.x = 10;
    THIS->size.y = 10;

    THIS->radius = cameraBounds.y;

    return 0;
}

int _Wormhole_Update(void* self, float DeltaTime) {
    WORMHOLE_DATA->rotate -= DeltaTime * 16;
    // we can cast our data struct to the right data like so:
    Wormhole_Data* data = THIS->data_struct;

    // // dont let the player past the wormhole
    // if (PLAYER_OBJECT_REF->position.x > THIS->position.x) {
    //     PLAYER_OBJECT_REF->position.x = THIS->position.x;
    // }

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
    Wormhole_Data* data = THIS->data_struct;

    RenderSpriteRelative(data->sprite, THIS->position, THIS->size, WORMHOLE_DATA->rotate, WHITE);

    // RenderColliderRelative(THIS->position, THIS->radius);

    return 0;
}

int _Wormhole_Destroy(void* self, float DeltaTime) {
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.

    // free our data struct here. free anything contained.
    Wormhole_Data* data = THIS->data_struct;
    free(data->sprite);
    free(data);

    return 0;
}

GameObj_Base* CreateWormhole() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_Wormhole_Init;
    obj_ptr->Update_Func = &_Wormhole_Update;
    obj_ptr->Draw_Func = &_Wormhole_Draw;
    obj_ptr->Destroy_Func = &_Wormhole_Destroy;

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_WORMHOLE;

    obj_ptr->currentLayer = LAYER_WORMHOLE;

    obj_ptr->radius = 0;
    obj_ptr->mass = 100;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = (Vector2) { 3, 3 } ;
    return obj_ptr;
}
