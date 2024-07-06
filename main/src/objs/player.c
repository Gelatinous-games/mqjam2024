#include "player.h"

// for just in this file
#define PLAYER_DATA ((Player_Data *)(THIS->data_struct))


int _Player_Init(void* self, float DeltaTime) {
    PLAYER_DATA->deltaTimeSinceLastImpact=0.0f;


    PLAYER_DATA->hullHealth = MAXIMUM_HULL; // start max
    PLAYER_DATA->hullRegenerationRate = HULL_REGEN_BASE_RATE;

    PLAYER_DATA->headingVector = (Vector2) { 1, 0 };
    PLAYER_DATA->rotateRate = 0; // velocity for rotation. affected by rotatejerk.
    PLAYER_DATA->rotateJerk = 3; // veryy low.
    PLAYER_DATA->accelRate = 6.5; // 5u/s



    cameraVelocity.x = CAMERA_COAST_SPEED;

    PLAYER_DATA->sprite = CreateSprite("resources/spaceship_outlined.png");
    return 0;
}

int _Player_Update(void* self, float DeltaTime) {
    // INCREASE TIMER SINCE LAST IMPACT
    PLAYER_DATA->deltaTimeSinceLastImpact += DeltaTime;

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
        if( CURRENT_PLAYER_THRUST_STATE < PLAYER_STATE_STARTTHRUST ){
            CURRENT_PLAYER_THRUST_STATE=PLAYER_STATE_STARTTHRUST;
            setTrackVolume(THRUST_END_ID, 0.0f);
            setTrackVolume(THRUST_START_ID, trackSettingVolume);
            playSoundOnce(THRUST_START_ID);
        }
        // check start sound finished
        else if(CURRENT_PLAYER_THRUST_STATE==PLAYER_STATE_STARTTHRUST && !IsSoundPlaying(TRACKS[THRUST_START_ID]->track)){
            // swap volume to loop
            setTrackVolume(THRUST_START_ID, 0.0f);
            setTrackVolume(THRUST_LOOP_ID, trackSettingVolume);
            // change state
            CURRENT_PLAYER_THRUST_STATE=PLAYER_STATE_THRUSTING;
        }
    }
    else
    {
        // STARTING OR LOOPING
        if(CURRENT_PLAYER_THRUST_STATE>PLAYER_STATE_STOPTHRUST){
            // turn on only the end sound
            setTrackVolume(THRUST_END_ID, 1.0f);
            setTrackVolume(THRUST_START_ID, 0.0f);
            setTrackVolume(THRUST_LOOP_ID, 0.0f);
            // but also play it
            playSoundOnce(THRUST_END_ID);
            CURRENT_PLAYER_THRUST_STATE=PLAYER_STATE_STOPTHRUST;
        }
        // STOPPING and finished playing
        else {
            // 0 volume all of them
            setTrackVolume(THRUST_END_ID, 0.0f);
            setTrackVolume(THRUST_START_ID, 0.0f);
            setTrackVolume(THRUST_LOOP_ID, 0.0f);
            // change state to no thrust
            CURRENT_PLAYER_THRUST_STATE=PLAYER_STATE_NOTHRUST;
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
            // ... collision sound
            playSoundOnce(HIT_SOUND_ID);

            Vector2 midPoint = Vector2Scale(Vector2Add(THIS->position, extobj->position), 0.5);
            THIS->velocity = Vector2Add(THIS->velocity, impartSelf);
            extobj->velocity = Vector2Add(extobj->velocity, impartAsteroid);

            // get the damage rate and apply
            PlayerTakeDamage(self, DeltaTime, ASTEROID_IMPACT_DAMMAGE_HULL, ASTEROID_IMPACT_DAMMAGE_SHIELDED);

            // create a spark effect or something

            int randomCount = (FLOAT_RAND * 4) + 4;
            for (int i = 0; i < randomCount; i++) {
                // pick a palette of 4
                Color colourVal = GetImpactParticleColor();
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
            // ... collided
            playSoundOnce(HIT_SOUND_ID);

            PlayerTakeDamage(self, DeltaTime, STAR_IMPACT_DAMMAGE_HULL, STAR_IMPACT_DAMMAGE_SHIELDED);


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
    // when hull is gone
    if(GetPlayerHullPercentage()<=0.0f){
        CURRENT_PLAYER_LIFE_STATE = PLAYER_LIFE_STATUS_ISDEAD;

        _ShieldObject_handlePlayerDeath((void *)SHIELD_OBJECT_REF, DeltaTime);
    }
    // otherwise handle regens
    else {
        // HANDLE THE STATE
        if(_ShieldObject_GetPlayerShieldPercentage() < MINIMUM_SHIELD_PERCENT){
            CURRENT_PLAYER_LIFE_STATE = PLAYER_LIFE_STATUS_ISHULL;
        }
        else if(_ShieldObject_GetPlayerShieldPercentage() >= MINIMUM_SHIELD_PERCENT){
            CURRENT_PLAYER_LIFE_STATE = PLAYER_LIFE_STATUS_ISSHIELDED;
        }

        // HANDLE THE REGEN

        // not max health
        if(GetPlayerHullPercentage() < 1.0f){
            // just increase it by the regen rate every second
            (PLAYER_DATA->hullHealth) += (PLAYER_DATA->hullRegenerationRate)*DeltaTime;
        }
    }

}



float GetPlayerHullPercentage(){
    // current
    float currHealth = (((Player_Data *)PLAYER_OBJECT_REF->data_struct)->hullHealth);
    // max or shielded
    if(currHealth>=MAXIMUM_HULL) return 1.0f;
    // dead or negative
    if(currHealth<0.0f) return 0.0f;
    // otherwise
    return currHealth / MAXIMUM_HULL;

}

Vector2 GetRandomUnitVector(){
    float angle = FLOAT_RAND*360.0;
    return Vector2Rotate((Vector2){ 1.0f, 0.0f}, angle);
}

Color GetHullParticleColor(){
    int diceRoll = (INT_RAND%4);
    switch (diceRoll) {
        default:
        case 0: return(Color) {245, 141, 38, 127};
        case 1: return(Color) {38, 217, 245, 127};
        case 2: return(Color) {252, 233, 112, 127};
        case 3: return(Color) {216, 214, 203, 127};
    }
}

Color GetImpactParticleColor(){
    if(CURRENT_PLAYER_LIFE_STATE == PLAYER_LIFE_STATUS_ISSHIELDED){
        return _ShieldObject_GetShieldParticleColor();
    }
    return GetHullParticleColor();
}

void PlayerTakeDamage(void *self, float DeltaTime, int hullRate, int shieldRate){
    // TODO: shake the health bar

    // reset the time since counter
    PLAYER_DATA->deltaTimeSinceLastImpact = 0.0f;

    float percentageNotAbsorbedByShields = _ShieldObject_TakeDamage(shieldRate) / ((float)shieldRate);
    PLAYER_DATA->hullHealth -= hullRate * percentageNotAbsorbedByShields;

    // else {
    //     ... 
    //     dead ghost debris taking damage from something?
    // }
}








// remove it from existence
#undef PLAYER_DATA