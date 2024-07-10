#include "background_dustart.h"

#define BACKGROUNDDUST_DATA ((BackgroundDust_Data *)(THIS->data_struct))

int _BackgroundDust_Init(void* self, float DeltaTime) {
    // load sprites
    BACKGROUNDDUST_DATA->spriteList = (Sprite **)malloc(sizeof(Sprite *) * BACKGROUNDDUST_SPRITE_COUNT);
    
    int index = 0;
    BACKGROUNDDUST_DATA->spriteList[index++] = CreateSprite("resources/dust/dust_cloud_long_01.png");
    BACKGROUNDDUST_DATA->spriteList[index++] = CreateSprite("resources/dust/dust_cloud_long_02.png");
    BACKGROUNDDUST_DATA->spriteList[index++] = CreateSprite("resources/dust/dust_cloud_long_03.png");
    BACKGROUNDDUST_DATA->spriteList[index++] = CreateSprite("resources/dust/dust_cloud_long_04.png");
    BACKGROUNDDUST_DATA->spriteList[index++] = CreateSprite("resources/dust/dust_cloud_long_05.png");
    BACKGROUNDDUST_DATA->spriteList[index++] = CreateSprite("resources/dust/dust_cloud_long_06.png");
    BACKGROUNDDUST_DATA->spriteList[index++] = CreateSprite("resources/dust/dust_cloud_long_07.png");
    BACKGROUNDDUST_DATA->spriteList[index++] = CreateSprite("resources/dust/dust_cloud_long_08.png");
    BACKGROUNDDUST_DATA->spriteList[index++] = CreateSprite("resources/dust/dust_cloud_long_09.png");
    BACKGROUNDDUST_DATA->spriteList[index++] = CreateSprite("resources/dust/dust_cloud_short_01.png");
    BACKGROUNDDUST_DATA->spriteList[index++] = CreateSprite("resources/dust/dust_cloud_short_02.png");

    // generate the placements
    BACKGROUNDDUST_DATA->spawnDataList = (BackgroundDust_SpawnData *)malloc(sizeof(BackgroundDust_SpawnData)*BACKGROUNDDUST_SPAWN_MAX);
    for(int i = 0; i < BACKGROUNDDUST_SPAWN_MAX; i++){
        // generate placement
        BACKGROUNDDUST_DATA->spawnDataList[i].position = (Vector2){PLAYER_OBJECT_REF->position.x, WINDOW_HEIGHT/2.0f};
        BACKGROUNDDUST_DATA->spawnDataList[i].spriteID = 0;
    }


    return 0;
}

int _BackgroundDust_Update(void* self, float DeltaTime) {

    //  reroll if it went off camera
    //      as somewhere approaching camera
    for(int i = 0; i < BACKGROUNDDUST_SPAWN_MAX; i++){
        int response = _BackgroundDust_SpawnData_TestLeftField(THIS, BACKGROUNDDUST_DATA->spawnDataList[i], _BackgroundDust_scaleFactor);
        // when left
        if( response > 0){
            // reroll
            printf("rerolling dust %d\n",i);
            _BackgroundDust_SpawnData_RollSpawn(THIS, BACKGROUNDDUST_DATA->spawnDataList[i], _BackgroundDust_scaleFactor);
            printf("went to %f with ID %d\n",BACKGROUNDDUST_DATA->spawnDataList[i].position.x,BACKGROUNDDUST_DATA->spawnDataList[i].spriteID);
        }else{
            // printf("had response %d for x: %f, when camera %f\n", response, BACKGROUNDDUST_DATA->spawnDataList[i].position.x, cameraPosition.x);
        }
    }
    

    return 0;
}

