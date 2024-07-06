#include "shield.h"

// imported by main.c

// ************************************************************
// ************************************************************
// ------------------------------------------------------------
// --- SHIELD OBJECT CODE SPACE ------------------ STARTING ---

// for lazy accesing the shield data object
#define SHIELD_DATA ((ShieldObject_Data *)(THIS->data_struct))



// ------------------------------------------------------------
// ------------------------------------------------------------

GameObj_Base* CreateShieldObject() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    printf("%s\n",">> creating shield object");

    obj_ptr->Init_Func = &_ShieldObject_Init;
    obj_ptr->Update_Func = &_ShieldObject_Update;
    obj_ptr->Draw_Func = &_ShieldObject_Draw;
    obj_ptr->Destroy_Func = &_ShieldObject_Destroy;

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_UNDEFINED_OBJ;

    obj_ptr->currentLayer = LAYER_GUI;

    obj_ptr->radius = 0;
    obj_ptr->mass = 100;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = Vector2One();
    
    return obj_ptr;
}

int _ShieldObject_Destroy(void* self, float DeltaTime) {
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.

    // free our data struct here. free anything contained.
    // free(SHIELD_DATA->sprite);
    free(SHIELD_DATA);

    return 0;
}



int _ShieldObject_Init(void* self, float DeltaTime) {
    // we have a reference to our own gameobject from which we can do things.
    // here we should create a reference to our datastructure and store it in the data_struct pointer.

    THIS->data_struct = malloc(sizeof(ShieldObject_Data)); 



    SHIELD_DATA->shieldHealth = 0.0f; // start none
    SHIELD_DATA->shieldRegenerationRate = SHIELD_REGEN_BASE_RATE;

    SHIELD_DATA->deltaTimeSinceLastImpact=0.0f;
    SHIELD_DATA->timeSinceLastShieldParticle = 0.0f;

    SHIELD_DATA->shieldMaxPull = 20.0f;
    SHIELD_DATA->shieldMaxRange = 10.0f;
    SHIELD_DATA->shieldMinRange = 0.5f;


    return 0;
}

int _ShieldObject_Update(void* self, float DeltaTime) {
    // set to mirror the player data
    _ShieldObject_MirrorPlayer(self, DeltaTime);

    SHIELD_DATA->deltaTimeSinceLastImpact += DeltaTime;
    SHIELD_DATA->timeSinceLastShieldParticle += DeltaTime;


    
    _ShieldObject_updateShieldHealth(self, DeltaTime);


    _ShieldObject_handleShieldEffect(self, DeltaTime);

    return 0;
}

int _ShieldObject_Draw(void* self, float DeltaTime) {
    // ibid

    // RenderSpriteRelative(SHIELD_DATA->sprite, THIS->position, THIS->size, sin(SHIELD_DATA->a)*90, WHITE);
    //RenderSquareRelative(THIS->position, THIS->size, 0, WHITE);

    return 0;
}



void _ShieldObject_MirrorPlayer(void *self, float DeltaTime){
    // ...
    THIS->position = PLAYER_OBJECT_REF->position;
    THIS->size = PLAYER_OBJECT_REF->size;
    
}



void _ShieldObject_handlePlayerDeath(void *self, float DeltaTime){
    // empty out the shield health
    SHIELD_DATA->shieldHealth = 0.0f;
}

void _ShieldObject_updateShieldHealth(void *self, float DeltaTime){

    // HANDLE THE REGEN

    // not max shields
    //  also been long enough since last impact
    if(_ShieldObject_GetPlayerShieldPercentage() < 1.0f && (SHIELD_DATA->deltaTimeSinceLastImpact >= SHIELD_REGEN_TIMEOUT_DELTATIME)){
        // if we're needing to regen shields
        (SHIELD_DATA->shieldHealth) += (SHIELD_DATA->shieldRegenerationRate)*DeltaTime;
    }

}


void _ShieldObject_handleShieldEffect(void *self, float DeltaTime){
    // how many particles we want per second
    float emitterParticlesPerDelta = (SHIELD_EMITTER_MAX_PARTICLE_PER_DELTA * _ShieldObject_GetPlayerShieldPercentage());

    // how long we want to wait per particle
    float deltaTimeToEmitParticle = 1.0f / emitterParticlesPerDelta;

    // check if it's been long enough
    if( SHIELD_DATA->timeSinceLastShieldParticle >= deltaTimeToEmitParticle){
        // emit a shield particle
        _ShieldObject_emitShieldParticle(self, DeltaTime);
    }
}
void _ShieldObject_emitShieldParticle(void *self, float DeltaTime){
    // reset particle emission timer
    SHIELD_DATA->timeSinceLastShieldParticle = 0.0f;
    
    // spawn a particle
    SpawnParticleEX(
        // v-- pos --v
        THIS->position,
        // v-- vel --v
        GetRandomUnitVector(),
        // v-- acc --v
        Vector2Zero(), 
        // v-- size --v
        Vector2Scale(
            Vector2One(),
            Lerp(SHIELD_PARTICLE_SCALE_MIN, SHIELD_PARTICLE_SCALE_MAX, FLOAT_RAND)
        ),
        // v-- lifetime --v
        Lerp(
            SHIELD_PARTICLE_LIFE_MIN,
            SHIELD_PARTICLE_LIFE_MAX,
            FLOAT_RAND
        ),
        // v-- color --v
        _ShieldObject_GetShieldParticleColor(),
        // v-- do outline --v
        true,
        // v-- do fadeout --v
        true,
        // v-- func_ptr --v
        _ShieldParticle_Update,
        // v-- func_data --v
        NULL 
        // --- -------- ---
    );
}






