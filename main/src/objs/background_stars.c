
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

typedef struct
{
    float a;
    float tmr;

    Sprite *sprite;
} BackgroundStars_Data;

#define BACKGROUNDSTARS_DATA ((BackgroundStars_Data *)(THIS->data_struct))


int _BackgroundStars_Init(void *self, float DeltaTime)
{
    // we have a reference to our own gameobject from which we can do things.
    // here we should create a reference to our datastructure and store it in the data_struct pointer.


    BACKGROUNDSTARS_DATA->a = 0;
    BACKGROUNDSTARS_DATA->tmr = 0;
    BACKGROUNDSTARS_DATA->sprite = CreateSprite("resources/kitr_temp.png");

    THIS->data_struct = malloc(sizeof(BackgroundStars_Data));

    return 0;
}

int _BackgroundStars_Update(void *self, float DeltaTime)
{

    THIS->position.x = (float)(sin(BACKGROUNDSTARS_DATA->a * (PI / 2)) * 5);
    BACKGROUNDSTARS_DATA->a += DeltaTime;

    // An example of spawning a particle
    BACKGROUNDSTARS_DATA->tmr += DeltaTime;
    if (BACKGROUNDSTARS_DATA->tmr >= 0.4)
    {
        BACKGROUNDSTARS_DATA->tmr = 0;
        SpawnParticle(THIS->position, (Vector2){0, 0}, (Vector2){0, 1},
                      (Vector2){0.125, 0.125}, 2,
                      (Color){255, 127, 0, 127}, 1);
    }

    // An example of searching for objects with neutral flag.
    for (int i = 0; i != -1;)
    {
        GameObj_Base *obj;
        i = GetObjectWithFlagsAny(FLAG_NEUTRAL_OBJECT, i, &obj);

        // Check if obj is not null
        if (!obj || i == -1)
            break;

        // Do an operation with the result...
    }

    return 0;
}

int _BackgroundStars_Draw(void *self, float DeltaTime)
{
    // ibid

    RenderSpriteRelative(BACKGROUNDSTARS_DATA->sprite, THIS->position, THIS->size, sin(BACKGROUNDSTARS_DATA->a) * 90, WHITE);
    // RenderSquareRelative(THIS->position, THIS->size, 0, WHITE);

    return 0;
}

int _BackgroundStars_Destroy(void *self, float DeltaTime)
{
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.

    // free our data struct here. free anything contained.
    free(BACKGROUNDSTARS_DATA->sprite);
    free(BACKGROUNDSTARS_DATA);

    return 0;
}

GameObj_Base *CreateBackgroundStars()
{
    GameObj_Base *obj_ptr = malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_BackgroundStars_Init;
    obj_ptr->Update_Func = &_BackgroundStars_Update;
    obj_ptr->Draw_Func = &_BackgroundStars_Draw;
    obj_ptr->Destroy_Func = &_BackgroundStars_Destroy;

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
