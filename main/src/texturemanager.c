#include "raylib.h"

enum _TEXTURELIBRARY_TextureType {
    TEXTURETYPE_ASTEROID,
    TEXTURETYPE_BGART,
    TEXTURETYPE_BGBRUSH,
    TEXTURETYPE_HEALTHBAR,
    TEXTURETYPE_PLANET,
    TEXTURETYPE_STAR,
    TEXTURETYPE_WORMHOLE,
    TEXTURETYPE_SHIP,

    // ...
    TEXTURETYPE_COUNTER
};


#define _TEXTURELIBRARY_AsteroidTextureList_SIZE 4
#define _TEXTURELIBRARY_BackgroundArtTextureList_SIZE 10
#define _TEXTURELIBRARY_BackgroundBrushTextureList_SIZE 22
#define _TEXTURELIBRARY_HealthBarTextureList_SIZE 22
#define _TEXTURELIBRARY_PlanetTextureList_SIZE 5
#define _TEXTURELIBRARY_StarTextureList_SIZE 5
#define _TEXTURELIBRARY_WormholeTextureList_SIZE 1
#define _TEXTURELIBRARY_ShipTextureList_SIZE 1

Texture2D **_TEXTURELIBRARY_TextureLists;

char **_TEXTURELIBRARY_TexturePathList;













void InitialiseTextureLibrary(){
    // ...
    // Texture2D tex = LoadTexture(const char *);
    
}

Texture2D *_TEXTURELIBRARY_GetTextureList(enum _TEXTURELIBRARY_TextureType typeIndex){
    if(_TEXTURELIBRARY_TextureLists)
    switch(typeIndex){
        default:
        case TEXTURETYPE_ASTEROID: return _TEXTURELIBRARY_TextureLists[TEXTURETYPE_ASTEROID];
        case TEXTURETYPE_BGART: return _TEXTURELIBRARY_TextureLists[TEXTURETYPE_BGART];
        case TEXTURETYPE_BGBRUSH: return _TEXTURELIBRARY_TextureLists[TEXTURETYPE_BGBRUSH];
        case TEXTURETYPE_HEALTHBAR: return _TEXTURELIBRARY_TextureLists[TEXTURETYPE_HEALTHBAR];
        case TEXTURETYPE_PLANET: return _TEXTURELIBRARY_TextureLists[TEXTURETYPE_PLANET];
        case TEXTURETYPE_STAR: return _TEXTURELIBRARY_TextureLists[TEXTURETYPE_STAR];
        case TEXTURETYPE_WORMHOLE: return _TEXTURELIBRARY_TextureLists[TEXTURETYPE_WORMHOLE];
        case TEXTURETYPE_SHIP: return _TEXTURELIBRARY_TextureLists[TEXTURETYPE_SHIP];
    }
}

// Texture2D *GetTexture(const char *TexturePath){
//     for(int typeIndex = 0; typeIndex < TEXTURETYPE_COUNTER; typeIndex++){
//         // ...
//     }
// }

int _TEXTURELIBRARY_GetTextureListSize(enum _TEXTURELIBRARY_TextureType typeIndex){
    if(_TEXTURELIBRARY_TextureLists)
    switch(typeIndex){
        default:
        case TEXTURETYPE_ASTEROID: return _TEXTURELIBRARY_AsteroidTextureList_SIZE;
        case TEXTURETYPE_BGART: return _TEXTURELIBRARY_BackgroundArtTextureList_SIZE;
        case TEXTURETYPE_BGBRUSH: return _TEXTURELIBRARY_BackgroundBrushTextureList_SIZE;
        case TEXTURETYPE_HEALTHBAR: return _TEXTURELIBRARY_HealthBarTextureList_SIZE;
        case TEXTURETYPE_PLANET: return _TEXTURELIBRARY_PlanetTextureList_SIZE;
        case TEXTURETYPE_STAR: return _TEXTURELIBRARY_StarTextureList_SIZE;
        case TEXTURETYPE_WORMHOLE: return _TEXTURELIBRARY_WormholeTextureList_SIZE;
        case TEXTURETYPE_SHIP: return _TEXTURELIBRARY_ShipTextureList_SIZE;
    }
}

// TODO: have this do the grab from the list aaaa pain who does fast?
// /// @brief 
// /// @param TexturePath 
// /// @return 
// int GetTextureID(const char *TexturePath){
//     // each list
//     for(int i = 0; i < TEXTURETYPE_COUNTER; i++){
//         // each item in the list
//         for(int j = 0; j < _TEXTURELIBRARY_GetTextureListSize(i); j++){
//             // TODO
//         }
//     }
// }


void DestroyTextureLibrary(){
    // ...
    // UnloadTexture(tex);
}