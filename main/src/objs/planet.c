
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

#include "../settings.h"

typedef struct {
    int spriteID;
    float distanceFromStart;

    float rotate;
} Planet_DataStruct;

#define PLANET_DATA ((Planet_DataStruct *)(THIS->data_struct))

#define PLANET_SPRITE_COUNT 2

Sprite** _planetSprites;

int _Planet_Init(void* self, float DeltaTime) {
    // we have a reference to our own gameobject from which we can do things.
    // here we should create a reference to our datastructure and store it in the data_struct pointer.
    if(!_planetSprites){
        _planetSprites = (Sprite **)malloc(PLANET_SPRITE_COUNT * sizeof(Sprite*));
        _planetSprites[0] = CreateSprite("resources/planets/p0.png");
        _planetSprites[1] = CreateSprite("resources/planets/p1.png");
    }



    THIS->data_struct = malloc(sizeof(Planet_DataStruct));

    
    // choose randome sprite
    PLANET_DATA->spriteID = (abs((int)FLOAT_RAND)%PLANET_SPRITE_COUNT);
    if (TO_WORMHOLE)
        THIS->position.x = PLANET_DATA->distanceFromStart;
    else
        THIS->position.x = WORMHOLE_TRAVEL_DISTANCE;
    THIS->position.y = 0;
    PLANET_DATA->distanceFromStart = THIS->position.x;

    PLANET_DATA->rotate = 0;

    THIS->size.x = 12;
    THIS->size.y = 12;

    THIS->radius = cameraBounds.y;

    return 0;
}

int _Planet_Update(void* self, float DeltaTime) {

    // // dont let the player past the Planet
    // if (PLAYER_OBJECT_REF->position.x > THIS->position.x) {
    //     PLAYER_OBJECT_REF->position.x = THIS->position.x;
    // }

    PLANET_DATA->rotate += DeltaTime *2;

    return 0;
}

int _Planet_Draw(void* self, float DeltaTime) {
    // ibid

    RenderSpriteRelative(_planetSprites[PLANET_DATA->spriteID], THIS->position, THIS->size, PLANET_DATA->rotate, WHITE);

    return 0;
}

int _Planet_Destroy(void* self, float DeltaTime) {
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.

    // free our data struct here. free anything contained.
    for(int i = 0; i < PLANET_SPRITE_COUNT; i++){
        DestroySprite(_planetSprites[i]);
    }
    free(_planetSprites);
    _planetSprites = 0;


    free(PLANET_DATA);

    return 0;
}

GameObj_Base* CreatePlanet() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_Planet_Init;
    obj_ptr->Update_Func = &_Planet_Update;
    obj_ptr->Draw_Func = &_Planet_Draw;
    obj_ptr->Destroy_Func = &_Planet_Destroy;

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
    obj_ptr->size = Vector2One();
    return obj_ptr;
}
