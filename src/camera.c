#include "camera.h"

/// @brief Computes a screenspace position relative to (0, 0) for rendering.
/// @param pos
/// @return
Vector2 GetScreenspacePositionAbsolute(Vector2 pos, Vector2 size)
{
    pos = Vector2Subtract(pos, Vector2Scale(size, 0.5));
    pos.x = pos.x / (cameraBounds.x * scaleFactor.x);
    pos.y = pos.y / (cameraBounds.y * scaleFactor.x);

    pos = Vector2Multiply(pos, cameraScreenQuarter);

    pos = Vector2Add(pos, cameraScreenQuarter);

    return pos;
}

/// @brief Computes a screenspace position relative to the camera for rendering.
/// @param pos
/// @return
Vector2 GetScreenspacePositionRelative(Vector2 pos, Vector2 size)
{
    pos = Vector2Subtract(pos, cameraPosition);

    return GetScreenspacePositionAbsolute(pos, size);
}

/// @brief Gets the pixel size of a shape.
/// @param size
/// @return
Vector2 GetScaledSize(Vector2 size)
{
    size = (Vector2){size.x / scaleFactor.x, size.y / scaleFactor.y};
    return Vector2Multiply(size, cameraUnitSize);
}

/// @brief Gets the unit shape of a pixel size.
/// @param size 
/// @return 
Vector2 GetUnscaledSize(Vector2 size) {
    size = (Vector2){size.x / cameraUnitSize.x, size.y /cameraUnitSize.y};
    return Vector2Multiply(size, scaleFactor);
}

/// @brief Gets the pixel radius of a circle as per the scale factor. Note that the scale factor on the x axis is used, not y.
/// @param radius
/// @return
float GetScaledRadius(float radius)
{
    radius = radius / scaleFactor.x;
    return radius * cameraUnitSize.x;
}

/// @brief Sets the scale factor of the camera. Defaults to 1. Anything above 1 will make the camera appear further away, anything below 1 makes it closer.
/// If you need to set different scaling per axis, you can utilize Vector2 scaleFactor in camera.c to do the same thing.
/// This is reset to 1 before each object draw call so you do not need to worry about affecting other object draws.
/// @param sf
void SetScaleFactor(float sf)
{
    scaleFactor = (Vector2){sf, sf};
}

/// @brief Call once a frame before drawing to use a similar 3D orthographic camera.
void UpdateCamera3D()
{
    cam3D.position = (Vector3){cameraPosition.x, cameraPosition.y, 1};
    cam3D.target = (Vector3){cameraPosition.x, cameraPosition.y, 0};
    cam3D.up = (Vector3){0, -1, 0};

    cam3D.projection = CAMERA_ORTHOGRAPHIC;

    cam3D.fovy = cameraBounds.y * 2;
}

/// @brief Renders a sprite relative to the camera at the provided position, rotation & color.
/// @param sprite
/// @param pos
/// @param size
/// @param rotation
/// @param color
void RenderSpriteRelative(Sprite *sprite, Vector2 pos, Vector2 size, float rotation, Color color)
{
    pos = GetScreenspacePositionRelative(pos, Vector2Zero());
    size = GetScaledSize(size);

    sprite->dst = (Rectangle){pos.x, pos.y, size.x, size.y};
    sprite->origin = Vector2Scale(size, 0.5);

    DrawTexturePro(sprite->tex, sprite->src, sprite->dst, sprite->origin, rotation, color);
}

/// @brief Renders a given sprite at the absolute provided position, rotation & color.
/// @param sprite
/// @param pos
/// @param size
/// @param rotation
/// @param color
void RenderSpriteAbsolute(Sprite *sprite, Vector2 pos, Vector2 size, float rotation, Color color)
{
    pos = GetScreenspacePositionAbsolute(pos, size);
    size = GetScaledSize(size);

    sprite->dst = (Rectangle){pos.x, pos.y, size.x, size.y};

    DrawTexturePro(sprite->tex, sprite->src, sprite->dst, sprite->origin, rotation, color);
}

