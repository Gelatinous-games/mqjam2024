

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
    #include "src/obj_pool.c"
#endif

#define PARTICLE_COUNT = 512;

typedef struct {
    Vector2 pos;
    Vector2 sizePix;
    Vector2 vel;

    int state;
    float timeout;
    Color color;

    /// @brief If true, has a function to apply to the particle.
    char func;
    int (*func_ptr)(void* self, float DeltaTime);
} _Particle;

typedef struct {
    _Particle* particles;
    int currentParticles;

} _ParticleObject_Data;

int _ParticleObject_Init(void* self, float DeltaTime) {

}
int _ParticleObject_Update(void* self, float DeltaTime) {

}
int _ParticleObject_Draw(void* self, float DeltaTime) {

}
int _ParticleObject_Destroy(void* self, float DeltaTime) {

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
}