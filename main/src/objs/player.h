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
    #include "src/objs/particle.c"
#endif

#ifndef _misc
    #define _misc
    #include "../misc.c"
#endif

#define CAMERA_COAST_SPEED 3

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

    cameraVelocity.x = CAMERA_COAST_SPEED;

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

        SpawnParticle(Vector2Add(origin, Vector2Scale(rorth, -0.225)), Vector2Add(Vector2Scale(ang, vel + (accel_delta * DeltaTime * PLAYER_DATA->accelRate)), Vector2Scale(rorth, (FLOAT_RAND-0.5)*0.5)), Vector2Zero(), (Vector2) {0.0625, 0.0625}, 0.5, (Color) {255, 127, 0, 127 }, (FLOAT_RAND * 1) + 0.5);

        SpawnParticle(Vector2Add(origin, Vector2Scale(rorth, +0.225)), Vector2Add(Vector2Scale(ang, vel + (accel_delta * DeltaTime * PLAYER_DATA->accelRate)), Vector2Scale(rorth, (FLOAT_RAND-0.5)*0.5)), Vector2Zero(), (Vector2) {0.0625, 0.0625}, 0.5, (Color) {255, 127, 0, 127 }, (FLOAT_RAND * 1) + 0.5);

    }

    // bind position within camerabounds y
    if (THIS->position.y > cameraBounds.y) {
        THIS->position.y = cameraBounds.y;
        THIS->velocity.y = 0;
    }
    else if (THIS->position.y < -cameraBounds.y) {
        THIS->position.y = -cameraBounds.y;
        THIS->velocity.y = 0;
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

            int randNum = (FLOAT_RAND * 4) + 4;
            for (int i = 0; i < randNum; i++) {
                // pick a palette of 4
                Color col;
                int k = (FLOAT_RAND * 4);
                switch (k) {
                    case 0: col = (Color) {245, 141, 38, 200};
                    break;
                    case 1: col = (Color) {38, 217, 245, 200};
                    break;
                    case 2: col = (Color) {252, 233, 112, 200};
                    break;
                    case 3: col = (Color) {216, 214, 203, 200};
                    break;
                }
                SpawnParticle(midPoint, Vector2Add(THIS->velocity, (Vector2) { (FLOAT_RAND * 3) / 2, (FLOAT_RAND * 3) / 2}), Vector2Zero(), Vector2Scale(Vector2One(), FLOAT_RAND * .25), (FLOAT_RAND * 1.75) + .25, col, 1);
            }
        }
    }   

    // check for gravity interactions
    // TODO

    // if player is dragging behind camera bounds, slow down camera until cant slowdown anymore, then speed up player
    // if the player is too fast for camera, speed up camera then slow down player if needed.

    if (THIS->position.x < cameraPosition.x - (cameraBounds.x - 1)) {
        THIS->position.x = cameraPosition.x - (cameraBounds.x - 1);
        THIS->velocity.x = cameraVelocity.x;
        cameraVelocity.x = CAMERA_COAST_SPEED;
    }
    else if (THIS->position.x > cameraPosition.x) {
        float delta = THIS->position.x - cameraPosition.x;
        delta /= cameraBounds.x;
        cameraVelocity.x = CAMERA_COAST_SPEED + (delta * ((FREE_MAX_SPEED - CAMERA_COAST_SPEED) + 1));
    }

    if (THIS->velocity.x > FREE_MAX_SPEED)
        THIS->velocity.x = FREE_MAX_SPEED;

    cameraPosition.x += cameraVelocity.x * DeltaTime;
    return 0;
}

int _Player_Draw(void* self, float DeltaTime) {
    RenderSpriteRelative(PLAYER_DATA->sprite, THIS->position, THIS->size, Vec2Angle(PLAYER_DATA->headingVector) - 180, WHITE);
    // RenderColliderRelative(THIS->position, THIS->radius); // Debug function for colliders
    return 0;
}

int _Player_Destroy(void* self, float DeltaTime) {
    DestroySprite(PLAYER_DATA->sprite);
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