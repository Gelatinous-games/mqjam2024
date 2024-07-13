#include "misc_util.h"



unsigned long long currMillis(){
    struct timeval timeVal;
    unsigned long long millisecondsSinceEpoch;

    gettimeofday(&timeVal, NULL);

    millisecondsSinceEpoch =
        (unsigned long long)(timeVal.tv_sec) * 1000 +
        (unsigned long long)(timeVal.tv_usec) / 1000;
    
    return millisecondsSinceEpoch;
}

Vector2 GetRandomUnitVector(){
    float angle = FLOAT_RAND*360.0;
    return Vector2Rotate((Vector2){ 1.0f, 0.0f}, angle);
}






/// @brief Returns 1 if key A is down and not B, -1 if B and not A, and 0 if neither.
/// @param A 
/// @param B 
/// @return 
int GetKeyDelta(int A, int B) {
    if (IsKeyDown(A) && !IsKeyDown(B)){
        setTrackVolume(THRUST_LOOP_ID, 1.0f);
        return 1;
    }
    else 
    if (IsKeyDown(B) && !IsKeyDown(A)){
        setTrackVolume(THRUST_LOOP_ID, 1.0f);
        return -1;
    }
    else{
        // ..
        setTrackVolume(THRUST_LOOP_ID, 0.0f);
        return 0;
    }
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
    Vector2 aPosition = Vector2Add(A->position,(Vector2){0.000001f,0.000001f});
    Vector2 bPosition = Vector2Add(B->position,(Vector2){-0.000001f,-0.000001f});
    
    float dist = Vec2Dist(aPosition, bPosition);
    // printf("dist is %f to sum radius %f (%f + %f)\n", dist, A->radius + B->radius, A->radius, B->radius);
    if (dist >= (fabsf(A->radius) + fabsf(B->radius))) return 0;

    // THE MESSY TASK OF CALCULATING IMPARTED VELOCITIES.

    // figure out the angles from each to each other
    Vector2 angle_A_B = Vector2Subtract(bPosition, aPosition);
    Vector2 angle_B_A = Vector2Subtract(aPosition, bPosition);

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