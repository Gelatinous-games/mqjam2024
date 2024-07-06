
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
    float a;
    float tmr;
    Sprite* sprite;
} ExampleObject_Data;


#define GAMEMANAGER_DATA ((ExampleObject_Data *)(THIS->data_struct))

int _GameManager_Init(void* self, float DeltaTime) {
    return 0;
}

int _GameManager_Update(void* self, float DeltaTime) {
    return 0;
}

int _GameManager_Draw(void* self, float DeltaTime) {
    return 0;
}

int _GameManager_Destroy(void* self, float DeltaTime) {
    return 0;
}

GameObj_Base* CreateGameManager() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_GameManager_Init;
    obj_ptr->Update_Func = &_GameManager_Update;
    obj_ptr->Draw_Func = &_GameManager_Draw;
    obj_ptr->Destroy_Func = &_GameManager_Destroy;

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_MAN;

    obj_ptr->currentLayer = LAYER_GUI;

    obj_ptr->radius = 0;
    obj_ptr->mass = 100;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = Vector2One();
    
    return obj_ptr;
}
