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

typedef struct {
    Vector2 position;
    int spriteID;
} BackgroundDust_SpawnData;

typedef struct {
    Sprite **spriteList;
    BackgroundDust_SpawnData *spawnDataList;
} BackgroundDust_Data;

#define BACKGROUNDDUST_SPAWN_MAX 1
#define BACKGROUNDDUST_SPRITE_COUNT 11
#define BACKGROUNDDUST_SPAWN_EXTRADISTANCE_MAX WORMHOLE_TRAVEL_DISTANCE/BACKGROUNDDUST_SPAWN_MAX

Vector2 _BackgroundDust_scaleFactor = (Vector2){0.1f, 0.1f};

int _BackgroundDust_Init(void* self, float DeltaTime);
int _BackgroundDust_Update(void* self, float DeltaTime);
int _BackgroundDust_Draw(void* self, float DeltaTime);
int _BackgroundDust_Destroy(void* self, float DeltaTime);

int _BackgroundDust_SpawnData_TestLeftField(GameObj_Base *BackgroundDustObject, BackgroundDust_SpawnData spawnInformation, Vector2 backgroundDustScale);
void _BackgroundDust_SpawnData_RollSpawn(GameObj_Base *BackgroundDustObject, BackgroundDust_SpawnData spawnInformation, Vector2 backgroundDustScale);