// ------------------------------------------------------------
// ------------------------------------------------------------

// remove it so we dont bloat namespace
#undef SHIELD_DATA

// --- SHIELD OBJECT CODE SPACE -------------------- ENDING ---
// ------------------------------------------------------------
// ************************************************************
// ************************************************************


float _ShieldObject_GetPlayerShieldPercentage(){
    // max
    if( (SHIELD_DATA_GLOBAL_ACCESS->shieldHealth) >= MAXIMUM_SHIELDS ) return 1.0f;
    // no shield or negative
    if( (SHIELD_DATA_GLOBAL_ACCESS->shieldHealth) < 0.0f ) return 0.0f;
    // otherwise
    return (SHIELD_DATA_GLOBAL_ACCESS->shieldHealth) / MAXIMUM_SHIELDS;
}


Color _ShieldObject_GetShieldParticleColor(){
    int diceRoll = (INT_RAND%4);
    switch (diceRoll) {
        default:
        case 0: return SHIELD_PARTICLE_COLOUR_0;
        case 1: return SHIELD_PARTICLE_COLOUR_1;
        case 2: return SHIELD_PARTICLE_COLOUR_2;
        case 3: return SHIELD_PARTICLE_COLOUR_3;
    }
}

// returns the amount that wasnt absorbed by shields
float _ShieldObject_TakeDamage(float rawDamage){
    // say none
    float spillOverDamage = 0.0f;
    // less than or equal remaining
    if(rawDamage <= SHIELD_DATA_GLOBAL_ACCESS->shieldHealth){
        // not enough to spill, leave spill 0 and remove shield health
        SHIELD_DATA_GLOBAL_ACCESS->shieldHealth -= rawDamage;
    }
    // more than the health we have remaining, spill over
    else {
        // keep the part not absorbed by shields
        spillOverDamage = rawDamage - (SHIELD_DATA_GLOBAL_ACCESS->shieldHealth);
        // empty out the shields
        SHIELD_DATA_GLOBAL_ACCESS->shieldHealth = 0.0f;
    }
    // reset time since impact timer
    SHIELD_DATA_GLOBAL_ACCESS->deltaTimeSinceLastImpact = 0.0f;
    // give it back
    return spillOverDamage;
}


// ************************************************************



int _ShieldParticle_Update(void *self, float DeltaTime){
    _Particle *particleObj = ((_Particle *)self);


    

    Vector2 vectorTowardsPlayer = Vector2Subtract(SHIELD_OBJECT_REF->position, particleObj->position);
    // get the accel
    Vector2 particleAcceleration = _ShieldParticle_GetShieldParticleAccelerationToPlayer(particleObj);
    // and dist
    float distanceToParticle = Vector2Length(vectorTowardsPlayer);

    // repel if we're less than minimum range
    float minShieldRange = SHIELD_DATA_GLOBAL_ACCESS->shieldMinRange;
    if( distanceToParticle <  minShieldRange){
        // when we're closer than minimum, we want to leave the area
        float percentageOfInnerDistance = 1.0f-(distanceToParticle/minShieldRange);
        // inverse the acceleration to repel
        Vector2 newAccel = Vector2Negate(Vector2Scale(particleAcceleration, percentageOfInnerDistance));
        // randomly rotate the acceleration vector
        float rotationCone = 90.0f;
        float randomRotation = FLOAT_RAND*(rotationCone-(rotationCone/2.0f));
        Vector2 rotatedAccel = Vector2Rotate(newAccel,randomRotation);
        // check which is pointing more perpendicular to the direction of the player
        //  we want the one that's closest to 0 to be perpendicular
        float repelPerpendicularityToVectorToPlayer = fabsf(Vector2DotProduct(vectorTowardsPlayer, newAccel));
        float rotatedPerpendicularityToVectorToPlayer = fabsf(Vector2DotProduct(vectorTowardsPlayer, rotatedAccel));

        if(repelPerpendicularityToVectorToPlayer<=rotatedPerpendicularityToVectorToPlayer){
            // but also make it less effective the less it's perpendicular
            particleAcceleration = Vector2Scale(newAccel, 1.0f-repelPerpendicularityToVectorToPlayer);
        }
        else {
            // but also make it less effective the less it's perpendicular
            particleAcceleration = Vector2Scale(rotatedAccel, 1.0f-rotatedPerpendicularityToVectorToPlayer);
        }
        
    }
    
    // apply to particle
    particleObj->acceleration = particleAcceleration;
    
    // but also get perpendicular
    // but also quick apply it to get the over correction
    particleObj->velocity = Vector2Add(particleObj->velocity, Vector2Scale(particleAcceleration, DeltaTime));
    
    return 0;
}

// chopped up version of the star gravity interaction
Vector2 _ShieldParticle_GetShieldParticleAccelerationToPlayer(_Particle *particleObj){

    float dist = Vector2Distance(SHIELD_OBJECT_REF->position, particleObj->position);

    if (dist > (SHIELD_DATA_GLOBAL_ACCESS->shieldMaxRange)) return Vector2Zero();

    float delta = dist/(SHIELD_DATA_GLOBAL_ACCESS->shieldMaxRange);
    delta = 1-delta; // inverse it to get 1 when closest.
    delta = delta * delta; // square it to get it reducing furher away

    Vector2 ang = Vector2Subtract(SHIELD_OBJECT_REF->position, particleObj->position);
    ang = Vector2Normalize(ang);
    ang = Vector2Scale(ang, delta * (SHIELD_DATA_GLOBAL_ACCESS->shieldMaxPull) );

    return ang;
}


