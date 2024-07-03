// Camera data: we're going for simplicity, not scale.

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

Vector2 cameraPosition;
Vector2 cameraBounds; // object must be within this range.
float cameraExclude; // how far out from the camera range to cull objects. default to 10.
Vector2 cameraScreenQuarter; // maps an object via the bounds to this
Vector2 cameraUnitSize; // maps shape sizes to this: what (1, 1) in size should be equivalent to. generally the camscreenquarter by the cambounds.

int layerCount;

/// @brief Computes a screenspace position relative to (0, 0) for rendering.
/// @param pos 
/// @return 
Vector2 GetScreenspacePositionAbsolute(Vector2 pos, Vector2 size) {
    pos = Vector2Subtract(pos, Vector2Scale(size, 0.5));
    pos.x = pos.x / cameraBounds.x;
    pos.y = pos.y / cameraBounds.y;

    pos = Vector2Multiply(pos, cameraScreenQuarter);

    pos = Vector2Add(pos, cameraScreenQuarter);

    return pos;
}

/// @brief Computes a screenspace position relative to the camera for rendering.
/// @param pos 
/// @return 
Vector2 GetScreenspacePositionRelative(Vector2 pos, Vector2 size) {
    pos = Vector2Subtract(pos, cameraPosition);
    
    return GetScreenspacePositionAbsolute(pos, size);
}

/// @brief Gets the pixel size of a shape.
/// @param size 
/// @return 
Vector2 GetScaledSize(Vector2 size) {
    return Vector2Multiply(size, cameraUnitSize);
}