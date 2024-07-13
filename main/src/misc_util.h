#pragma once

#include "raylib.h"
#include "raymath.h"
#include "base.h"

#include "obj_register.h"
#include "sound_manager.h"

#define COLLIDE_MAX_SPEED 3
#define FREE_MAX_SPEED 10



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

unsigned long long currMillis();
Vector2 GetRandomUnitVector();
int GetKeyDelta(int A, int B);
float Vec2Angle(Vector2 a);
float Vec2Dist(Vector2 a, Vector2 b);
char GetCollided(GameObj_Base* A, GameObj_Base* B, Vector2* impartA, Vector2* impartB);