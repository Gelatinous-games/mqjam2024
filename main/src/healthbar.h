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
#include "SpriteLibrary.h"




typedef struct {
    int currentHealthBarIndex;
    int currentShieldBarIndex;
} HealthBar_DataStruct;

