#pragma once

#include "raylib.h"
#include "raymath.h"
#include "base.h"

#define TAU 6.28318530718f

const static int WINDOW_WIDTH = 1366;
const static int WINDOW_HEIGHT = 768;

const static int NUMBER_OF_ASTEROIDS = 10;
static GameObj_Base **ASTEROID_REF_LIST;

static GameObj_Base *PLAYER_OBJECT_REF;
static GameObj_Base *WORMHOLE_OBJECT_REF;
static GameObj_Base *STAR_OBJECT_REF;


static Sound *MAINSOUNDLOOP;
