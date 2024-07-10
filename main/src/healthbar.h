#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "sound_manager.h"
#include "base.h"
#include "settings.h"
#include "misc_util.h"

#include "objs/player.h"
#include "objs/shield.h"

#ifndef _camera
    #define _camera
    #include "camera.c"
#endif


#ifndef _obj_pool
    #define _obj_pool
    #include "obj_pool.c"
#endif

#ifndef _obj_particle
    #define _obj_particle
    #include "objs/particle.c"
#endif

#include "sprite.h"


#define HEALTHBAR_GASLEVEL_SPRITE_COUNT 10
#define HEALTHBAR_SHIELDLEVEL_SPRITE_COUNT 10

Sprite *HEALTHBAR_FRAME_SPRITE;
Sprite *HEALTHBAR_OVERLAY_SPRITE;
Sprite **HEALTHBAR_GASLEVEL_LIST;
Sprite **HEALTHBAR_SHIELDLEVEL_LIST;


typedef struct {
    int currentHealthBarIndex;
    int currentShieldBarIndex;
} HealthBar_DataStruct;

