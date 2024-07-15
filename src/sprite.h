#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"


typedef struct {
    Texture2D tex;
    Rectangle src;
    Rectangle dst;
    Vector2 origin;
} Sprite;

/// @brief Properly creates & fills out data for a Sprite object, returning a pointer. Remember to destroy a sprite once used.
/// Try not to use too many sprites. Make sure they're destroyed too!
/// @param filePath 
/// @param sizeOut How big it should be (in camera units) once rendering. Consults camera, so should be updated if the camera changes.
/// @return 
Sprite* CreateSprite(const char *filePath);
void DestroySprite(Sprite* sprite);