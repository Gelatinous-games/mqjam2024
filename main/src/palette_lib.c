#pragma once

#include "raylib.h"
#include "misc_util.h"


/**
 * @brief the express purpose of this file is for producing
 *      random colours, and should be kept to minimal overhead
 * 
 */



/// @brief get random death particle
/// @param dest already allocated pointer to colour var we change
/// @return dest
Color *_PALETTE_Player_ParticleColor_Death( Color *dest ){
    int rng = INT_RAND % 3;
    switch (rng) {
        default:
        case 0: *dest = (Color) {132,132,132,127};
        break;
        case 1: *dest = (Color) {75,75,75,127};
        break;
        case 2: *dest = (Color) {200,200,200,127};
        break;
    }
    return dest;
}