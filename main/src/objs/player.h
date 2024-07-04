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
    #include "./particle.c"
#endif

#ifndef _misc
    #define _misc
    #include "../misc.c"
#endif



typedef struct {
    Sprite* sprite;
    Vector2 headingVector;
    float rotateJerk;
    float rotateRate;
    float accelRate;
    float health;

} Player_Data;

#define PLAYER_DATA ((Player_Data *)(THIS->data_struct))

int _Player_Init(void* self, float DeltaTime) {
    PLAYER_DATA->headingVector = (Vector2) { 1, 0 };
    PLAYER_DATA->health = 100; //idk!
    PLAYER_DATA->rotateRate = 0; // velocity for rotation. affected by rotatejerk.
    PLAYER_DATA->rotateJerk = 3; // veryy low.
    PLAYER_DATA->accelRate = 6.5; // 5u/s

    PLAYER_DATA->sprite = CreateSprite("resources/spaceship.png");
    return 0;
}

int _Player_Update(void* self, float DeltaTime) {

    // Default steering movement NOT STRAFING mode.
    int rotate_delta = GetKeyDelta(KEY_D, KEY_A);
    if (rotate_delta) {
        // PLAYER_DATA->rotateRate += PLAYER_DATA->rotateJerk * rotate_delta * DeltaTime; // Disabled this as its a headache to control.
        PLAYER_DATA->headingVector = Vector2Rotate(PLAYER_DATA->headingVector, rotate_delta * PLAYER_DATA->rotateJerk * DeltaTime);
    }
    // PLAYER_DATA->headingVector = Vector2Rotate(PLAYER_DATA->headingVector, PLAYER_DATA->rotateRate * DeltaTime); // Ibid

    // Acceleration control.
    int accel_delta = GetKeyDelta(KEY_W, KEY_S);
    if (accel_delta) {
        THIS->velocity = Vector2Add(THIS->velocity, Vector2Scale(PLAYER_DATA->headingVector, accel_delta * DeltaTime * PLAYER_DATA->accelRate));
    }
    THIS->position = Vector2Add(THIS->position, Vector2Scale(THIS->velocity, DeltaTime));

    // COSMETIC, CREATES PARTICLES WHEN ACCELERATING.
    if (accel_delta) {
        float vel = Vector2Length(THIS->velocity);
        Vector2 ang = Vector2Normalize(PLAYER_DATA->headingVector);
        Vector2 rorth = (Vector2) { -ang.y, ang.x };
        Vector2 pos = THIS->position;

        Vector2 origin = pos;
        float tmp = accel_delta == -1 ? 0.05 : -0.5;
        origin = Vector2Add(origin, Vector2Scale(ang, tmp));

        if (accel_delta == 1) ang = Vector2Negate(ang);

        SpawnParticle(
            Vector2Add(origin, Vector2Scale(rorth, -0.225)),
            Vector2Add(Vector2Scale(ang, vel + (accel_delta * DeltaTime * PLAYER_DATA->accelRate)), Vector2Scale(rorth, (FLOAT_RAND-0.5)*0.5)),
            Vector2Zero(),
            (Vector2) {0.0625, 0.0625},
            0.5,
            (Color) {255, 127, 0, 127 },
            (FLOAT_RAND * 1) + 0.5
        );

        SpawnParticle(
            Vector2Add(origin, Vector2Scale(rorth, +0.225)),
            Vector2Add(Vector2Scale(ang, vel + (accel_delta * DeltaTime * PLAYER_DATA->accelRate)), Vector2Scale(rorth, (FLOAT_RAND-0.5)*0.5)),
            Vector2Zero(),
            (Vector2) {0.0625, 0.0625},
            0.5,
            (Color) {255, 127, 0, 127 },
            (FLOAT_RAND * 1) + 0.5
        );

    }

    // bind position within camerabounds y
    if (THIS->position.y > cameraBounds.y + THIS->size.y) {
        THIS->position.y = -cameraBounds.y - THIS->size.y;
    }
    else if (THIS->position.y <= -cameraBounds.y - THIS->size.y) {
        THIS->position.y = cameraBounds.y + THIS->size.y;
    }
    
    // check asteroid collisions
    GameObj_Base* extobj;
    for (int sIDX = 0; sIDX != -1; ) {
        sIDX = GetObjectWithFlagsExact(FLAG_ASTEROID, sIDX, &extobj);

        if (sIDX == -1) break;

        Vector2 impartSelf, impartAsteroid;
        // Check if collision occurs
        if (GetCollided(THIS, extobj, &impartSelf, &impartAsteroid)) {
            Vector2 midPoint = Vector2Scale(Vector2Add(THIS->position, extobj->position), 0.5);
            THIS->velocity = Vector2Add(THIS->velocity, impartSelf);
            extobj->velocity = Vector2Add(extobj->velocity, impartAsteroid);

            PLAYER_DATA->health -= 20;

            // create a spark effect or something

            int randomCount = (FLOAT_RAND * 4) + 4;
            for (int i = 0; i < randomCount; i++) {
                // pick a palette of 4
                Color colourVal;
                int diceRoll = (FLOAT_RAND * 4);
                switch (diceRoll) {
                    case 0: colourVal = (Color) {245, 141, 38, 127};
                    break;
                    case 1: colourVal = (Color) {38, 217, 245, 127};
                    break;
                    case 2: colourVal = (Color) {252, 233, 112, 127};
                    break;
                    case 3: colourVal = (Color) {216, 214, 203, 127};
                    break;
                }
                SpawnParticle(
                    midPoint,
                    Vector2Add(THIS->velocity, (Vector2) { (FLOAT_RAND * 3) / 2, (FLOAT_RAND * 3) / 2}),
                    Vector2Zero(),
                    Vector2Scale(Vector2One(), FLOAT_RAND * .25),
                    (FLOAT_RAND * 1.75) + .25,
                    colourVal,
                    1
                );
            }
        }
    }   

    // lock the camera position to myself.
    cameraPosition.x = THIS->position.x + (THIS->velocity.x * DeltaTime * 2);
    return 0;
}

int _Player_Draw(void* self, float DeltaTime) {
    RenderSpriteRelative(PLAYER_DATA->sprite, THIS->position, THIS->size, Vec2Angle(PLAYER_DATA->headingVector) - 180, WHITE);
    // RenderColliderRelative(THIS->position, THIS->radius); // Debug function for colliders
    return 0;
}

int _Player_Destroy(void* self, float DeltaTime) {
    free(PLAYER_DATA);

    return 0;
}

GameObj_Base* CreatePlayer() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    // ============================================================
    // ==== setup the data scruct data
    obj_ptr->data_struct = malloc(sizeof(Player_Data)); 

    // ============================================================
    obj_ptr->Init_Func = _Player_Init;
    obj_ptr->Update_Func = _Player_Update;
    obj_ptr->Draw_Func = _Player_Draw;
    obj_ptr->Destroy_Func = _Player_Destroy;
    // ============================================================

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_PLAYER_OBJECT;

    obj_ptr->currentLayer = LAYER_PLAYER;

    obj_ptr->radius = 0.45;
    obj_ptr->mass = 1000;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size.x = 1;
    obj_ptr->size.y = 1;

    return obj_ptr;
}