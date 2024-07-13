#include "planet_body.h"

#define PLANET_DATA ((Planet_DataStruct *)(THIS->data_struct))

int _Planet_Init(void* self, float DeltaTime) {

    THIS->data_struct = malloc(sizeof(Planet_DataStruct));

    
    // choose randome sprite
    PLANET_DATA->spriteID = (INT_RAND%_SPRITELIBRARY_PLANET_SPRITELIST_LENGTH);
    printf("using planet %d\n",PLANET_DATA->spriteID);
    if (TO_WORMHOLE)
        THIS->position.x = PLANET_DATA->distanceFromStart;
    else
        THIS->position.x = WORMHOLE_TRAVEL_DISTANCE;
    THIS->position.y = 0;
    PLANET_DATA->distanceFromStart = THIS->position.x;

    PLANET_DATA->rotate = 0;

    THIS->size.x = 12;
    THIS->size.y = 12;

    THIS->radius = cameraBounds.y;

    return 0;
}

int _Planet_Update(void* self, float DeltaTime) {

    // // dont let the player past the Planet
    // if (PLAYER_OBJECT_REF->position.x > THIS->position.x) {
    //     PLAYER_OBJECT_REF->position.x = THIS->position.x;
    // }

    PLANET_DATA->rotate += DeltaTime *2;

    return 0;
}

int _Planet_Draw(void* self, float DeltaTime) {
    // ibid

    RenderSpriteRelative(_SpriteLibrary_Planet_spritelist[PLANET_DATA->spriteID], THIS->position, THIS->size, PLANET_DATA->rotate, WHITE);

    return 0;
}

int _Planet_Destroy(void* self, float DeltaTime) {
    // ibid.


    free(PLANET_DATA);

    return 0;
}

GameObj_Base* CreatePlanet() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_Planet_Init;
    obj_ptr->Update_Func = &_Planet_Update;
    obj_ptr->Draw_Func = &_Planet_Draw;
    obj_ptr->Destroy_Func = &_Planet_Destroy;

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_WORMHOLE;

    obj_ptr->currentLayer = LAYER_WORMHOLE;

    obj_ptr->radius = 0;
    obj_ptr->mass = 100;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = Vector2One();
    return obj_ptr;
}


#undef PLANET_DATA