int _BackgroundDust_Draw(void* self, float DeltaTime) {
    // each spawn
    for(int i = 0; i < BACKGROUNDDUST_SPAWN_MAX; i++){
        // get spawn
        BackgroundDust_SpawnData currSpawn = BACKGROUNDDUST_DATA->spawnDataList[i];
        // get sprite
        Sprite *currSprite = BACKGROUNDDUST_DATA->spriteList[currSpawn.spriteID];
        Vector2 position = currSpawn.position;
        // printf("drawing dust %d -> [%f, %f]\n",i,position.x,position.y);
        scaleFactor = Vector2One();
        // draw
        // ...
        RenderSpriteRelative(
            currSprite,
            position,
            // PLAYER_OBJECT_REF->position,
            // (Vector2){, currSprite->tex.height},
            currSpawn.scale,
            0.0f,
            WHITE
        );
        // RenderSquareRelative(position, (Vector2){currSprite->tex.width, currSprite->tex.height},0,(Color){200,200,200,127});
    }

    return 0;
}

int _BackgroundDust_Destroy(void* self, float DeltaTime) {

    // spawn list
    free(BACKGROUNDDUST_DATA->spawnDataList);
    BACKGROUNDDUST_DATA->spawnDataList = 0;


    // each sprite
    for(int i = 0; i < BACKGROUNDDUST_SPRITE_COUNT; i++){
        DestroySprite(BACKGROUNDDUST_DATA->spriteList[i]);
        BACKGROUNDDUST_DATA->spriteList[i] = 0;
    }

    // the list
    free(BACKGROUNDDUST_DATA->spriteList);
    BACKGROUNDDUST_DATA->spriteList = 0;


    // data structure thing
    free(((GameObj_Base *)self)->data_struct);
    ((GameObj_Base *)self)->data_struct = 0;

    return 0;
}








GameObj_Base* _BackgroundDust_Create() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_BackgroundDust_Init;
    obj_ptr->Update_Func = &_BackgroundDust_Update;
    obj_ptr->Draw_Func = &_BackgroundDust_Draw;
    obj_ptr->Destroy_Func = &_BackgroundDust_Destroy;

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_BACKGROUND;

    obj_ptr->data_struct = malloc(sizeof(BackgroundDust_Data));

    obj_ptr->currentLayer = LAYER_BACKGROUND_STARSCAPE_3;

    obj_ptr->radius = 0;
    obj_ptr->mass = 100;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = Vector2One();
    
    return obj_ptr;
}




int _BackgroundDust_SpawnData_TestLeftField(GameObj_Base *BackgroundDustObject, BackgroundDust_SpawnData spawnInformation, Vector2 backgroundDustScale){
    Sprite *currSprite = ((BackgroundDust_Data *)(BackgroundDustObject->data_struct))->spriteList[spawnInformation.spriteID];
    // currSprite->
    Vector2 currPosition = spawnInformation.position;
    float cameraLeftSide = cameraPosition.x - cameraBounds.x;
    // check less than
    if(currPosition.x < cameraPosition.x - spawnInformation.scale.x){
        // it left
        printf("%s\n", "need to reroll some dust");
        return 1;
    }
    return 0;
}

void _BackgroundDust_SpawnData_RollSpawn(GameObj_Base *BackgroundDustObject, BackgroundDust_SpawnData spawnInformation, Vector2 backgroundDustScale){
    // reroll the sprite ID
    spawnInformation.spriteID = INT_RAND % BACKGROUNDDUST_SPRITE_COUNT;
    // find the sprite with that ID
    Sprite *currSprite = ((BackgroundDust_Data *)(BackgroundDustObject->data_struct))->spriteList[spawnInformation.spriteID];
    float ratio = currSprite->tex.width / currSprite->tex.height;
    spawnInformation.scale = (Vector2){_BackgroundDust_scaleFactor.x * ratio, _BackgroundDust_scaleFactor.y / ratio};
    // position it to somewhere after the camera
    //  including the sprite width
    //  randomise it a little
    spawnInformation.position.x = cameraPosition.x + spawnInformation.scale.x + (BACKGROUNDDUST_SPAWN_EXTRADISTANCE_MAX*FLOAT_RAND);
}



#undef BACKGROUNDDUST_DATA