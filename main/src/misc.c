#include "raylib.h"
#include "raymath.h"
#include "base.h"

#include "obj_register.h"

#define COLLIDE_MAX_SPEED 3
#define FREE_MAX_SPEED 10

/// @brief Returns 1 if key A is down and not B, -1 if B and not A, and 0 if neither.
/// @param A 
/// @param B 
/// @return 
int GetKeyDelta(int A, int B) {
    if (IsKeyDown(A) && !IsKeyDown(B)) return 1;
    else 
    if (IsKeyDown(B) && !IsKeyDown(A)) return -1;
    else
    return 0;
}

/// @brief Converts a vector to a degree angle.
/// @param a 
/// @return 
float Vec2Angle(Vector2 a) {
    return atan2(-a.y, -a.x) * RAD2DEG;
}

float Vec2Dist(Vector2 a, Vector2 b) {
    Vector2 d = Vector2Subtract(b, a);
    return Vector2Length(d);
}

/// @brief Returns 1 if the two objects A & B collided, and imparted velocities on the two should they.
/// Imparted velocities are calculated according to the sum of the effective velocity between the two, the exact angle, and the mass distribution.
/// @param A Object A
/// @param B Object B
/// @param impartA The velocity imparted onto object A
/// @param impartB The velocity imparted onto object B
/// @return 
char GetCollided(GameObj_Base* A, GameObj_Base* B, Vector2* impartA, Vector2* impartB) {
    float dist = Vec2Dist(A->position, B->position);
    // printf("dist is %f to sum radius %f (%f + %f)\n", dist, A->radius + B->radius, A->radius, B->radius);
    if (dist >= (fabsf(A->radius) + fabsf(B->radius))) return 0;

    // THE MESSY TASK OF CALCULATING IMPARTED VELOCITIES.

    // one is player
    if((A->flags)&FLAG_PLAYER_OBJECT|(B->flags)&FLAG_PLAYER_OBJECT){
        // player impacting somehting - not star
        if(!((A->flags)&FLAG_GRAVITY_WELL|(B->flags)&FLAG_GRAVITY_WELL)){
            playSoundOnce(HIT_SOUND_ID);
        }
    }

    if ((A->flags)&FLAG_ASTEROID)
    {
        // printf("%s\n","A is asteroid");
    }
    if ((B->flags)&FLAG_ASTEROID)
    {
        // printf("%s\n","B is asteroid");
    }

    if ((A->flags)&(B->flags)&FLAG_ASTEROID)
    {
        // printf("%s\n","asteroid to asteroid collision");
    }
    

    // figure out the angles from each to each other
    Vector2 angle_A_B = Vector2Subtract(B->position, A->position);
    Vector2 angle_B_A = Vector2Subtract(A->position, B->position);

    // calculate effective velocities of both as they impact, and sum together to get the effective collision velocity
    Vector2 vel_Eff_A = Vector2Multiply(A->velocity, angle_A_B);
    Vector2 vel_Eff_B = Vector2Multiply(B->velocity, angle_B_A);
    Vector2 vel_Effective = Vector2Add(vel_Eff_A, vel_Eff_B);
    float effectiveVelocity = Vector2Length(vel_Effective);
    if (effectiveVelocity > COLLIDE_MAX_SPEED)
        effectiveVelocity = COLLIDE_MAX_SPEED;

    // calculate the system mass, and figure out the distributions.
    float sys_Mass = A->mass + B->mass;
    float ratio_A = B->mass / sys_Mass;
    float ratio_B = A->mass / sys_Mass;

    // multiply those ratios by the effective velocity and scale it to the angle from the other object to this to get our imparted velocity at impact.
    // i think?
    *impartA = Vector2Scale(angle_B_A, ratio_A * effectiveVelocity);
    *impartB = Vector2Scale(angle_A_B, ratio_B * effectiveVelocity);

    return 1;
}