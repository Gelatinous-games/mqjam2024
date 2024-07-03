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

// 2d easy-understand camera stuff

Vector2 cameraPosition;
Vector2 cameraBounds; // object must be within this range.
float cameraExclude; // how far out from the camera range to cull objects. default to 10. (unimplemented)
Vector2 cameraScreenQuarter; // maps an object via the bounds to this
Vector2 cameraUnitSize; // maps shape sizes to this: what (1, 1) in size should be equivalent to. generally the camscreenquarter by the cambounds.
int layerCount; // how many layers to try parse to render

// 3D camera just in case we needed it
Camera3D cam3D = { 0 };

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

/// @brief Call once a frame before drawing to use a similar 3D orthographic camera.
void UpdateCamera3D() {
    cam3D.position = (Vector3){ cameraPosition.x, cameraPosition.y, 1 };
    cam3D.target = (Vector3){ cameraPosition.x, cameraPosition.y, 0};
    cam3D.up = (Vector3){ 0, -1, 0 };

    cam3D.projection = CAMERA_ORTHOGRAPHIC;

    cam3D.fovy = cameraBounds.y * 2;
}