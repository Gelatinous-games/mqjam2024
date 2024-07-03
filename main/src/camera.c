// Camera data: we're going for simplicity, not scale.

#include <stdio.h>
#include <stdlib.h>

#ifndef _raylib
    #define _raylib
    #include "raylib.h"
#endif

#ifndef _raymath
    #define _raymath
    #include "raymath.h"
#endif

Vector2 cameraPosition;
Vector2 cameraBounds; // object must be within this range.
Vector2 cameraScreenQuarter; // maps an object via the bounds to this

int layerCount;

/// @brief Computes a screenspace position relative to (0, 0) for rendering.
/// @param pos 
/// @return 
Vector2 GetScreenspacePositionAbsolute(Vector2 pos, Vector2 size) {
    pos.x = pos.x / cameraBounds.x;
    pos.y = pos.y / cameraBounds.y;

    pos = Vector2Multiply(pos, cameraScreenQuarter);

    pos = Vector2Add(pos, cameraScreenQuarter);
    
    size.x /= 2; size.y /= 2;

    pos = Vector2Subtract(pos, size);

    return pos;
}

/// @brief Computes a screenspace position relative to the camera for rendering.
/// @param pos 
/// @return 
Vector2 GetScreenspacePositionRelative(Vector2 pos, Vector2 size) {
    pos = Vector2Subtract(pos, cameraPosition);
    
    return GetScreenspacePositionAbsolute(pos, size);
}