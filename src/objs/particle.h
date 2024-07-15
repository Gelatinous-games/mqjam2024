#pragma once

#include "raylib.h"
#include "raymath.h"

#include "../base.h"
#include "../obj_register.h"

#ifndef _camera
    #define _camera
    #include "../camera.c"
#endif

#ifndef _obj_pool
    #define _obj_pool
    #include "../obj_pool.c"
#endif

#define PARTICLE_COUNT 512

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    Vector2 size;

    int internalID;

    float timeout;
    float maxTime;
    char doFadeout;
    char oAlpha;
    Color color;

    /// @brief If true, redraws at twice the size and half the opacity.
    char doOutline;
    /// @brief If true, has a function to apply to the particle.
    char func;
    int (*func_ptr)(void* self, float DeltaTime);
    /// @brief A reference to the data packet used by the function, if it is present.
    void* func_data;
} _Particle;

_Particle* _particles;
int _particleCount;
int _ParticleObject_Particles_Global_Counter;


int SpawnParticleEX(Vector2 pos, Vector2 vel, Vector2 acc, Vector2 size, float lifetime, Color color, char doOutline, char doFadeout, void* func_ptr, void* func_data);
