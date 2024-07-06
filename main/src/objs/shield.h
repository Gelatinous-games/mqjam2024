#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "../base.h"
#include "../settings.h"


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

#ifndef _sprite
    #define _sprite
    #include "../sprite.c"
#endif



// instance fields
typedef struct {
    float deltaTimeSinceLastImpact;

    float shieldHealth;
    float shieldRegenerationRate;
    
    float timeSinceLastShieldParticle;
    float shieldMinRange;
    float shieldMaxRange;
    float shieldMaxPull;


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

// TODO: have this referenced by update health
void _ShieldObject_handlePlayerDeath(void *self, float DeltaTime);
void _ShieldObject_updateShieldHealth(void *self, float DeltaTime);

void _ShieldObject_handleShieldEffect(void *self, float DeltaTime);
void _ShieldObject_emitShieldParticle(void *self, float DeltaTime);



float _ShieldObject_GetPlayerShieldPercentage();
Color _ShieldObject_GetShieldParticleColor();
float _ShieldObject_TakeDamage(float rawDamage);



int _ShieldParticle_Update(void *self, float DeltaTime);
Vector2 _ShieldParticle_GetShieldParticleAccelerationToPlayer(_Particle *particleObj);