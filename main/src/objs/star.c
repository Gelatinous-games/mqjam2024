
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
    #include "src/objs/particle.c"
#endif

#ifndef _sprite
    #define _sprite
    #include "src/sprite.c"
#endif

#ifndef _misc
    #define _misc
    #include "../misc.c"
#endif

#define STAR_RANDOM_RANGE 4

typedef struct {
    float maxRange;
    float maxPull;
} _Star_Data;

#define STAR_DATA ((_Star_Data *)(THIS->data_struct))

void _StarObject_Randomize(void* self) {
    Vector2 originPos;
    Vector2 originVel;
    GameObj_Base* player;
    if (!GetObjectWithFlagsExact(FLAG_PLAYER_OBJECT, 0, &player)) {
        // if no player found, default to camera pos.
        // 0/0; // hollistic debugging.
        originPos = cameraPosition;
        originVel = Vector2Zero();
    }
    else {
        originPos = player->position;
        originVel = player->velocity;
    }

    THIS->position.x = (STAR_RANDOM_RANGE * FLOAT_RAND * cameraBounds.x) + ( 2 * cameraBounds.x); 
    THIS->position.x += originPos.x;

    THIS->position.y = ((FLOAT_RAND) * cameraBounds.y * 2) - cameraBounds.y;

    STAR_DATA->maxPull = (FLOAT_RAND * FREE_MAX_SPEED * 4) + FREE_MAX_SPEED;
    STAR_DATA->maxRange = (FLOAT_RAND * 8) + 8;

    // printf("Placed star at %f (elected origin at %f)\n", THIS->position.x, originPos.x);
    
}

Vector2 GetAccelerationToSink(GameObj_Base* star, GameObj_Base* obj) {
    _Star_Data* starData = (_Star_Data*)star->data_struct;
    float dist = Vector2Distance(obj->position, star->position);

    if (dist > starData->maxRange) return Vector2Zero();

    float delta = dist/starData->maxRange;
    delta = 1-delta; // inverse it to get 1 when closest.
    delta = delta * delta; // square it to get it reducing furher away

    Vector2 ang = Vector2Subtract(star->position, obj->position);
    ang = Vector2Normalize(ang);
    ang = Vector2Scale(ang, delta * starData->maxPull);

    return ang;
}

int _StarObject_Init(void* self, float DeltaTime) {
    _StarObject_Randomize(self);

    return 0;
}

int _StarObject_Update(void* self, float DeltaTime) {
    // stars dont move but for now, if it is too far behind player / far enough ahead, kill it and restart.

    if (THIS->position.x + (2*cameraBounds.x) < cameraPosition.x) {
        // printf("Re-randomizing star (pos %f and cam %f)\n", THIS->position.x, cameraPosition.x);
        _StarObject_Randomize(self);
    }

    return 0;
}

int _StarObject_Draw(void* self, float DeltaTime) {

    // todo: red/blueshift rendering.
    
    // render effective radius
    RenderCircleRelative(THIS->position, STAR_DATA->maxRange, (Color) {255, 127, 0, 100 });
    RenderCircleRelative(THIS->position, THIS->radius, (Color) { 255, 127, 0, 200 });

    return 0;
}

int _StarObject_Destroy(void* self, float DeltaTime) {
    free(STAR_DATA);
    return 0;
}

GameObj_Base* CreateStarObject() {
    GameObj_Base* obj_ptr = malloc(sizeof(GameObj_Base));

    obj_ptr->data_struct = malloc(sizeof(_Star_Data));

    obj_ptr->Init_Func = &_StarObject_Init;
    obj_ptr->Update_Func = &_StarObject_Update;
    obj_ptr->Draw_Func = &_StarObject_Draw;
    obj_ptr->Destroy_Func = &_StarObject_Destroy;

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_GRAVITY_WELL;

    obj_ptr->currentLayer = LAYER_STARS;

    obj_ptr->radius = 1;
    obj_ptr->mass = 10000;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = (Vector2) { 2, 2 };
    return obj_ptr;
}
