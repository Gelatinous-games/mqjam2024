#include "SpriteLibrary.h"













int _SpriteLibrary_LoadSprites(){
    int index = 0;

    // === title / player sprites ===

    // _SpriteLibrary_Title_AsteroidSprite = CreateSprite("resources/asteroids/A3.png");

    _SpriteLibrary_Player_ShipSprite = CreateSprite("resources/spaceship_outlined_v2.png");


    // === wormhole sprites ===
    _SpriteLibrary_Wormhole_spriteList = (Sprite **)malloc(sizeof(Sprite *) * _SPRITELIBRARY_WORMHOLE_SPRITELIST_LENGTH);
    index = 0;
    _SpriteLibrary_Wormhole_spriteList[index++] = CreateSprite("resources/wormhole/w1.png");
    _SpriteLibrary_Wormhole_spriteList[index++] = CreateSprite("resources/wormhole/w2.png");
    _SpriteLibrary_Wormhole_spriteList[index++] = CreateSprite("resources/wormhole/w3.png");


    // === star sprites ===
    _SpriteLibrary_Star_spriteList = malloc(sizeof(Sprite*) * _SPRITELIBRARY_STAR_SPRITELIST_LENGTH);
    index = 0;
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S0.png");
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S1.png");
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S2.png");
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S3.png");
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S4.png"); // blackhole
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S5.png");
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S6.png");
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S7.png");
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S8.png"); // blackhole


    // === planet sprites ===
    // printf("%s\n","loading planets");
    _SpriteLibrary_Planet_spritelist = (Sprite **)malloc(_SPRITELIBRARY_PLANET_SPRITELIST_LENGTH * sizeof(Sprite*));
    index = 0;
    _SpriteLibrary_Planet_spritelist[index++] = CreateSprite("./resources/planets/P0_updoot.png");
    _SpriteLibrary_Planet_spritelist[index++] = CreateSprite("./resources/planets/P1_updoot.png");
    _SpriteLibrary_Planet_spritelist[index++] = CreateSprite("./resources/planets/P2_updoot.png");
    _SpriteLibrary_Planet_spritelist[index++] = CreateSprite("./resources/planets/P3_updoot.png");
    _SpriteLibrary_Planet_spritelist[index++] = CreateSprite("./resources/planets/P4_updoot.png");


    // === asteroid sprites ===
    _SpriteLibrary_Asteroid_spritelist = malloc(sizeof(Sprite*) * _SPRITELIBRARY_ASTEROID_SPRITELIST_LENGTH);
    index = 0;
    _SpriteLibrary_Asteroid_spritelist[index++] = CreateSprite("resources/asteroids/A0.png");
    _SpriteLibrary_Asteroid_spritelist[index++] = CreateSprite("resources/asteroids/A1.png");
    _SpriteLibrary_Asteroid_spritelist[index++] = CreateSprite("resources/asteroids/A2.png");
    _SpriteLibrary_Asteroid_spritelist[index++] = CreateSprite("resources/asteroids/A3_updoot.png");





    // did it
    return 0;
}

int _SpriteLibrary_DestroySprites(){
    // ...






    // === asteroid sprites ===
    for (int i = 0; i < _SPRITELIBRARY_ASTEROID_SPRITELIST_LENGTH; i++) {
        DestroySprite(_SpriteLibrary_Asteroid_spritelist[i]);
        _SpriteLibrary_Asteroid_spritelist[i] = 0;
    }
    free(_SpriteLibrary_Asteroid_spritelist);
    _SpriteLibrary_Asteroid_spritelist = 0;


    // === planet sprites ===
    for(int i = 0; i < _SPRITELIBRARY_PLANET_SPRITELIST_LENGTH; i++){
        DestroySprite(_SpriteLibrary_Planet_spritelist[i]);
        _SpriteLibrary_Planet_spritelist[i] = 0;
    }
    free(_SpriteLibrary_Planet_spritelist);
    _SpriteLibrary_Planet_spritelist = 0;


    // === star sprites ===
    for(int i = 0; i < _SPRITELIBRARY_STAR_SPRITELIST_LENGTH; i++){
        DestroySprite(_SpriteLibrary_Star_spriteList[i]);
        _SpriteLibrary_Star_spriteList[i] = 0;
    }
    free(_SpriteLibrary_Star_spriteList);
    _SpriteLibrary_Star_spriteList = 0;


    // === wormhole sprites ===
    for(int i = 0; i < _SPRITELIBRARY_WORMHOLE_SPRITELIST_LENGTH; i++){
        DestroySprite(_SpriteLibrary_Wormhole_spriteList[i]);
        _SpriteLibrary_Wormhole_spriteList[i] = 0;
    }
    free(_SpriteLibrary_Wormhole_spriteList);
    _SpriteLibrary_Wormhole_spriteList = 0;


    // === player sprite ===
    DestroySprite(_SpriteLibrary_Player_ShipSprite);
    _SpriteLibrary_Player_ShipSprite = 0;


    // === title sprite ===
    // DestroySprite(_SpriteLibrary_Title_AsteroidSprite);
    // _SpriteLibrary_Title_AsteroidSprite = 0;



    // did it
    return 0;
}








