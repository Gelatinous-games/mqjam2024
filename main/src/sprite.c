#include "sprite.h"

/// @brief Properly creates & fills out data for a Sprite object, returning a pointer. Remember to destroy a sprite once used.
/// Try not to use too many sprites. Make sure they're destroyed too!
/// @param filePath 
/// @param sizeOut How big it should be (in camera units) once rendering. Consults camera, so should be updated if the camera changes.
/// @return 
Sprite* CreateSprite(const char *filePath) {
    Sprite* tmp = malloc(sizeof(Sprite));

    tmp->tex = LoadTexture(filePath);
    tmp->src = (Rectangle) { 0, 0, tmp->tex.width, tmp->tex.height };
    tmp->dst = (Rectangle) { 0, 0, 2*tmp->tex.width, 2*tmp->tex.height};
    tmp->origin = (Vector2) { tmp->tex.width/2, tmp->tex.height/2 };

    return tmp;
}

void DestroySprite(Sprite* sprite) {
    UnloadTexture(sprite->tex);
    free(sprite);
}