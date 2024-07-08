
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "../base.h"


#ifndef _camera
    #define _camera
    #include "../camera.c"
#endif

#ifndef _obj_pool
    #define _obj_pool
    #include "../obj_pool.c"
#endif

#include "../obj_register.h"
#include "../settings.h"



typedef struct {
    char* data;
    float textFlash;
    float textTmr;
    char doTextFlash;
    char visible;
    char doWidthSubtr;
} _TextObj_Data;

#define DATA ((_TextObj_Data *)(THIS->data_struct))

int _TextObj_Init(void* self, float DeltaTime) {
    DATA->visible = 1;

    SetScaleFactor(1);
    // properly set size
    Vector2 size = GetScaledSize(THIS->size);
    int width = MeasureText(DATA->data, size.y);

    // FACE MY WRATH
    //int i = 1/0;
    size.x = width;
    THIS->size = GetUnscaledSize(size);

    return 0;
}

int _TextObj_Update(void* self, float DeltaTime) {
    if (DATA->doTextFlash) {
        DATA->textTmr -= DeltaTime;
        if (DATA->textTmr < 0) {
            DATA->textTmr = DATA->textFlash;
            DATA->visible = 1-DATA->visible;
        }
    }
    return 0;
}

int _TextObj_Draw(void* self, float DeltaTime) {
    if (DATA->visible) {
        // printf("rendering with pos <%f,%f> and size <%f,%f>\n", THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y);
        Vector2 size = GetScaledSize(THIS->size);
        int width = MeasureText(DATA->data, size.y);

        Vector2 pos = GetScreenspacePositionAbsolute(THIS->position, THIS->size);

        if (DATA->doWidthSubtr) width = 0;
        DrawText(DATA->data, pos.x+(width/2), pos.y, size.y, WHITE);
    }
    return 0;
}

int _TextObj_Destroy(void* self, float DeltaTime) {
    // Destroy any objects we're using for the death screen.

    free(DATA);
    THIS->data_struct = 0;
    return 0;
}

GameObj_Base* CreateTextObj() {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    obj_ptr->Init_Func = &_TextObj_Init;
    obj_ptr->Update_Func = &_TextObj_Update;
    obj_ptr->Draw_Func = &_TextObj_Draw;
    obj_ptr->Destroy_Func = &_TextObj_Destroy;

    obj_ptr->data_struct = malloc(sizeof(_TextObj_Data));

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_GUI;

    obj_ptr->currentLayer = LAYER_GUI_2;

    obj_ptr->radius = 0;
    obj_ptr->mass = 0;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = Vector2One();
    
    return obj_ptr;
}

#undef DATA