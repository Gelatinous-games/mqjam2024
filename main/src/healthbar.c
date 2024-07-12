#include "healthbar.h"

#define HEALTHBAR_DATA ((HealthBar_DataStruct *)(THIS->data_struct))




int _HealthBar_Init(void* self, float DeltaTime) {
    // we have a reference to our own gameobject from which we can do things.
    // here we should create a reference to our datastructure and store it in the data_struct pointer.

    THIS->data_struct = malloc(sizeof(HealthBar_DataStruct));


    HEALTHBAR_DATA->currentHealthBarIndex = _SPRITELIBRARY_HEALTHBARGASLEVEL_SPRITELIST_LENGTH-1;

    THIS->size.x = _SpriteLibrary_HealthBarFrame_sprite->tex.width;
    THIS->size.y = _SpriteLibrary_HealthBarFrame_sprite->tex.height;

    THIS->position.x = WINDOW_WIDTH/2;
    THIS->position.y = WINDOW_HEIGHT-(THIS->size.y);


    return 0;
}

int _HealthBar_Update(void* self, float DeltaTime) {
    


    // TOO LOW
    if(GetPlayerHullPercentage() <= 0.0f){ 
        // dont draw
        HEALTHBAR_DATA->currentHealthBarIndex = -1;
    }
    // TOO FAR
    else if(GetPlayerHullPercentage() >= 1.0f){ 
        HEALTHBAR_DATA->currentHealthBarIndex = _SPRITELIBRARY_HEALTHBARGASLEVEL_SPRITELIST_LENGTH-1;
    }
    else {
        HEALTHBAR_DATA->currentHealthBarIndex = (int)(GetPlayerHullPercentage() * (float)(_SPRITELIBRARY_HEALTHBARGASLEVEL_SPRITELIST_LENGTH));
    }

    if(_ShieldObject_GetPlayerShieldPercentage() <= 0.0f){
        // dont draw
        HEALTHBAR_DATA->currentShieldBarIndex = -1;
    }
    else if(_ShieldObject_GetPlayerShieldPercentage() >= 1.0f){ 
        HEALTHBAR_DATA->currentShieldBarIndex = _SPRITELIBRARY_HEALTHBARSHIELDLEVEL_SPRITELIST_LENGTH-1;
    }
    else {
        // ...
        HEALTHBAR_DATA->currentShieldBarIndex = (int)(_ShieldObject_GetPlayerShieldPercentage() * (float)(_SPRITELIBRARY_HEALTHBARSHIELDLEVEL_SPRITELIST_LENGTH));
    }
    




    

    return 0;
}

int _HealthBar_Draw(void* self, float DeltaTime) {
    // ibid
    // ===========================================
    // === draw frame

    _SpriteLibrary_HealthBarFrame_sprite->dst = (Rectangle) { THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y };
    _SpriteLibrary_HealthBarFrame_sprite->origin = Vector2Scale(THIS->size, 0.5);

    // frame tinter
    Color frameTint = ALIVE_HEALTHBAR_FRAME_TINT;
    if(CURRENT_PLAYER_LIFE_STATE == PLAYER_LIFE_STATUS_ISDEAD) frameTint = DEATH_HEALTHBAR_FRAME_TINT;

    DrawTexturePro(_SpriteLibrary_HealthBarFrame_sprite->tex, _SpriteLibrary_HealthBarFrame_sprite->src, _SpriteLibrary_HealthBarFrame_sprite->dst, _SpriteLibrary_HealthBarFrame_sprite->origin, 0, frameTint);


    // ===========================================
    // === draw level

    // not dead
    if(HEALTHBAR_DATA->currentHealthBarIndex >= 0){
        // grab the sprite
        Sprite *healthLevelSprite = _SpriteLibrary_HealthBarGasLevel_spritelist[HEALTHBAR_DATA->currentHealthBarIndex];

        healthLevelSprite->dst = (Rectangle) { THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y };
        healthLevelSprite->origin = Vector2Scale(THIS->size, 0.5);

        DrawTexturePro(healthLevelSprite->tex, healthLevelSprite->src, healthLevelSprite->dst, healthLevelSprite->origin, 0, WHITE);
    }
    // has shields
    if(HEALTHBAR_DATA->currentShieldBarIndex >= 0){
        // deal with shield sprite

        // grab the sprite
        Sprite *shieldLevelSprite = _SpriteLibrary_HealthBarShieldLevel_spritelist[HEALTHBAR_DATA->currentShieldBarIndex];

        shieldLevelSprite->dst = (Rectangle) { THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y };
        shieldLevelSprite->origin = Vector2Scale(THIS->size, 0.5);

        DrawTexturePro(shieldLevelSprite->tex, shieldLevelSprite->src, shieldLevelSprite->dst, shieldLevelSprite->origin, 0, (Color){255,255,255,127});
    }

    // ===========================================
    // === draw overlay

    _SpriteLibrary_HealthBarOverlay_sprite->dst = (Rectangle) { THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y };
    _SpriteLibrary_HealthBarOverlay_sprite->origin = Vector2Scale(THIS->size, 0.5);

    // overlay tintind
    Color overlayTint = WHITE;
    if(CURRENT_PLAYER_LIFE_STATE == PLAYER_LIFE_STATUS_ISDEAD) overlayTint = DEATH_HEALTHBAR_OVERLAY_TINT;

    // draw it
    DrawTexturePro(_SpriteLibrary_HealthBarOverlay_sprite->tex, _SpriteLibrary_HealthBarOverlay_sprite->src, _SpriteLibrary_HealthBarOverlay_sprite->dst, _SpriteLibrary_HealthBarOverlay_sprite->origin, 0, overlayTint);

    // ===========================================

    return 0;
}

int _HealthBar_Destroy(void* self, float DeltaTime) {
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.


    free(HEALTHBAR_DATA);

    return 0;
}

GameObj_Base* CreateHealthBar() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_HealthBar_Init;
    obj_ptr->Update_Func = &_HealthBar_Update;
    obj_ptr->Draw_Func = &_HealthBar_Draw;
    obj_ptr->Destroy_Func = &_HealthBar_Destroy;

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_BACKGROUND;

    obj_ptr->currentLayer = LAYER_GUI;

    obj_ptr->radius = 0;
    obj_ptr->mass = 0;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = Vector2One();
    return obj_ptr;
}