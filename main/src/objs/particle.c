
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

int _ParticleObject_Particles_DestroyParticle(_Particle* particle) {
    if (particle->func && particle->func_data) {
        free(particle->func_data);
        particle->func = 0;
    }
}

int _ParticleObject_Init(void* self, float DeltaTime) {
    _particleCount = 0;
    _ParticleObject_Particles_Global_Counter = 0;

    _particles = malloc(sizeof(_Particle) * PARTICLE_COUNT);

    return 0;
}
int _ParticleObject_Update(void* self, float DeltaTime) {
    for (int i = 0; i < _particleCount; i++) {
        _Particle particle = _particles[i];

        particle.position = Vector2Add(particle.position, Vector2Scale(particle.velocity, DeltaTime));

        particle.velocity = Vector2Add(particle.velocity, Vector2Scale(particle.acceleration, DeltaTime));

        particle.timeout -= DeltaTime;

        particle.color.a = (unsigned char)((float)particle.oAlpha * (particle.timeout / particle.maxTime));

        if (particle.func) {
            particle.func_ptr(&particle, DeltaTime);
        }

        if (particle.timeout <= 0) {
            // Kill particle, replace it with the contents of the last in the entry. Bubble down.
            _ParticleObject_Particles_DestroyParticle(&_particles[i]);
            _particles[i] = _particles[_particleCount-1];
            _particleCount -= 1;
            i -= 1;
        }
        else {
            _particles[i] = particle;
        }

    }

    return 0;
}
int _ParticleObject_Draw(void* self, float DeltaTime) {
    BeginBlendMode(BLEND_ALPHA);

    for (int i = 0; i < _particleCount; i++) {
        _Particle particle = _particles[i];

        Vector2 pos;
        if (particle.doOutline) {
            pos = GetScreenspacePositionRelative(particle.position, Vector2Scale(particle.size, 2));
            Color tmp = particle.color;
            tmp.a = tmp.a/(float)2;
            DrawRectangleV(pos, GetScaledSize(Vector2Scale(particle.size, 2)), tmp);

        }

        pos = GetScreenspacePositionRelative(particle.position, particle.size);
        DrawRectangleV(pos, GetScaledSize(particle.size), particle.color);
    }


    EndBlendMode();

    return 0;
}
int _ParticleObject_Destroy(void* self, float DeltaTime) {
    free(_particles);

    return 0;
}

/// @brief Spawns a particle with the given parameters. 
/// Does the same function as SpawnParticleEX but with less parameters. This one is here simply because I'm too lazy to replace all instances of this.
/// Still perfectly functional but you should use SpawnParticleEX if you want better function.
/// @param pos The initial position of the particle.
/// @param vel The initial velocity of the particle.
/// @param acc The acceleration vector of the particle.
/// @param size How big the particle is.
/// @param lifetime How long the particle should persist for (in seconds). The particle will fade out.
/// @param color The color of the particle.
/// @param doOutline Set to 1 if the particle should have a bright center and a dimmer outer.
/// @return The ID of the created particle. If the return is -1, then no particle was created.
int SpawnParticle(Vector2 pos, Vector2 vel, Vector2 acc, Vector2 size, float lifetime, Color color, char doOutline) {
    return SpawnParticleEX(pos, vel, acc, size, lifetime, color, doOutline, 1, NULL, NULL);
}

/// @brief Spawns a particle with the given parameters. Parameters include functions, fadeout specifics, and so on.
/// @param pos The initial position of the particle.
/// @param vel The initial velocity of the particle.
/// @param acc The acceleration vector of the particle.
/// @param size How big the particle is.
/// @param lifetime How long the particle should persist for (in seconds). The particle will fade out.
/// @param color The color of the particle.
/// @param doOutline Set to 1 if the particle should have a bright center and a dimmer outer.
/// @param doFadeout Set to 1 if the particle should fade out as it approaches death.
/// @param func_ptr The address of the function that should run per update on this particle. Set to NULL if none.
/// @param func_data A reference to the data package that should be used by this particles function. Set to NULL if none. 
/// @return The ID of the created particle. If the return is -1, then no particle was created.
int SpawnParticleEX(Vector2 pos, Vector2 vel, Vector2 acc, Vector2 size, float lifetime, Color color, char doOutline, char doFadeout, int (*func_ptr)(void* self, float DeltaTime), void* func_data) {
    if (_particleCount >= PARTICLE_COUNT) return -1;

    _Particle tmp;
    tmp.internalID = _ParticleObject_Particles_Global_Counter;
    _ParticleObject_Particles_Global_Counter = _ParticleObject_Particles_Global_Counter + 1;
    tmp.color = color;
    tmp.timeout = lifetime;
    tmp.position = pos;
    tmp.velocity = vel;
    tmp.acceleration = acc;
    tmp.size = size;
    tmp.doOutline = doOutline;
    tmp.func = 0;
    tmp.doFadeout = 

    tmp.maxTime = lifetime;
    tmp.oAlpha = color.a;

    if (func_ptr) 
        tmp.func = 1;

    tmp.func_ptr = func_ptr;
    tmp.func_data = func_data;

    _particles[_particleCount] = tmp;
    _particleCount++;

    return tmp.internalID;
}

/// @brief Gets a particle object with the given ID.
/// @param ParticleID 
/// @return Null if none found.
_Particle* GetParticle(int ParticleID) {
    for (int i = 0; i < _particleCount; i++) {
        if (_particles[i].internalID == ParticleID)
            return &_particles[i];
    }
    return NULL;
}

GameObj_Base* CreateParticleObject() {
    GameObj_Base* obj_ptr = malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_ParticleObject_Init;
    obj_ptr->Update_Func = &_ParticleObject_Update;
    obj_ptr->Draw_Func = &_ParticleObject_Draw;
    obj_ptr->Destroy_Func = &_ParticleObject_Destroy;

    obj_ptr->awaitDestroy = 0;

    obj_ptr->flags = FLAG_PARTICLE;

    obj_ptr->currentLayer = LAYER_PARTICLES;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = Vector2Zero();

    obj_ptr->radius = 0;
    obj_ptr->mass = 0;

    return obj_ptr;
}