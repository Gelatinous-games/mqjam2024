
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

#ifndef _sprite
    #define _sprite
    #include "src/sprite.c"
#endif

/** ====== PARENT OBJ DATA ======
    // a structure to refer to for all unique data on this object. void type, so will require casting to the particular func type.
    void* data_struct; 
    int _poolID;

    int currentLayer;

    /// @brief Set to 1 to flag it to be destroyed after the next frame.
    int awaitDestroy;

    // generic data; position, velocity, size.
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    long unsigned int flags;

    int (*Init_Func)(void* self, float DeltaTime);
    int (*Update_Func)(void* self, float DeltaTime);
    int (*Draw_Func)(void* self, float DeltaTime);
    int (*Destroy_Func)(void* self, float DeltaTime);
*/
typedef struct {
    Sprite* sprite;
    float ageTime;
    float degreeRotationSpeed;
    float degreeRotation;
} Asteroid_Data;

#define ASTEROIDDATA ((Asteroid_Data *)(THIS->data_struct))

int Asteroid_Init(void* self, float DeltaTime) {
    // Set a random position & size.
    // Position should be somewhere off top or bottom of screen.
    // Velocity should be within an angle to go across the screen.
    // Size should be within a certain range for big asteroids & small asteroids.

    // ==================================================

    ASTEROIDDATA->sprite = CreateSprite("resources/asteroid.png");

    // ==================================================
    
    
    ASTEROIDDATA->degreeRotation = 0.0f;
    ASTEROIDDATA->degreeRotationSpeed = 10;
    
    // ==================================================

    return 0;
}

int Asteroid_Update(void* self, float DeltaTime) {
    ASTEROIDDATA->ageTime += DeltaTime;
    ASTEROIDDATA->degreeRotation += DeltaTime * ASTEROIDDATA->degreeRotationSpeed;

    THIS->position = Vector2Add(THIS->position, Vector2Scale(THIS->velocity, DeltaTime));
    
    if(THIS->position.y > cameraBounds.y + THIS->size.y || THIS->position.y < -(cameraBounds.y + THIS->size.y)){
        // destroy me! release me from this realm of hurt!
        // (and, also, let the asteroid_processor create a new one if it so chooses.)
        // TODO: figure out an asteroid handler!

        // for now we will just reposition the asteroid similar to above.
    }

    return 0;
}

int Asteroid_Draw(void* self, float DeltaTime) {
    // TODO: red/blue shift calculation.
    RenderSpriteRelative(ASTEROIDDATA->sprite, THIS->position, THIS->size, ASTEROIDDATA->degreeRotation, WHITE);
    return 0;
}

int Asteroid_Destroy(void* self, float DeltaTime) {
    DestroySprite(ASTEROIDDATA->sprite);
    free(ASTEROIDDATA);
    return 0;
}

GameObj_Base* CreateAsteroid() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    // ============================================================
    // ==== setup the data struct data
    obj_ptr->data_struct = malloc(sizeof(Asteroid_Data)); 

    // ============================================================
    // ==== assign the ufnctions
    obj_ptr->Init_Func = Asteroid_Init;
    obj_ptr->Update_Func = Asteroid_Update;
    obj_ptr->Draw_Func = Asteroid_Draw;
    obj_ptr->Destroy_Func = Asteroid_Destroy;
    // ============================================================
    
    obj_ptr->awaitDestroy = 0;

    obj_ptr->flags = FLAG_ASTEROID;

    obj_ptr->currentLayer = LAYER_ASTEROIDS;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = (Vector2) {2, 2};

    return obj_ptr;
}
