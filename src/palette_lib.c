#include "palette_lib.h"

/// @brief get random debris particle colour
/// @param dest already allocated pointer to colour var we change
/// @return dest
Color *_PALETTE_Player_ParticleColor_Debris( Color *dest ){
    // [0,1,2]
    int rng = INT_RAND % 3;
    switch (rng) {
        default:
        case 0:
            *dest = (Color) {132,132,132,127};
            break;
        case 1:
            *dest = (Color) {75,75,75,127};
            break;
        case 2:
            *dest = (Color) {200,200,200,127};
            break;
    }
    return dest;
}

/// @brief get random trail particle colour
/// @param dest already allocated pointer to colour var we change
/// @return dest
Color *_PALETTE_Player_ParticleColor_Trail( Color *dest ){
    // // [0]
    // int rng = INT_RAND % 1;
    int rng = 0;
    switch (rng) {
        default:
        case 0:
            *dest = (Color) {255, 127, 0, 127 };
            break;
    }
    return dest;
}

/// @brief get random hull collision particle colour
/// @param dest already allocated pointer to colour var we change
/// @return dest
Color *_PALETTE_Player_ParticleColor_HullCollision( Color *dest ){
    // [0,1,2,3]
    int rng = INT_RAND % 4;
    switch (rng) {
        default:
        case 0:
            *dest = (Color) {245, 141, 38, 127};
            break;
        case 1:
            *dest = (Color) {38, 217, 245, 127};
            break;
        case 2:
            *dest = (Color) {252, 233, 112, 127};
            break;
        case 3:
            *dest = (Color) {216, 214, 203, 127};
            break;
    }
    return dest;
}

/// @brief get random shield collision particle colour
/// @param dest already allocated pointer to colour var we change
/// @return dest
Color *_PALETTE_Player_ParticleColor_ShieldCollision( Color *dest ){
    // [0,1,2,3]
    int rng = INT_RAND % 4;
    switch (rng) {
        default:
        case 0:
            *dest = (Color){  34, 195, 222, 127 };
            break;
        case 1:
            *dest = (Color){  27, 118, 213, 127 };
            break;
        case 2:
            *dest = (Color){  34, 107, 220, 127 };
            break;
        case 3:
            *dest = (Color){  25,  66, 219, 127 };
            break;
    }
    return dest;
}


/// @brief get random asteroid collision particle colour
/// [replaces]: GetAsteroidParticleColor()
/// @param dest already allocated pointer to colour var we change
/// @return dest
Color *_PALETTE_Asteroid_ParticleColor_Collision( Color *dest, int isIcyComet ){
    // [0,1,2,3]
    int rng = INT_RAND % 4;
    if(isIcyComet){
        // icy comet
        switch (rng) {
            default:
            case 0:
                *dest = (Color) {189, 222, 236, 127};
                break;
            case 1:
                *dest = (Color) {92, 182, 189, 127};
                break;
            case 2:
                *dest = (Color) {225, 236, 231, 127};
                break;
            case 3:
                *dest = (Color) {164, 240, 216, 127};
                break;
        }
    }
    else {
        // regular ahh asteroid
        switch (rng) {
            default:
            case 0:
                *dest = (Color) {75, 35, 35, 127};
                break;
            case 1:
                *dest = (Color) {142, 27, 27, 127};
                break;
            case 2:
                *dest = (Color) {50, 44, 44, 127};
                break;
            case 3:
                *dest = (Color) {100, 100, 100, 127};
                break;
        }
    }
    return dest;
}

/// @brief get random sprite tint colour
/// @param dest already allocated pointer to colour var we change
/// @return dest
Color *_PALETTE_Sprite_TintColor_brushes( Color *dest ){
    // [0,1,2,3,4]
    int rng = INT_RAND % 5;
    switch (rng) {
        default:
        case 0:
            *dest = WHITE;
            break;
        case 1:
            // lavender
            *dest = (Color){0xdc, 0xd3, 0xff, 255};
            break;
        case 2:
            // light cyan
            *dest = (Color){0xc4, 0xfa, 0xf8, 255};
            break;
        case 3:
            // light orange
            *dest = (Color){0xff, 0xdb, 0xcc, 255};
            break;
        case 4:
            // light yellow
            *dest = (Color){0xf9, 0xff, 0xb5, 255};
            break;
    }
    return dest;
}