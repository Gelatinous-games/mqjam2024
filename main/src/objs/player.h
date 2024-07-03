#pragma once

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


// Generate a simple triangle mesh from code
static Mesh GenMeshCustom(void);

typedef struct {
    Model model;
    Vector3 pos3D;
    Camera3D modelCamera;
    float a;

} Player_DataContainer;

#define PLAYERDATA ((Player_DataContainer *)(THIS->data_struct))

#define PLAYERPOSITION3D (PLAYERDATA->pos3D)

int Player_Init(void* self, float DeltaTime) {
    // we have a reference to our own gameobject from which we can do things.
    
    PLAYERDATA->model = LoadModelFromMesh(GenMeshCustom());

    // cursed
    Image checked = GenImageChecked(2, 2, 1, 1, ORANGE, BLUE);
    Texture2D baseTexture = LoadTextureFromImage(checked);
    UnloadImage(checked);

    // even more cursed
    PLAYERDATA->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = baseTexture;

    // PLAYERDATA->model.materialCount = 1;

    PLAYERDATA->pos3D = Vector3Zero();

    // superdooper cursed


    
    PLAYERDATA->modelCamera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    PLAYERDATA->modelCamera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    
    PLAYERDATA->modelCamera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    PLAYERDATA->modelCamera.fovy = 45.0f;                                // Camera field-of-view Y
    PLAYERDATA->modelCamera.projection = CAMERA_ORTHOGRAPHIC;             // Camera projection type

    return 0;
}

int Player_Update(void* self, float DeltaTime) {
    // see above
    // use data here.


    // UpdateCamera(&(PLAYERDATA->modelCamera), CAMERA_ORTHOGRAPHIC);

    

    PLAYERDATA->a += DeltaTime;

    THIS->position.x += THIS->velocity.x;
    THIS->position.y += THIS->velocity.y;

    for (int i = 0; i != -1; ) {
        GameObj_Base* obj;
        i = GetObjectWithFlagsAny(FLAG_NEUTRAL_OBJECT, i, &obj);

        // Check if obj is not null
        if (!obj || i == -1) break;

        // Do an operation with the result...
    }

    // copy to the fake 3d pos
    PLAYERDATA->pos3D.x = THIS->position.x;
    PLAYERDATA->pos3D.y = THIS->position.y;

    return 0;
}

int Player_Draw(void* self, float DeltaTime) {
    // ibid
    // Vector2 screenPosition = GetScreenspacePositionRelative(THIS->position, THIS->size);


    // DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color);
    // DrawEllipse(THIS->position.x, THIS->position.y, THIS->size.x, THIS->size.y, WHITE);


    // ultra cursed

    BeginMode3D(PLAYERDATA->modelCamera);
    DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, 2.0f, 2.0f, 2.0f, RED);
    // DrawModel(PLAYERDATA->model, PLAYERPOSITION3D, 1.0f, WHITE);
    EndMode3D();
    

    return 0;
}

int Player_Destroy(void* self, float DeltaTime) {
    // ibid.
    // if you malloc anything, destroy it here. this includes your data package.

    // free our data struct here. free anything contained.
    free(THIS->data_struct);

    return 0;
}

GameObj_Base* CreatePlayer(int windowWidth, int windowHeight) {
    GameObj_Base* obj_ptr = (GameObj_Base *)malloc(sizeof(GameObj_Base));

    // ============================================================
    // ==== setup the data scruct data
    obj_ptr->data_struct = malloc(sizeof(Player_DataContainer)); 

    // ============================================================
    // ==== assign the ufnctions
    obj_ptr->Init_Func = &Player_Init;
    obj_ptr->Update_Func = &Player_Update;
    obj_ptr->Draw_Func = &Player_Draw;
    obj_ptr->Destroy_Func = &Player_Destroy;
    // ============================================================
    // huh
    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_PLAYER_OBJECT;

    // FIXME
    obj_ptr->currentLayer = LAYER_GUI;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size.x = 50;
    obj_ptr->size.y = 50;


    // use this stuffs
    obj_ptr->position.x = 25;
    obj_ptr->position.y = (windowHeight)/2.0f;


    return obj_ptr;
}



static Mesh GenMeshCustom(void)
{
    Mesh mesh = { 0 };
    mesh.triangleCount = 1;
    mesh.vertexCount = mesh.triangleCount*3;
    mesh.vertices = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));    // 3 vertices, 3 coordinates each (x, y, z)
    mesh.texcoords = (float *)MemAlloc(mesh.vertexCount*2*sizeof(float));   // 3 vertices, 2 coordinates each (x, y)
    mesh.normals = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));     // 3 vertices, 3 coordinates each (x, y, z)

    // Vertex at (0, 0, 0)
    mesh.vertices[0] = 0;
    mesh.vertices[1] = 0;
    mesh.vertices[2] = 0;
    mesh.normals[0] = 0;
    mesh.normals[1] = 1;
    mesh.normals[2] = 0;
    mesh.texcoords[0] = 0;
    mesh.texcoords[1] = 0;

    // Vertex at (1, 0, 2)
    mesh.vertices[3] = 1;
    mesh.vertices[4] = 0;
    mesh.vertices[5] = 2;
    mesh.normals[3] = 0;
    mesh.normals[4] = 1;
    mesh.normals[5] = 0;
    mesh.texcoords[2] = 0.5f;
    mesh.texcoords[3] = 1.0f;

    // Vertex at (2, 0, 0)
    mesh.vertices[6] = 2;
    mesh.vertices[7] = 0;
    mesh.vertices[8] = 0;
    mesh.normals[6] = 0;
    mesh.normals[7] = 1;
    mesh.normals[8] = 0;
    mesh.texcoords[4] = 1;
    mesh.texcoords[5] =0;

    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    UploadMesh(&mesh, false);

    return mesh;
}