/// @brief Renders a square relative to the camera at the provided position, rotation & color.
/// @param pos
/// @param size
/// @param rotation
/// @param color
void RenderSquareRelative(Vector2 pos, Vector2 size, float rotation, Color color)
{
    pos = GetScreenspacePositionRelative(pos, size);
    size = GetScaledSize(size);

    DrawRectangleV(pos, size, color);
}

/// @brief Renders a square at the absolute provided position, rotation & color.
/// @param pos
/// @param size
/// @param rotation
/// @param color
void RenderSquareAbsolute(Vector2 pos, Vector2 size, float rotation, Color color)
{
    pos = GetScreenspacePositionAbsolute(pos, size);
    size = GetScaledSize(size);

    DrawRectangleV(pos, size, color);
}

/// @brief Draws a circle at an absolute position with a given radius (in units) and color.
/// @param pos
/// @param radius
/// @param color
void RenderCircleAbsolute(Vector2 pos, float radius, Color color)
{
    Vector2 size = (Vector2){radius * 2, radius * 2};
    pos = GetScreenspacePositionAbsolute(pos, size);
    size = GetScaledSize(size);

    radius = size.x / 2;

    DrawCircle(pos.x + radius, pos.y + radius, radius, color);
}
/// @brief Draws a circle at an absolute position with a given radius (in units) and color.
/// @param pos
/// @param radius
/// @param color
void RenderCircleGradientAbsolute(Vector2 pos, float radius, Color color1, Color color2)
{
    Vector2 size = (Vector2){radius * 2, radius * 2};
    pos = GetScreenspacePositionAbsolute(pos, size);
    size = GetScaledSize(size);

    radius = size.x / 2;

    // void DrawCircleGradient(int centerX, int centerY, float radius, Color color1, Color color2)
    //     Draw a gradient-filled circle
    //     NOTE: Gradient goes from center (color1) to border (color2)
    DrawCircleGradient(pos.x + radius, pos.y + radius, radius, color1, color2);
}

/// @brief Draws a circle relative to the camera with the given radius (in units) and color.
/// @param pos
/// @param radius
/// @param color
void RenderCircleRelative(Vector2 pos, float radius, Color color)
{
    Vector2 size = (Vector2){radius * 2, radius * 2};
    pos = GetScreenspacePositionRelative(pos, size);
    size = GetScaledSize(size);

    radius = size.x / 2;

    DrawCircle(pos.x + radius, pos.y + radius, radius, color);
}

void RenderColliderRelative(Vector2 pos, float radius)
{
    Vector2 size = (Vector2){radius * 2, radius * 2};
    pos = GetScreenspacePositionRelative(pos, size);
    size = GetScaledSize(size);
    radius = size.x / 2;

    // DrawEllipse(pos.x, pos.y, radius, radius, (Color) { 255, 0, 0, 127 });
    DrawCircle(pos.x + radius, pos.y + radius, radius, (Color){255, 0, 0, 127});
}
/// @brief Draw a color-filled triangle at an absolute position (vertex in counter-clockwise order!)
/// @param pos
/// @param radius
/// @param color
void RenderTriangleAbsolute(Vector2 v1, Vector2 v2, Vector2 v3, Color color)
{
    Vector2 size = (Vector2){-1, -1};
    v1 = GetScreenspacePositionAbsolute(v1, size);
    v2 = GetScreenspacePositionAbsolute(v2, size);
    v3 = GetScreenspacePositionAbsolute(v3, size);

    // Draw a color-filled triangle (vertex in counter-clockwise order!)
    DrawTriangle(v1, v2, v3, color);
}

/// @brief Draw a color-filled triangle at an absolute position (vertex in counter-clockwise order!)
/// @param pos
/// @param radius
/// @param color
void RenderTriangleRelative(Vector2 v1, Vector2 v2, Vector2 v3, Color color)
{
    Vector2 size = (Vector2){1, 1};
    v1 = GetScreenspacePositionRelative(v1, size);
    v2 = GetScreenspacePositionRelative(v2, size);
    v3 = GetScreenspacePositionRelative(v3, size);
    // Draw a color-filled triangle (vertex in counter-clockwise order!)
    DrawTriangle(v1, v2, v3, color);
}