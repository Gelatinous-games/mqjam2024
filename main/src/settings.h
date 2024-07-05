#pragma once

#include "raylib.h"
#include "raymath.h"
#include "base.h"
#include <sys/time.h>

#define TAU 6.28318530718f

#define FRAMERATE 60

const static int WINDOW_WIDTH = 1366;
const static int WINDOW_HEIGHT = 768;


#define BACKGROUND_OBJECT_COUNT 200

#define BACKGROUND_SPAWN_MAX_DISTANCE_X 400.0f
#define BACKGROUND_SPAWN_MAX_DISTANCE_Y 10.0f


#define BACKGROUND_SPAWN_MIN_SCALE_X 0.5f
#define BACKGROUND_SPAWN_MIN_SCALE_Y 0.5f
#define BACKGROUND_SPAWN_MAX_SCALE_X 3.0f
#define BACKGROUND_SPAWN_MAX_SCALE_Y 3.0f


const static int NUMBER_OF_ASTEROIDS = 4;
static GameObj_Base **ASTEROID_REF_LIST;

struct timeval timerStart;

static GameObj_Base *PLAYER_OBJECT_REF;

static GameObj_Base *BACKGROUND_OBJECT_REF;

static GameObj_Base *WORMHOLE_OBJECT_REF;
static GameObj_Base *PLANET_OBJECT_REF;
static GameObj_Base *STAR_OBJECT_REF;


