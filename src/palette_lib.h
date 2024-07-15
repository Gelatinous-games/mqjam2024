#pragma once

#include "raylib.h"
#include "misc_util.h"

/**
 * @brief the express purpose of this file is for producing
 *      random colours, and should be kept to minimal overhead
 * 
 */


/// @brief get random debris particle colour
/// @param dest already allocated pointer to colour var we change
/// @return dest
Color *_PALETTE_Player_ParticleColor_Debris( Color *dest );

/// @brief get random trail particle colour
/// @param dest already allocated pointer to colour var we change
/// @return dest
Color *_PALETTE_Player_ParticleColor_Trail( Color *dest );

/// @brief get random hull collision particle colour
/// @param dest already allocated pointer to colour var we change
/// @return dest
Color *_PALETTE_Player_ParticleColor_HullCollision( Color *dest );

/// @brief get random shield collision particle colour
/// @param dest already allocated pointer to colour var we change
/// @return dest
Color *_PALETTE_Player_ParticleColor_ShieldCollision( Color *dest );

/// @brief get random asteroid collision particle colour
/// [replaces]: GetAsteroidParticleColor()
/// @param dest already allocated pointer to colour var we change
/// @return dest
Color *_PALETTE_Asteroid_ParticleColor_Collision( Color *dest, int isIcyComet );

/// @brief get random sprite tint colour
/// @param dest already allocated pointer to colour var we change
/// @return dest
Color *_PALETTE_Sprite_TintColor_brushes( Color *dest );