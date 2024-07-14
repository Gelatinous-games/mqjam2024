#pragma once


// standard library includes
#include <stdio.h>
#include <stdlib.h>

// library includes
#include "raylib.h"
#include "raymath.h"

// project includes
#include "sprite.h"
// #include "settings.h"

// === title / player sprites ===

// Sprite *_SpriteLibrary_Title_AsteroidSprite;
#define _SPRITELIBRARY_TITLE_ASTEROID_SPRITE_REF _SpriteLibrary_Asteroid_spritelist[3]
Sprite *_SpriteLibrary_Player_ShipSprite;


// === wormhole sprites ===
#define _SPRITELIBRARY_WORMHOLE_SPRITELIST_LENGTH 3
Sprite **_SpriteLibrary_Wormhole_spriteList;



// === star sprites ===
#define _SPRITELIBRARY_STAR_SPRITELIST_LENGTH 9
Sprite** _SpriteLibrary_Star_spriteList;

// the ID of black hole
#define _SPRITELIBRARY_STAR_SPRITELIST_BLACK_HOLE_ID_1 4
#define _SPRITELIBRARY_STAR_SPRITELIST_BLACK_HOLE_ID_2 8


// === planet sprites ===
#define _SPRITELIBRARY_PLANET_SPRITELIST_LENGTH 5
Sprite** _SpriteLibrary_Planet_spritelist;


// === asteroid sprites ===
#define _SPRITELIBRARY_ASTEROID_SPRITELIST_LENGTH 8
Sprite** _SpriteLibrary_Asteroid_spritelist;


// === background sprites ===
#define _SPRITELIBRARY_BACKGROUNDLAYER0_SPRITELIST_LENGTH 20
Sprite** _SpriteLibrary_backgroundLayer0_spritelist;
#define _SPRITELIBRARY_BACKGROUNDLAYER1_SPRITELIST_LENGTH 5
Sprite** _SpriteLibrary_backgroundLayer1_spritelist;
#define _SPRITELIBRARY_BACKGROUNDLAYER2_SPRITELIST_LENGTH 9
Sprite** _SpriteLibrary_backgroundLayer2_spritelist;
#define _SPRITELIBRARY_BACKGROUNDLAYER3_SPRITELIST_LENGTH 11
Sprite** _SpriteLibrary_backgroundLayer3_spritelist;


// === healthbar sprites ===

#define _SPRITELIBRARY_HEALTHBARGASLEVEL_SPRITELIST_LENGTH 10
#define _SPRITELIBRARY_HEALTHBARSHIELDLEVEL_SPRITELIST_LENGTH 10

Sprite *_SpriteLibrary_HealthBarFrame_sprite;
Sprite *_SpriteLibrary_HealthBarOverlay_sprite;
Sprite **_SpriteLibrary_HealthBarGasLevel_spritelist;
Sprite **_SpriteLibrary_HealthBarShieldLevel_spritelist;