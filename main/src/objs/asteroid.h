
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
    int spriteID;
    float ageTime;
    float degreeRotationSpeed;
    float degreeRotation;


} Asteroid_Data;

#define ASTEROIDDATA ((Asteroid_Data *)(THIS->data_struct))
#define ASTEROID_SPRITE_COUNT 4

Sprite** _asteroidSprites;

void _Asteroid_Randomize(void *self) {
    // sprite
    ASTEROIDDATA->spriteID = (int)(FLOAT_RAND * 4);
    if (ASTEROIDDATA->spriteID == 4) ASTEROIDDATA->spriteID = 3;

    // size
    float rng = (FLOAT_RAND * 2.25) + .25;
    THIS->size = (Vector2) { rng, rng };

    // X position
    THIS->position = (Vector2) { FLOAT_RAND * cameraBounds.x, -cameraBounds.y - THIS->size.y};

    // Horizontal velocity: between 1 and -1
    THIS->velocity = (Vector2) { (FLOAT_RAND * 2) - 1, 1 };
    THIS->velocity = Vector2Normalize(THIS->velocity);

    // Now scale it for a random velocity. Smaller they are, the faster they can move (up to 2.25x faster for smallest)
    THIS->velocity = Vector2Scale(THIS->velocity, (FLOAT_RAND * 2 * (3-rng)) + 1);

    // flip it vertically if flip a coin
    if (FLOAT_RAND < 0.5) {
        THIS->velocity.y *= -1;
        THIS->position.y *= -1;
    }

    rng = Vector2Length(THIS->velocity);

    // we're also going to use the random velocity to decide the rotation speed plus a bit of wiggle.
    ASTEROIDDATA->degreeRotationSpeed = (rng + (((FLOAT_RAND * 0.4) - 0.2) * rng)) * 37;
}

int _Asteroid_Init(void* self, float DeltaTime) {
    // If they havent yet been loaded, load all the asteroid sprites.
    if (!_asteroidSprites) {
        _asteroidSprites = malloc(sizeof(Sprite*) * ASTEROID_SPRITE_COUNT);

        _asteroidSprites[0] = CreateSprite("resources/asteroids/A0.png");
        _asteroidSprites[1] = CreateSprite("resources/asteroids/A1.png");
        _asteroidSprites[2] = CreateSprite("resources/asteroids/A2.png");
        _asteroidSprites[3] = CreateSprite("resources/asteroids/A3.png");
    }

    // Set a random position & size.
    // Position should be somewhere off top or bottom of screen.
    // Velocity should be within an angle to go across the screen.
    // Size should be within a certain range for big asteroids & small asteroids.

    _Asteroid_Randomize(self);
    ASTEROIDDATA->degreeRotation = 0.0f;

    // ==================================================

    // ASTEROIDDATA->sprite = CreateSprite("resources/asteroid_temp.png");

    // ==================================================
    // ==================================================

    return 0;
}

int _Asteroid_Update(void* self, float DeltaTime) {
    ASTEROIDDATA->ageTime += DeltaTime;
    ASTEROIDDATA->degreeRotation += DeltaTime * ASTEROIDDATA->degreeRotationSpeed;

    THIS->position = Vector2Add(THIS->position, Vector2Scale(THIS->velocity, DeltaTime));
    
    if(THIS->position.y > cameraBounds.y + THIS->size.y || THIS->position.y < -(cameraBounds.y + THIS->size.y)){
        // destroy me! release me from this realm of hurt!
        // (and, also, let the asteroid_processor create a new one if it so chooses.)
        // TODO: figure out an asteroid handler!

        // for now we will just reposition the asteroid similar to above.
        _Asteroid_Randomize(self);
    }

    return 0;
}

int _Asteroid_Draw(void* self, float DeltaTime) {
    // TODO: red/blue shift calculation.
    RenderSpriteRelative(_asteroidSprites[ASTEROIDDATA->spriteID], THIS->position, THIS->size, ASTEROIDDATA->degreeRotation, WHITE);
    return 0;
}

int _Asteroid_Destroy(void* self, float DeltaTime) {
    // DestroySprite(ASTEROIDDATA->sprite);
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
    obj_ptr->Init_Func = _Asteroid_Init;
    obj_ptr->Update_Func = _Asteroid_Update;
    obj_ptr->Draw_Func = _Asteroid_Draw;
    obj_ptr->Destroy_Func = _Asteroid_Destroy;
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
