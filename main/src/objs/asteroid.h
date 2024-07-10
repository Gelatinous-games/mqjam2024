#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "../base.h"
#include "../settings.h"
#include "../misc_util.h"


#ifndef _camera
    #define _camera
    #include "../camera.c"
#endif

#ifndef _obj_pool
    #define _obj_pool
    #include "../obj_pool.c"
#endif

#include "../sprite.h"

#ifndef _obj_particle
    #define _obj_particle
    #include "./particle.c"
#endif

#ifndef _star_obj
    #define _star_obj
    #include "star.c"
#endif



/** ====== PARENT OBJ DATA ======
    // a structure to refer to for all unique data on this object. void type, so will require casting to the particular func type.
    void* data_struct; 
    int _poolID;

    int currentLayer;

    /// @brief Set to 1 to flag it to be destroyed after the next frame.
    int awaitDestroy;

    // generic data; position, velocity, size.
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    long unsigned int flags;

    int (*Init_Func)(void* self, float DeltaTime);
    int (*Update_Func)(void* self, float DeltaTime);
    int (*Draw_Func)(void* self, float DeltaTime);
    int (*Destroy_Func)(void* self, float DeltaTime);
*/
typedef struct {
    int spriteID;
    float ageTime;
    float degreeRotationSpeed;
    float degreeRotation;


} Asteroid_Data;

#define ASTEROID_SPRITE_COUNT 4

#define ASTEROID_RANDOM_RANGE 4

Sprite** _asteroidSprites;
int asteroidsUsing = 0;

Color GetAsteroidParticleColor();


void _Asteroid_Randomize(void *self);
int _Asteroid_Init(void* self, float DeltaTime);
int _Asteroid_Update(void* self, float DeltaTime);
int _Asteroid_Draw(void* self, float DeltaTime);
int _Asteroid_Destroy(void* self, float DeltaTime);
Color GetAsteroidParticleColor();