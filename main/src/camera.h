#pragma once

// Camera data: we're going for simplicity, not scale.

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "sprite.h"

// 2d easy-understand camera stuff

Vector2 cameraPosition;
Vector2 cameraVelocity;
Vector2 cameraBounds;        // object must be within this range.
float cameraExclude;         // how far out from the camera range to cull objects. default to 10. (unimplemented)
Vector2 cameraScreenQuarter; // maps an object via the bounds to this
Vector2 cameraUnitSize;      // maps shape sizes to this: what (1, 1) in size should be equivalent to. generally the camscreenquarter by the cambounds.

// Change this to make them seem further away from the screen.
Vector2 scaleFactor;

// 3D camera just in case we needed it
Camera3D cam3D = {0};
