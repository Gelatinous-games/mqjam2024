#pragma once

// import this when you want the functions

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "../base.h"
#include "../settings.h"
#include "../misc_util.h"
#include "player.h"


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
    #include "particle.c"
#endif

#include "../sprite.h"
#include "../palette_lib.h"



typedef struct {
    Vector2 orbitCurrentVector;
    float orbitRotationSpeed;
} ShieldParticle_Data;

// instance fields
typedef struct {
    float deltaTimeSinceLastImpact;

    float shieldHealth;
    float shieldRegenerationRate;
    
    float timeSinceLastShieldParticle;
    float shieldRadius;

    int maximumShieldParticles;

    ShieldParticle_Data **particleDataList;
    int nextParticleDataIndex;


} ShieldObject_Data;


// instance constructor
GameObj_Base* CreateShieldObject();
// instance destructor
int _ShieldObject_Destroy(void* self, float DeltaTime);

// required instance methods
int _ShieldObject_Init(void* self, float DeltaTime);
int _ShieldObject_Update(void* self, float DeltaTime);
int _ShieldObject_Draw(void* self, float DeltaTime);


// specialised instance methods
void _ShieldObject_MirrorPlayer(void *self, float DeltaTime);

void _ShieldObject_handlePlayerDeath(void *self, float DeltaTime);
void _ShieldObject_updateShieldHealth(void *self, float DeltaTime);

void _ShieldObject_handleShieldEffect(void *self, float DeltaTime);
void _ShieldObject_emitShieldParticle(void *self, float DeltaTime);



float _ShieldObject_GetPlayerShieldPercentage();
// Color _ShieldObject_GetShieldParticleColor();
float _ShieldObject_TakeDamage(float rawDamage);



int _ShieldParticle_Update(void *self, float DeltaTime);

ShieldParticle_Data *_ShieldParticle_constructData( Vector2 randomVec, float rotationSpeed );