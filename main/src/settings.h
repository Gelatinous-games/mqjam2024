#pragma once

#include "raylib.h"
#include "raymath.h"
#include "base.h"
#include <sys/time.h>

#define TAU 6.28318530718f

#define FRAMERATE 60

const static int WINDOW_WIDTH = 1366;
const static int WINDOW_HEIGHT = 768;


enum GAME_SCENE_STATE {
    GAME_SCENE_STATE_MAINMENU = 0,
    GAME_SCENE_STATE_INGAME = 1
};

// static int CURRENT_GAME_SCENE_STATE = GAME_SCENE_STATE_MAINMENU;
// skip till it's implemented
static int CURRENT_GAME_SCENE_STATE = GAME_SCENE_STATE_INGAME;


#define CAMERA_COAST_SPEED 3


#define MINIMUM_SHIELD_PERCENT 0.02f
#define SHIELD_BASE_RADIUS 0.667f

#define SHIELD_REGEN_TIMEOUT_DELTATIME 2.0f

#define SHIELD_REGEN_BASE_RATE 15.0f
#define HULL_REGEN_BASE_RATE 5.0f

#define MAXIMUM_HULL 100
#define MAXIMUM_SHIELDS 250

#define MAXIMUM_TOTAL_HEALTH (MAXIMUM_HULL+MAXIMUM_SHIELDS)



#define SHIELD_PARTICLE_SCALE_MIN 0.15f
#define SHIELD_PARTICLE_SCALE_MAX 0.35f
#define SHIELD_PARTICLE_LIFE_MIN 0.25f
#define SHIELD_PARTICLE_LIFE_MAX 1.5f
#define SHIELD_PARTICLE_ORBIT_RATE_MIN 12
#define SHIELD_PARTICLE_ORBIT_RATE_MAX 35
#define SHIELD_EMITTER_MAX_PARTICLE_PER_DELTA 40


#define ASTEROID_IMPACT_DAMMAGE_SHIELDED 10
#define ASTEROID_IMPACT_DAMMAGE_HULL 20

#define STAR_IMPACT_DAMMAGE_SHIELDED 15
#define STAR_IMPACT_DAMMAGE_HULL 30





#define SHIELD_PARTICLE_COLOUR_0 (Color){  34, 195, 222, 127 }
#define SHIELD_PARTICLE_COLOUR_1 (Color){  27, 118, 213, 127 }
#define SHIELD_PARTICLE_COLOUR_2 (Color){  34, 107, 220, 127 }
#define SHIELD_PARTICLE_COLOUR_3 (Color){  25,  66, 219, 127 }


#define ALIVE_HEALTHBAR_FRAME_TINT (Color){255,255,255,127}
#define DEATH_HEALTHBAR_FRAME_TINT (Color){150,100,100,75}
#define DEATH_HEALTHBAR_OVERLAY_TINT (Color){255,255,255,127}

#define WORMHOLE_SPWAWN_POSITION_X 100.0f

#define BACKGROUND_OBJECT_COUNT 50

#define BACKGROUND_SPAWN_MAX_DISTANCE_X (WORMHOLE_SPWAWN_POSITION_X+20.0f)
#define BACKGROUND_SPAWN_MAX_DISTANCE_Y 10.0f

#define BACKGROUND_SPAWN_MIN_SCALE_X 0.5f
#define BACKGROUND_SPAWN_MIN_SCALE_Y 0.5f
#define BACKGROUND_SPAWN_MAX_SCALE_X 3.0f
#define BACKGROUND_SPAWN_MAX_SCALE_Y 3.0f

#define BACKGROUNDSTARS_COLOUR (Color){ 253, 249, 200, 100 }//yellow
#define BACKGROUNDSTARS_MIN_NUMBER_STARS 20
#define BACKGROUNDSTARS_MAX_NUMBER_STARS 10



struct timeval timerStart;


#define SHIELD_DATA_GLOBAL_ACCESS ((ShieldObject_Data *)SHIELD_OBJECT_REF->data_struct)
static GameObj_Base *SHIELD_OBJECT_REF;


#define PLAYER_DATA_GLOBAL_ACCESS ((Player_Data *)PLAYER_OBJECT_REF->data_struct)
static GameObj_Base *PLAYER_OBJECT_REF;


const static int NUMBER_OF_BACKGROUNDSTARS_LAYERS = 3;
static GameObj_Base **BACKGROUNDSTARS_EFFECT_REF_LIST;
static GameObj_Base **BACKGROUND_OBJECT_REF_LIST;



const static int NUMBER_OF_ASTEROIDS = 4;
static GameObj_Base **ASTEROID_REF_LIST;

static GameObj_Base *WORMHOLE_OBJECT_REF;
static GameObj_Base *PLANET_OBJECT_REF;
static GameObj_Base *STAR_OBJECT_REF;


unsigned long long currMillis();
Vector2 GetRandomUnitVector();




#define THIS ((GameObj_Base *)self)
#ifndef _WIN32
// unix randoming
#define INT_RAND (int)(random())
#define FLOAT_RAND ((float)(random()) / (float)RAND_MAX)
#define CHAR_RAND ((char)(random() & 0xff))
#define GAME_TIME currMillis()
// #define GAME_TIME GetSystemTime( &systemTime )
#else
// windows randoming
#define INT_RAND (int)(rand())
#define FLOAT_RAND ((float)(rand()) / (float)RAND_MAX)
#define CHAR_RAND ((char)(rand() & 0xff))
#define GAME_TIME GetTime
#endif



unsigned long long currMillis(){
    struct timeval timeVal;
    unsigned long long millisecondsSinceEpoch;

    gettimeofday(&timeVal, NULL);

    millisecondsSinceEpoch =
        (unsigned long long)(timeVal.tv_sec) * 1000 +
        (unsigned long long)(timeVal.tv_usec) / 1000;
    
    return millisecondsSinceEpoch;
}

Vector2 GetRandomUnitVector(){
    float angle = FLOAT_RAND*360.0;
    return Vector2Rotate((Vector2){ 1.0f, 0.0f}, angle);
}