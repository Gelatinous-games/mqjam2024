#pragma once

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

#include "../misc_util.h"

#define STAR_RANDOM_RANGE 4

#define STAR_SPRITE_COUNT 9

typedef struct {
    float maxRange;
    float maxPull;
    int spriteID;
} _Star_Data;


// the ID of black hole
#define BLACK_HOLE_SPRITE_ID_1 4
#define BLACK_HOLE_SPRITE_ID_2 4
// d20 dice check for when we roll a black hole for if we should reroll it
#define BLACK_HOLE_REROLL_DC 9
// how much we amplify the stats for them
#define BLACK_HOLE_POWER_FACTOR 1.333333f

Sprite** _starSprites;
