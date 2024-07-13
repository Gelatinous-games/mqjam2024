#include "star.h"

#define STAR_NEARBY_DATA ((_Star_Data *)(THIS->data_struct))


void _StarObject_Randomize(void* self) {
    Vector2 originPos;
    Vector2 originVel;
    GameObj_Base* player;
    if (!GetObjectWithFlagsExact(FLAG_PLAYER_OBJECT, 0, &player)) {
        // if no player found, default to camera pos.
        // 0/0; // hollistic debugging.
        originPos = cameraPosition;
        originVel = Vector2Zero();
    }
    else {
        originPos = player->position;
        originVel = player->velocity;
    }


    // sprite
    STAR_NEARBY_DATA->spriteID = (INT_RAND % _SPRITELIBRARY_STAR_SPRITELIST_LENGTH);

    // sometimes reroll on blackhole
    if(
        ( STAR_NEARBY_DATA->spriteID == _SPRITELIBRARY_STAR_SPRITELIST_BLACK_HOLE_ID_1 || STAR_NEARBY_DATA->spriteID == _SPRITELIBRARY_STAR_SPRITELIST_BLACK_HOLE_ID_2 ) &&
            ((INT_RAND % 20) >= BLACK_HOLE_REROLL_DC)
    ) {
        STAR_NEARBY_DATA->spriteID = (INT_RAND % _SPRITELIBRARY_STAR_SPRITELIST_LENGTH);
    }

    THIS->position.x = (STAR_RANDOM_RANGE * FLOAT_RAND * cameraBounds.x) + ( 2 * cameraBounds.x); 
    THIS->position.x += originPos.x;

    THIS->position.y = ((FLOAT_RAND) * cameraBounds.y * 2) - cameraBounds.y;


    STAR_NEARBY_DATA->maxPull = (FLOAT_RAND * FREE_MAX_SPEED * 4) + FREE_MAX_SPEED;
    STAR_NEARBY_DATA->maxRange = (FLOAT_RAND * 8) + 8;

    if(
        ( STAR_NEARBY_DATA->spriteID == _SPRITELIBRARY_STAR_SPRITELIST_BLACK_HOLE_ID_1 || STAR_NEARBY_DATA->spriteID == _SPRITELIBRARY_STAR_SPRITELIST_BLACK_HOLE_ID_2 )
    ){
        // BLACK HOLE, double them
        STAR_NEARBY_DATA->maxPull = STAR_NEARBY_DATA->maxPull * BLACK_HOLE_POWER_FACTOR;
        STAR_NEARBY_DATA->maxRange = STAR_NEARBY_DATA->maxRange * BLACK_HOLE_POWER_FACTOR;
    }

    // printf("Placed star at %f (elected origin at %f)\n", THIS->position.x, originPos.x);
    
}

Vector2 GetAccelerationToSink(GameObj_Base* star, GameObj_Base* obj) {
    _Star_Data* starData = (_Star_Data*)star->data_struct;
    float dist = Vector2Distance(obj->position, star->position);

    if (dist > starData->maxRange) return Vector2Zero();

    float delta = dist/starData->maxRange;
    delta = 1-delta; // inverse it to get 1 when closest.
    delta = delta * delta; // square it to get it reducing furher away

    Vector2 ang = Vector2Subtract(star->position, obj->position);
    ang = Vector2Normalize(ang);
    ang = Vector2Scale(ang, delta * starData->maxPull);

    return ang;
}

int _StarObject_Init(void* self, float DeltaTime) {
    _StarObject_Randomize(self);

    return 0;
}

int _StarObject_Update(void* self, float DeltaTime) {
    // stars dont move but for now, if it is too far behind player / far enough ahead, kill it and restart.

    if (THIS->position.x + (2*cameraBounds.x) + STAR_NEARBY_DATA->maxRange < cameraPosition.x) {
        // printf("Re-randomizing star (pos %f and cam %f)\n", THIS->position.x, cameraPosition.x);
        _StarObject_Randomize(self);
    }

    return 0;
}

int _StarObject_Draw(void* self, float DeltaTime) {

    // todo: red/blueshift rendering.
    
    // doesnt work
    // RenderCircleGradientAbsolute( THIS->position, STAR_NEARBY_DATA->maxRange, (Color) {255, 0, 0, 255 }, (Color) {0, 0, 255, 255 } );

    // // render effective radius
    // RenderCircleRelative(THIS->position, STAR_NEARBY_DATA->maxRange, (Color) { 255, 127, 0, 100 });
    // // RenderCircleRelative(THIS->position, THIS->radius, (Color) { 255, 127, 0, 127 });

    // star glow
    int additiveAlpha = 15;
    int shadeValue = 100;
    Color additiveColor = (Color) { shadeValue, shadeValue, shadeValue, additiveAlpha };
    RenderCircleRelative(THIS->position, STAR_NEARBY_DATA->maxRange*1.0f, additiveColor);
    RenderCircleRelative(THIS->position, STAR_NEARBY_DATA->maxRange*0.9f, additiveColor);
    RenderCircleRelative(THIS->position, STAR_NEARBY_DATA->maxRange*0.8f, additiveColor);
    RenderCircleRelative(THIS->position, STAR_NEARBY_DATA->maxRange*0.7f, additiveColor);
    RenderCircleRelative(THIS->position, STAR_NEARBY_DATA->maxRange*0.6f, additiveColor);
    RenderCircleRelative(THIS->position, STAR_NEARBY_DATA->maxRange*0.5f, additiveColor);
    RenderCircleRelative(THIS->position, STAR_NEARBY_DATA->maxRange*0.4f, additiveColor);
    RenderCircleRelative(THIS->position, STAR_NEARBY_DATA->maxRange*0.3f, additiveColor);
    RenderCircleRelative(THIS->position, STAR_NEARBY_DATA->maxRange*0.2f, additiveColor);
    

    RenderSpriteRelative(
        _SpriteLibrary_Star_spriteList[STAR_NEARBY_DATA->spriteID],
        THIS->position,
        THIS->size,
        0,
        WHITE
    );

    return 0;
}

int _StarObject_Destroy(void* self, float DeltaTime) {
    free(STAR_NEARBY_DATA);
    THIS->data_struct = 0;
    return 0;
}

GameObj_Base* CreateStarObject() {
    GameObj_Base* obj_ptr = malloc(sizeof(GameObj_Base));

    obj_ptr->data_struct = malloc(sizeof(_Star_Data));

    obj_ptr->Init_Func = &_StarObject_Init;
    obj_ptr->Update_Func = &_StarObject_Update;
    obj_ptr->Draw_Func = &_StarObject_Draw;
    obj_ptr->Destroy_Func = &_StarObject_Destroy;

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_GRAVITY_WELL;

    obj_ptr->currentLayer = LAYER_STARS;

    obj_ptr->radius = 1;
    obj_ptr->mass = 10000;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = (Vector2) { 2, 2 };
    return obj_ptr;
}


#undef STAR_NEARBY_DATA