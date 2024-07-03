
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "../base.h"
#include "../settings.h"


#ifndef _camera
    #define _camera
    #include "../camera.c"
#endif


#ifndef _obj_pool
    #define _obj_pool
    #include "../obj_pool.c"
#endif

/** ====== PARENT OBJ DATA ======
    // a structure to refer to for all unique data on this object. void type, so will require casting to the particular func type.
    void* data_struct; 
    int _poolID;

    int currentLayer;

    /// @brief Set to 1 to flag it to be destroyed after the next frame.
    int awaitDestroy;

    // generic data; position, velocity, size.
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    long unsigned int flags;

    int (*Init_Func)(void* self, float DeltaTime);
    int (*Update_Func)(void* self, float DeltaTime);
    int (*Draw_Func)(void* self, float DeltaTime);
    int (*Destroy_Func)(void* self, float DeltaTime);
*/
typedef struct {
    Texture2D spriteTexture;
    float ageTime;
    float degreeRotationSpeed;
    float degreeRotation;
    float scalarScale;
} Asteroid_Data;

#define ASTEROIDDATA ((Asteroid_Data *)(THIS->data_struct))

int Asteroid_Init(void* self, float DeltaTime) {
    // we have a reference to our own gameobject from which we can do things.

    // ==================================================
    // idk some values now

    THIS->position.x = 25;
    THIS->position.y = 345;

    THIS->velocity.x = 2;
    THIS->velocity.y = 4;

    // ==================================================

    // printf("exist in: [%s]\n",GetWorkingDirectory());
    ASTEROIDDATA->spriteTexture = LoadTexture("resources/asteroid.png");

    THIS->size.x = ASTEROIDDATA->spriteTexture.width;
    THIS->size.y = ASTEROIDDATA->spriteTexture.height;

    // ==================================================
    
    ASTEROIDDATA->scalarScale = 1.0f;
    ASTEROIDDATA->degreeRotation = 0.0f;
    // ASTEROIDDATA->degreeRotationSpeed = (int)(random()%360);
    ASTEROIDDATA->degreeRotationSpeed = 360;
    
    // ==================================================

    return 0;
}

int Asteroid_Update(void* self, float DeltaTime) {
    // see above

    // use data here.

    ASTEROIDDATA->ageTime += DeltaTime;
    ASTEROIDDATA->degreeRotation += DeltaTime*ASTEROIDDATA->degreeRotationSpeed;

    THIS->position.x += THIS->velocity.x;
    THIS->position.y += THIS->velocity.y;

    if(THIS->position.y > WINDOW_HEIGHT){
        THIS->position.y = 0;
    }
    else if(THIS->position.y < 0){
        THIS->position.y = WINDOW_HEIGHT;
    }

    for (int i = 0; i != -1; ) {
        GameObj_Base* obj;
        i = GetObjectWithFlagsAny(FLAG_NEUTRAL_OBJECT, i, &obj);

        // Check if obj is not null
        if (!obj || i == -1) break;

        // Do an operation with the result...
    }

    return 0;
}

int Asteroid_Draw(void* self, float DeltaTime) {
    // ibid
    
    // DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color);
    // DrawEllipse(THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y, DARKGRAY);
    

    // DrawTexture(ASTEROIDDATA->spriteTexture, (int)(THIS->position.x), (int)(THIS->position.y), WHITE);


    // void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint)
    DrawTextureEx(ASTEROIDDATA->spriteTexture, THIS->position, ASTEROIDDATA->degreeRotation, ASTEROIDDATA->scalarScale, WHITE);
    
    



    return 0;
}

int Asteroid_Destroy(void* self, float DeltaTime) {
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.

    UnloadTexture(ASTEROIDDATA->spriteTexture);
    // free our data struct here. free anything contained.
    free(THIS->data_struct);


    return 0;
}

GameObj_Base* CreateAsteroid() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    // ============================================================
    // ==== setup the data scruct data
    obj_ptr->data_struct = malloc(sizeof(Asteroid_Data)); 

    // ============================================================
    // ==== assign the ufnctions
    obj_ptr->Init_Func = &Asteroid_Init;
    obj_ptr->Update_Func = &Asteroid_Update;
    obj_ptr->Draw_Func = &Asteroid_Draw;
    obj_ptr->Destroy_Func = &Asteroid_Destroy;
    // ============================================================
    // huh
    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_ASTEROID;

    // FIXME
    obj_ptr->currentLayer = LAYER_GUI;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size.x = 35;
    obj_ptr->size.y = 35;

    return obj_ptr;
}
