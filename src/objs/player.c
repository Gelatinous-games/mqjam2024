#include "player.h"

// for just in this file
#define PLAYER_DATA ((Player_Data *)(THIS->data_struct))
// #define DEBUG_ON

int _Player_Init(void* self, float DeltaTime) {
    PLAYER_DATA->deltaTimeSinceLastImpact=0.0f;

    cameraPosition = Vector2Zero();
    PLAYER_DATA->hullHealth = MAXIMUM_HULL; // start max
    PLAYER_DATA->hullRegenerationRate = HULL_REGEN_BASE_RATE;

    PLAYER_DATA->headingVector = (Vector2) { 1, 0 };
    PLAYER_DATA->rotateRate = 0; // velocity for rotation. affected by rotatejerk.
    PLAYER_DATA->rotateJerk = 3; // veryy low.
    PLAYER_DATA->accelRate = 6.5; // 5u/s

    PLAYER_DATA->damageImmunity = 0;

    setTrackVolume(STAR_PROXIMITY_LOOP_ID, 0);

    cameraVelocity.x = CAMERA_COAST_SPEED;
    

    CURRENT_PLAYER_THRUST_STATE = PLAYER_STATE_NOTHRUST;
    CURRENT_PLAYER_LIFE_STATE = PLAYER_LIFE_STATUS_ISHULL;
    return 0;
}

int _Player_Update(void* self, float DeltaTime) {
    // INCREASE TIMER SINCE LAST IMPACT
    PLAYER_DATA->deltaTimeSinceLastImpact -= DeltaTime;

    // ================
    if(playerCanControl) updateThrustingSoundState(self, DeltaTime);
    
    if (!IsPlayerAlive() || !playerCanControl) return 0;

    handlePlayerMovement(self, DeltaTime);

    constrainPlayerToCamera(self, DeltaTime);
    
    handleAsteroidCollistions(self, DeltaTime);

    handleGravityInteractions(self, DeltaTime);

    handleCameraRelativity(self, DeltaTime);

    updateHealth(self, DeltaTime);
    
    #ifdef DEBUG_ON
        if (IsKeyDown(KEY_K))
            PLAYER_DATA->lastDamageSourceFlag = FLAG_GUI;
            PLAYER_DATA->hullHealth = -1;
    #endif

    //

    return 0;
}

int _Player_Draw(void* self, float DeltaTime) {
    // if (CURRENT_PLAYER_LIFE_STATE != PLAYER_LIFE_STATUS_ISDEAD)
    if (IsPlayerAlive())
        RenderSpriteRelative(_SpriteLibrary_Player_ShipSprite, THIS->position, THIS->size, Vec2Angle(PLAYER_DATA->headingVector) - 180, WHITE);
    // RenderColliderRelative(THIS->position, THIS->radius); // Debug function for colliders

    // hwat the henk is a gcvt??
    // #ifdef DEBUG_ON
    //     char dt_buff[128];
    //     gcvt(THIS->position.x, 10, dt_buff);
    //     DrawText(dt_buff, GetScreenWidth()/2, 0, 32, WHITE);
    // #endif

    return 0;
}

