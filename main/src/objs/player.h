#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "../base.h"
#include "../settings.h"
#include "../misc_util.h"
#include "shield.h"

#include "../sprite.h"

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

#ifndef _star_obj
    #define _star_obj
    #include "star.c"
#endif

typedef struct {
    Sprite* sprite;
    Vector2 headingVector;
    float rotateJerk;
    float rotateRate;
    float accelRate;
    
    float hullHealth;
    float hullRegenerationRate;

    float damageImmunity;

    float deltaTimeSinceLastImpact;
} Player_Data;








// declare functions we're adding so they can be used
void updateThrustingSoundState(void *self, float DeltaTime);
void handleAsteroidCollistions(void *self, float DeltaTime);
void handleGravityInteractions(void *self, float DeltaTime);
void handleStarProximityAlert(GameObj_Base *self, GameObj_Base *extobj);
void handlePlayerMovement(void *self, float DeltaTime);
void constrainPlayerToCamera(void *self, float DeltaTime);
void handleCameraRelativity(void *self, float DeltaTime);
void updateHealth(void *self, float DeltaTime);


Vector2 GetRandomUnitVector();

float GetPlayerHullPercentage();


Color GetHullParticleColor();
Color GetImpactParticleColor();

void PlayerTakeDamage(void *self, float DeltaTime, int hullRate, int shieldRate);


void _PLAYER_HandleDeath(void *self, float DeltaTime);


int IsPlayerAlive();