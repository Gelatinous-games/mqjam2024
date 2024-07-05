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

#ifndef _star_obj
    #define _star_obj
    #include "star.c"
#endif

#define MAXIMUM_HEALTH 100
#define CAMERA_COAST_SPEED 3

typedef struct {
    Sprite* sprite;
    Vector2 headingVector;
    float rotateJerk;
    float rotateRate;
    float accelRate;
    float health;
    float healthRegenerationRate;
} Player_Data;

#define PLAYER_DATA ((Player_Data *)(THIS->data_struct))

int CURRENT_PLAYER_STATE = PLAYER_STATE_NOTHRUST;

// declare functions we're adding so they can be used
void updateThrustingSoundState(void *self, float DeltaTime);
void handleAsteroidCollistions(void *self, float DeltaTime);
void handleGravityInteractions(void *self, float DeltaTime);
void handleStarProximityAlert(GameObj_Base *self, GameObj_Base *extobj);
void handlePlayerMovement(void *self, float DeltaTime);
void constrainPlayerToCamera(void *self, float DeltaTime);
void handleCameraRelativity(void *self, float DeltaTime);
void updateHealth(void *self, float DeltaTime);


int _Player_Init(void* self, float DeltaTime) {
    PLAYER_DATA->headingVector = (Vector2) { 1, 0 };
    PLAYER_DATA->health = MAXIMUM_HEALTH; // start max
    PLAYER_DATA->healthRegenerationRate = 10.0f;
    PLAYER_DATA->rotateRate = 0; // velocity for rotation. affected by rotatejerk.
    PLAYER_DATA->rotateJerk = 3; // veryy low.
    PLAYER_DATA->accelRate = 6.5; // 5u/s

    cameraVelocity.x = CAMERA_COAST_SPEED;

    PLAYER_DATA->sprite = CreateSprite("resources/spaceship_outlined.png");
    return 0;
}

int _Player_Update(void* self, float DeltaTime) {
    // ================
    updateThrustingSoundState(self, DeltaTime);

    handlePlayerMovement(self, DeltaTime);

    constrainPlayerToCamera(self, DeltaTime);
    
    handleAsteroidCollistions(self, DeltaTime);

    handleGravityInteractions(self, DeltaTime);

    handleCameraRelativity(self, DeltaTime);

    updateHealth(self, DeltaTime);
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

void updateThrustingSoundState(void *self, float DeltaTime){

    // when starting to thrust
    float trackSettingVolume = 0.0f;
    if (
        IsKeyDown(KEY_W) ||
        IsKeyDown(KEY_A) ||
        IsKeyDown(KEY_S) ||
        IsKeyDown(KEY_D)
    ) {
        // forward backwards max, just rotate is half volume
        if(IsKeyDown(KEY_W)||IsKeyDown(KEY_S)){
            // full volume
            trackSettingVolume = 1.0;
        }else {
            // half
            trackSettingVolume = 0.5;
        }
        // is it new OR were we ending
        if( CURRENT_PLAYER_STATE < PLAYER_STATE_STARTTHRUST ){
            CURRENT_PLAYER_STATE=PLAYER_STATE_STARTTHRUST;
            setTrackVolume(THRUST_END_ID, 0.0f);
            setTrackVolume(THRUST_START_ID, trackSettingVolume);
            playSoundOnce(THRUST_START_ID);
        }
        // check start sound finished
        else if(CURRENT_PLAYER_STATE==PLAYER_STATE_STARTTHRUST && !IsSoundPlaying(TRACKS[THRUST_START_ID]->track)){
            // swap volume to loop
            setTrackVolume(THRUST_START_ID, 0.0f);
            setTrackVolume(THRUST_LOOP_ID, trackSettingVolume);
            // change state
            CURRENT_PLAYER_STATE=PLAYER_STATE_THRUSTING;
        }
    }
    else
    {
        // STARTING OR LOOPING
        if(CURRENT_PLAYER_STATE>PLAYER_STATE_STOPTHRUST){
            // turn on only the end sound
            setTrackVolume(THRUST_END_ID, 1.0f);
            setTrackVolume(THRUST_START_ID, 0.0f);
            setTrackVolume(THRUST_LOOP_ID, 0.0f);
            // but also play it
            playSoundOnce(THRUST_END_ID);
            CURRENT_PLAYER_STATE=PLAYER_STATE_STOPTHRUST;
        }
        // STOPPING and finished playing
        else {
            // 0 volume all of them
            setTrackVolume(THRUST_END_ID, 0.0f);
            setTrackVolume(THRUST_START_ID, 0.0f);
            setTrackVolume(THRUST_LOOP_ID, 0.0f);
            // change state to no thrust
            CURRENT_PLAYER_STATE=PLAYER_STATE_NOTHRUST;
        }
    }
}

void handleAsteroidCollistions(void *self, float DeltaTime){

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
}


void handleGravityInteractions(void *self, float DeltaTime){

    // check for gravity interactions
    GameObj_Base* extobj;
    // TODO
    for (int sIDX = 0; sIDX != -1; ) {
        sIDX = GetObjectWithFlagsExact(FLAG_GRAVITY_WELL, sIDX, &extobj);

        if (sIDX == -1) break;

        Vector2 impartSelf, impartStar;
        // Check if collision occurs
        if (GetCollided(THIS, extobj, &impartSelf, &impartStar)) {
            PLAYER_DATA->health -= MAXIMUM_HEALTH;

            // END STATE

            //create a billion particles as the ship disintegrates
        }
        else {
            handleStarProximityAlert(THIS,extobj);
            
            // apply gravity vector
            Vector2 accel = GetAccelerationToSink(extobj, THIS);
            THIS->velocity = Vector2Add(THIS->velocity, Vector2Scale(accel, DeltaTime));
        }
    }
}

void handleStarProximityAlert(GameObj_Base *self, GameObj_Base *extobj){
    // grab our information
    float distanceFromStar = Vector2Distance(THIS->position, extobj->position);
    float starRange = ((_Star_Data *)(extobj->data_struct))->maxRange;
    // when we're 
    float proximity = 1.0f-((distanceFromStar) / starRange);
    // printf("unclamp proximity: %f\n", proximity);
    if (proximity > 1.0f) proximity = 1.0f;
    if (proximity < 0.0f) proximity = 0.0f;
    // printf("clamped proximity: %f\n", proximity);
    setTrackVolume(STAR_PROXIMITY_LOOP_ID, proximity);
}

void handlePlayerMovement(void *self, float DeltaTime){

    // ================
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
}

void constrainPlayerToCamera(void *self, float DeltaTime){
    // bind position within camerabounds y
    if (THIS->position.y > cameraBounds.y) {
        THIS->position.y = cameraBounds.y;
        THIS->velocity.y = 0;
    }
    else if (THIS->position.y < -cameraBounds.y) {
        THIS->position.y = -cameraBounds.y;
        THIS->velocity.y = 0;
    }
}

void handleCameraRelativity(void *self, float DeltaTime){

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
}

void updateHealth(void *self, float DeltaTime){
    // not max health
    if(PLAYER_DATA->health < MAXIMUM_HEALTH){
        // just increase it by the regen rate every second
        (PLAYER_DATA->health) += (PLAYER_DATA->healthRegenerationRate)*DeltaTime;
    }
    // spooky over heal
    else if(PLAYER_DATA->health > MAXIMUM_HEALTH){
        // TODO: shield?
        (PLAYER_DATA->health) = MAXIMUM_HEALTH;

    }
}