int _Player_Destroy(void* self, float DeltaTime) {
    free(PLAYER_DATA);
    THIS->data_struct = 0;
    
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
    _PLAYER_CauseOfDeath = -1;

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
            Vector2 midPoint = Vector2Scale(Vector2Add(THIS->position, extobj->position), 0.5);
            THIS->velocity = Vector2Add(THIS->velocity, impartSelf);
            extobj->velocity = Vector2Add(extobj->velocity, impartAsteroid);

            if (PLAYER_DATA->deltaTimeSinceLastImpact > 0) {
                continue;
            }
            // FIXME: spooky bad, use flags
            //  possible segfault here
            Asteroid_Data *asteroidData;
            asteroidData = ((Asteroid_Data *)(extobj->data_struct));
            // FIXME: damage should be logarithm of mass or something times the damage rate
            // float asteroidTonnage = extobj->mass/1000.0f;
            if(asteroidData->isIcyComet){
                // comet actually
                // get the damage rate and apply
                // slightly lower shield damage to slightly more hull damage
                PlayerTakeDamage(self, DeltaTime, ASTEROID_IMPACT_DAMMAGE_HULL*1.1f, ASTEROID_IMPACT_DAMMAGE_SHIELDED*0.7f, extobj->flags);
            }
            else{
                // regular ahh asteroid
                // get the damage rate and apply
                PlayerTakeDamage(self, DeltaTime, ASTEROID_IMPACT_DAMMAGE_HULL, ASTEROID_IMPACT_DAMMAGE_SHIELDED, extobj->flags);
            }

            // create a spark effect or something
            int randomCount = 2 * ((FLOAT_RAND * 8) + 8);
            for (int i = 0; i < randomCount; i++) {
                
                Color colourVal;
                GetImpactParticleColor(&colourVal);

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

            PlayerTakeDamage(self, DeltaTime, STAR_IMPACT_DAMMAGE_HULL, STAR_IMPACT_DAMMAGE_SHIELDED, extobj->flags);


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
        float playerVelocityDisplacement = Vector2Length(THIS->velocity);
        Vector2 playerHeading = Vector2Normalize(PLAYER_DATA->headingVector);

        // perpendicular vector
        Vector2 perpendicularVector = (Vector2) { -playerHeading.y, playerHeading.x };
        Vector2 playerPos = THIS->position;

        Vector2 modelOrigin = playerPos;
        // check if backward, then only move a little from center, otherwise, we move back half the model
        float forwardBackwardScale = (accel_delta == -1) ? 0.05 : -0.5;
        modelOrigin = Vector2Add(modelOrigin, Vector2Scale(playerHeading, forwardBackwardScale));

        // positive accel, negate the heading for later calculations
        if (accel_delta == 1) playerHeading = Vector2Negate(playerHeading);

        // for the variance
        Color trailColor;

        _PALETTE_Player_ParticleColor_Trail(&trailColor);
        SpawnParticle(
            Vector2Add(modelOrigin, Vector2Scale(perpendicularVector, -0.225)),
            Vector2Add(Vector2Scale(playerHeading, playerVelocityDisplacement + (accel_delta * DeltaTime * PLAYER_DATA->accelRate)), Vector2Scale(perpendicularVector, (FLOAT_RAND-0.5)*0.5)),
            Vector2Zero(),
            (Vector2) {0.0625, 0.0625},
            0.5,
            trailColor,
            (FLOAT_RAND * 1) + 0.5
        );

        _PALETTE_Player_ParticleColor_Trail(&trailColor);
        SpawnParticle(
            Vector2Add(modelOrigin, Vector2Scale(perpendicularVector, +0.225)),
            Vector2Add(Vector2Scale(playerHeading, playerVelocityDisplacement + (accel_delta * DeltaTime * PLAYER_DATA->accelRate)), Vector2Scale(perpendicularVector, (FLOAT_RAND-0.5)*0.5)),
            Vector2Zero(),
            (Vector2) {0.0625, 0.0625},
            0.5,
            trailColor,
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
        // vector from center to player
        float delta = THIS->position.x - cameraPosition.x;
        // 0.5f limits the amount we can travel past center
        delta /= (cameraBounds.x*0.5f);
        cameraVelocity.x = CAMERA_COAST_SPEED + (delta * ((FREE_MAX_SPEED - CAMERA_COAST_SPEED) + 1));
    }

    if (THIS->velocity.x > FREE_MAX_SPEED)
        THIS->velocity.x = FREE_MAX_SPEED;

    cameraPosition.x += cameraVelocity.x * DeltaTime;
}

void updateHealth(void *self, float DeltaTime){
    if (!IsPlayerAlive()) return;

    // when hull is gone
    if(GetPlayerHullPercentage()<=0.0f ){
        _PLAYER_HandleDeath(self,DeltaTime);
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


Color *GetImpactParticleColor( Color *dest ){
    if(CURRENT_PLAYER_LIFE_STATE == PLAYER_LIFE_STATUS_ISSHIELDED){
        return _PALETTE_Player_ParticleColor_ShieldCollision(dest);
    }
    return _PALETTE_Player_ParticleColor_HullCollision(dest);
}

void PlayerTakeDamage(void *self, float DeltaTime, int hullRate, int shieldRate, int flagOfDamageSource){
    // TODO: shake the health bar
    if(IsPlayerAlive()){
        // ...
        // reset the time since counter
        if (PLAYER_DATA->deltaTimeSinceLastImpact > 0) {
            return;
        }
        // otherwise
        PLAYER_DATA->lastDamageSourceFlag = flagOfDamageSource;
        PLAYER_DATA->deltaTimeSinceLastImpact = PLAYER_DAMAGE_IMMUNITY_TIMEOUT; // set to the timout

        float percentageNotAbsorbedByShields = _ShieldObject_TakeDamage(shieldRate) / ((float)shieldRate);

        if(CURRENT_PLAYER_LIFE_STATE != PLAYER_LIFE_STATUS_ISSHIELDED){
            SoundManagerHandleImpact(HULL_IMPACT);
        }
        else {
            SoundManagerHandleImpact(SHIELD_IMPACT);
        }

        PLAYER_DATA->hullHealth -= hullRate * percentageNotAbsorbedByShields;
    }
    else {
        // ... 
        // dead ghost debris taking damage from something?
        SoundManagerHandleImpact(DEATH_IMPACT);
    }
}




void _PLAYER_HandleDeath(void *self, float DeltaTime){
    _PLAYER_CauseOfDeath = PLAYER_DATA->lastDamageSourceFlag;

    // also handle death
    SoundManagerHandleDeath();

    CURRENT_PLAYER_LIFE_STATE = PLAYER_LIFE_STATUS_ISDEAD;

    _ShieldObject_handlePlayerDeath((void *)SHIELD_OBJECT_REF, DeltaTime);

    // create a scatter of particles

    for (int i = 0; i < 64; i++) {
        Color debrisColour;
        _PALETTE_Player_ParticleColor_Debris(&debrisColour);

        SpawnParticle(
            Vector2Add(THIS->position, (Vector2) { (FLOAT_RAND * 1) - 0.5, (FLOAT_RAND * 1) - 0.5}),
            Vector2Add(THIS->velocity, (Vector2) { (FLOAT_RAND * 1) - 0.5, (FLOAT_RAND * 1) - 0.5}),
            Vector2Zero(), (Vector2) { 0.125, 0.125 }, 5, debrisColour, 1);
    }

    
}






// returns 0 when dead
//  non zero otherwise, which is treated as true
int IsPlayerAlive(){
    
    return (CURRENT_PLAYER_LIFE_STATE != PLAYER_LIFE_STATUS_ISDEAD);
}

// remove it from existence
#undef PLAYER_DATA






