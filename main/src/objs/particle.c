

#ifndef _raylib
    #define _raylib
    #include "raylib.h"
#endif

#ifndef _raymath
    #define _raymath
    #include "raymath.h"
#endif

#ifndef _game_base
    #define _game_base
    #include "../base.h"
#endif

#ifndef _camera
    #define _camera
    #include "../camera.c"
#endif

#ifndef _registry
    #define _registry
    #include "../obj_register.h"
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

    int state;
    float timeout;
    float maxTime;
    char oAlpha;
    Color color;

    /// @brief If true, redraws at twice the size and half the opacity.
    char doOutline;
    /// @brief If true, has a function to apply to the particle.
    char func;
    int (*func_ptr)(void* self, float DeltaTime);
} _Particle;

_Particle* _particles;
int _particleCount;

int _ParticleObject_Init(void* self, float DeltaTime) {
    GameObj_Base* selfObj = (GameObj_Base*)self;

    _particleCount = 0;
    _particles = malloc(sizeof(_Particle) * PARTICLE_COUNT);

    return 0;
}
int _ParticleObject_Update(void* self, float DeltaTime) {
    GameObj_Base* selfObj = (GameObj_Base*)self;

    for (int i = 0; i < _particleCount; i++) {
        _Particle particle = _particles[i];

        particle.position = Vector2Add(particle.position, Vector2Scale(particle.velocity, DeltaTime));

        particle.velocity = Vector2Add(particle.velocity, Vector2Scale(particle.acceleration, DeltaTime));

        particle.timeout -= DeltaTime;

        particle.color.a = (char)((float)particle.oAlpha * (particle.timeout / particle.maxTime));

        if (particle.func) {
            particle.func_ptr(&particle, DeltaTime);
        }

        if (particle.timeout <= 0) {
            // Kill particle, replace it with the contents of the last in the entry. Bubble down.
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
    for (int i = 0; i < _particleCount; i++) {
        _Particle particle = _particles[i];

        Vector2 pos;
        if (particle.doOutline) {
            pos = GetScreenspacePositionRelative(particle.position, Vector2Scale(particle.size, 0.5));
            Color tmp = particle.color;
            tmp.a = tmp.a/2;
            DrawRectangleV(pos, GetScaledSize(Vector2Scale(particle.size, 0.5)), tmp);
        }

        pos = GetScreenspacePositionRelative(particle.position, particle.size);
        DrawRectangleV(pos, GetScaledSize(particle.size), particle.color);
    }

    return 0;
}
int _ParticleObject_Destroy(void* self, float DeltaTime) {
    free(_particles);

    return 0;
}

int SpawnParticle(Vector2 pos, Vector2 vel, Vector2 acc, Vector2 size, float lifetime, Color color, int state, char doOutline) {
    if (_particleCount >= PARTICLE_COUNT) return -1;
    _Particle tmp;
    tmp.color = color;
    tmp.timeout = lifetime;
    tmp.position = pos;
    tmp.velocity = vel;
    tmp.acceleration = acc;
    tmp.size = size;
    tmp.state = state;
    tmp.doOutline = doOutline;
    tmp.func = 0;

    tmp.maxTime = lifetime;
    tmp.oAlpha = color.a;

    _particles[_particleCount] = tmp;
    _particleCount++;

    return 0;
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

    return obj_ptr;
}