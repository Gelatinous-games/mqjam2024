#include "SpriteLibrary.h"













int _SpriteLibrary_LoadSprites(){
    int index = 0;

    // ============================= title / player sprites =============================

    // _SpriteLibrary_Title_AsteroidSprite = CreateSprite("resources/asteroids/A3.png");

    _SpriteLibrary_Player_ShipSprite = CreateSprite("resources/spaceship_outlined_v2.png");


    // ============================= wormhole sprites =============================
    _SpriteLibrary_Wormhole_spriteList = (Sprite **)malloc(sizeof(Sprite *) * _SPRITELIBRARY_WORMHOLE_SPRITELIST_LENGTH);
    index = 0;
    _SpriteLibrary_Wormhole_spriteList[index++] = CreateSprite("resources/wormhole/w1.png");
    _SpriteLibrary_Wormhole_spriteList[index++] = CreateSprite("resources/wormhole/w2.png");
    _SpriteLibrary_Wormhole_spriteList[index++] = CreateSprite("resources/wormhole/w3.png");


    // ============================= star sprites =============================
    _SpriteLibrary_Star_spriteList = malloc(sizeof(Sprite*) * _SPRITELIBRARY_STAR_SPRITELIST_LENGTH);
    index = 0;
    // _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S0.png"); // first
    // _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S1.png"); // second
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S2.png");
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S3.png");
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S4.png"); // blackhole
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S5.png");
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S6.png");
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S7.png");
    _SpriteLibrary_Star_spriteList[index++] = CreateSprite("resources/stars/S8.png"); // blackhole


    // ============================= planet sprites =============================
    // printf("%s\n","loading planets");
    _SpriteLibrary_Planet_spritelist = (Sprite **)malloc(_SPRITELIBRARY_PLANET_SPRITELIST_LENGTH * sizeof(Sprite*));
    index = 0;
    _SpriteLibrary_Planet_spritelist[index++] = CreateSprite("./resources/planets/P0_updoot.png");
    _SpriteLibrary_Planet_spritelist[index++] = CreateSprite("./resources/planets/P1_updoot.png");
    _SpriteLibrary_Planet_spritelist[index++] = CreateSprite("./resources/planets/P2_updoot.png");
    _SpriteLibrary_Planet_spritelist[index++] = CreateSprite("./resources/planets/P3_updoot.png");
    _SpriteLibrary_Planet_spritelist[index++] = CreateSprite("./resources/planets/P4_updoot.png");


    // ============================= asteroid sprites =============================
    _SpriteLibrary_Asteroid_spritelist = malloc(sizeof(Sprite*) * _SPRITELIBRARY_ASTEROID_SPRITELIST_LENGTH);
    index = 0;
    _SpriteLibrary_Asteroid_spritelist[index++] = CreateSprite("resources/asteroids/A0.png");
    _SpriteLibrary_Asteroid_spritelist[index++] = CreateSprite("resources/asteroids/A1.png");
    _SpriteLibrary_Asteroid_spritelist[index++] = CreateSprite("resources/asteroids/A2.png");
    _SpriteLibrary_Asteroid_spritelist[index++] = CreateSprite("resources/asteroids/A3_updoot.png");
    _SpriteLibrary_Asteroid_spritelist[index++] = CreateSprite("resources/asteroids/C0.png");
    _SpriteLibrary_Asteroid_spritelist[index++] = CreateSprite("resources/asteroids/C1.png");
    _SpriteLibrary_Asteroid_spritelist[index++] = CreateSprite("resources/asteroids/C2.png");
    _SpriteLibrary_Asteroid_spritelist[index++] = CreateSprite("resources/asteroids/C3.png");


    // ============================= background sprites =============================
    // === layer 0 ===
    _SpriteLibrary_backgroundLayer0_spritelist = (Sprite **)malloc(_SPRITELIBRARY_BACKGROUNDLAYER0_SPRITELIST_LENGTH * sizeof(Sprite *));
    index = 0;
    // artwork 9
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/background/bg_art_01_noBG.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/background/bg_art_02_noBG.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/background/bg_art_03_noBG.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/background/bg_art_04_noBG.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/background/bg_art_05_noBG.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/background/bg_art_06_noBG.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/background/bg_art_07_noBG.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/background/bg_art_08_noBG.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/background/bg_art_09_noBG.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/background/bg_art_11_noBG.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/background/bg_art_12_noBG.png");

    // medium 6
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/brushes/brush_medium_dust_01.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/brushes/brush_medium_dust_02.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/brushes/brush_medium_dust_03.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/brushes/brush_medium_glitter_01.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/brushes/brush_medium_glitter_02.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/brushes/brush_medium_glow_01.png");

    // large 3
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/brushes/brush_large_dust_01.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/brushes/brush_large_dust_02.png");
    _SpriteLibrary_backgroundLayer0_spritelist[index++] = CreateSprite("resources/brushes/brush_large_dust_03.png");

    // === layer 1 ===
    _SpriteLibrary_backgroundLayer1_spritelist = (Sprite **)malloc(_SPRITELIBRARY_BACKGROUNDLAYER1_SPRITELIST_LENGTH * sizeof(Sprite *));
    index = 0;
    // artwork 1
    _SpriteLibrary_backgroundLayer1_spritelist[index++] = CreateSprite("resources/background/bg_art_10.png");
    // small 2
    _SpriteLibrary_backgroundLayer1_spritelist[index++] = CreateSprite("resources/brushes/brush_small_glitter_01.png");
    _SpriteLibrary_backgroundLayer1_spritelist[index++] = CreateSprite("resources/brushes/brush_small_glitter_02.png");
    // medium 1
    _SpriteLibrary_backgroundLayer1_spritelist[index++] = CreateSprite("resources/brushes/brush_medium_flare_01.png");
    // large 1
    _SpriteLibrary_backgroundLayer1_spritelist[index++] = CreateSprite("resources/brushes/brush_large_flare_01.png");
    
    // === layer 2 ===
    _SpriteLibrary_backgroundLayer2_spritelist = (Sprite **)malloc(_SPRITELIBRARY_BACKGROUNDLAYER2_SPRITELIST_LENGTH * sizeof(Sprite *));
    index = 0;
    // small 3
    _SpriteLibrary_backgroundLayer2_spritelist[index++] = CreateSprite("resources/brushes/brush_small_star_01.png");
    _SpriteLibrary_backgroundLayer2_spritelist[index++] = CreateSprite("resources/brushes/brush_small_star_02.png");
    _SpriteLibrary_backgroundLayer2_spritelist[index++] = CreateSprite("resources/brushes/brush_small_star_03.png");
    // large 6
    _SpriteLibrary_backgroundLayer2_spritelist[index++] = CreateSprite("resources/brushes/brush_large_constellation_01.png");
    _SpriteLibrary_backgroundLayer2_spritelist[index++] = CreateSprite("resources/brushes/brush_large_constellation_02.png");
    _SpriteLibrary_backgroundLayer2_spritelist[index++] = CreateSprite("resources/brushes/brush_large_constellation_03.png");
    _SpriteLibrary_backgroundLayer2_spritelist[index++] = CreateSprite("resources/brushes/brush_large_constellation_04.png");
    _SpriteLibrary_backgroundLayer2_spritelist[index++] = CreateSprite("resources/brushes/brush_large_constellation_05.png");
    _SpriteLibrary_backgroundLayer2_spritelist[index++] = CreateSprite("resources/brushes/brush_large_constellation_06.png");

    // === layer 3 ===
    _SpriteLibrary_backgroundLayer3_spritelist = (Sprite **)malloc(_SPRITELIBRARY_BACKGROUNDLAYER3_SPRITELIST_LENGTH * sizeof(Sprite *));
    index = 0;
    _SpriteLibrary_backgroundLayer3_spritelist[index++] = CreateSprite("resources/dust/dust_cloud_long_01_halfres.png");
    _SpriteLibrary_backgroundLayer3_spritelist[index++] = CreateSprite("resources/dust/dust_cloud_long_02_halfres.png");
    _SpriteLibrary_backgroundLayer3_spritelist[index++] = CreateSprite("resources/dust/dust_cloud_long_03_halfres.png");
    _SpriteLibrary_backgroundLayer3_spritelist[index++] = CreateSprite("resources/dust/dust_cloud_long_04_halfres.png");
    _SpriteLibrary_backgroundLayer3_spritelist[index++] = CreateSprite("resources/dust/dust_cloud_long_05_halfres.png");
    _SpriteLibrary_backgroundLayer3_spritelist[index++] = CreateSprite("resources/dust/dust_cloud_long_06_halfres.png");
    _SpriteLibrary_backgroundLayer3_spritelist[index++] = CreateSprite("resources/dust/dust_cloud_long_07_halfres.png");
    _SpriteLibrary_backgroundLayer3_spritelist[index++] = CreateSprite("resources/dust/dust_cloud_long_08_halfres.png");
    _SpriteLibrary_backgroundLayer3_spritelist[index++] = CreateSprite("resources/dust/dust_cloud_long_09_halfres.png");
    _SpriteLibrary_backgroundLayer3_spritelist[index++] = CreateSprite("resources/dust/dust_cloud_short_01_halfres.png");
    _SpriteLibrary_backgroundLayer3_spritelist[index++] = CreateSprite("resources/dust/dust_cloud_short_02_halfres.png");


    // ============================= healthbar sprites =============================
    _SpriteLibrary_HealthBarFrame_sprite = CreateSprite("resources/gasbar/gasbar_frame.png");
    _SpriteLibrary_HealthBarOverlay_sprite = CreateSprite("resources/gasbar/gasbar_overlay.png");

    _SpriteLibrary_HealthBarGasLevel_spritelist = (Sprite **)malloc(sizeof(Sprite*) * _SPRITELIBRARY_HEALTHBARGASLEVEL_SPRITELIST_LENGTH);
    index = 0;
    _SpriteLibrary_HealthBarGasLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_gaslevel_01.png");
    _SpriteLibrary_HealthBarGasLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_gaslevel_02.png");
    _SpriteLibrary_HealthBarGasLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_gaslevel_03.png");
    _SpriteLibrary_HealthBarGasLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_gaslevel_04.png");
    _SpriteLibrary_HealthBarGasLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_gaslevel_05.png");
    _SpriteLibrary_HealthBarGasLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_gaslevel_06.png");
    _SpriteLibrary_HealthBarGasLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_gaslevel_07.png");
    _SpriteLibrary_HealthBarGasLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_gaslevel_08.png");
    _SpriteLibrary_HealthBarGasLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_gaslevel_09.png");
    _SpriteLibrary_HealthBarGasLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_gaslevel_10.png");

    _SpriteLibrary_HealthBarShieldLevel_spritelist = (Sprite **)malloc(sizeof(Sprite*) * _SPRITELIBRARY_HEALTHBARSHIELDLEVEL_SPRITELIST_LENGTH);
    index = 0;
    _SpriteLibrary_HealthBarShieldLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_shieldlevel_01.png");
    _SpriteLibrary_HealthBarShieldLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_shieldlevel_02.png");
    _SpriteLibrary_HealthBarShieldLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_shieldlevel_03.png");
    _SpriteLibrary_HealthBarShieldLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_shieldlevel_04.png");
    _SpriteLibrary_HealthBarShieldLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_shieldlevel_05.png");
    _SpriteLibrary_HealthBarShieldLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_shieldlevel_06.png");
    _SpriteLibrary_HealthBarShieldLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_shieldlevel_07.png");
    _SpriteLibrary_HealthBarShieldLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_shieldlevel_08.png");
    _SpriteLibrary_HealthBarShieldLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_shieldlevel_09.png");
    _SpriteLibrary_HealthBarShieldLevel_spritelist[index++] = CreateSprite("resources/gasbar/gasbar_shieldlevel_10.png");




    // did it
    return 0;
}

int _SpriteLibrary_DestroySprites(){
    // ...

    // ============================= healthbar sprites =============================

    DestroySprite(_SpriteLibrary_HealthBarFrame_sprite);
    _SpriteLibrary_HealthBarFrame_sprite = 0;
    DestroySprite(_SpriteLibrary_HealthBarOverlay_sprite);
    _SpriteLibrary_HealthBarOverlay_sprite = 0;

    for(int i =  0; i < _SPRITELIBRARY_HEALTHBARGASLEVEL_SPRITELIST_LENGTH; i++){
        DestroySprite(_SpriteLibrary_HealthBarGasLevel_spritelist[i]);
        _SpriteLibrary_HealthBarGasLevel_spritelist[i] = 0;
    }
    free(_SpriteLibrary_HealthBarGasLevel_spritelist);
    _SpriteLibrary_HealthBarGasLevel_spritelist = 0;

    for(int i =  0; i < _SPRITELIBRARY_HEALTHBARSHIELDLEVEL_SPRITELIST_LENGTH; i++){
        DestroySprite(_SpriteLibrary_HealthBarShieldLevel_spritelist[i]);
        _SpriteLibrary_HealthBarShieldLevel_spritelist[i] = 0;
    }
    free(_SpriteLibrary_HealthBarShieldLevel_spritelist);
    _SpriteLibrary_HealthBarShieldLevel_spritelist = 0;


    // ============================= background sprites =============================
    // === layer 3 ===
    for (int i = 0; i < _SPRITELIBRARY_BACKGROUNDLAYER3_SPRITELIST_LENGTH; i++) {
        DestroySprite(_SpriteLibrary_backgroundLayer3_spritelist[i]);
        _SpriteLibrary_backgroundLayer3_spritelist[i] = 0;
    }
    free(_SpriteLibrary_backgroundLayer3_spritelist);
    _SpriteLibrary_backgroundLayer3_spritelist = 0;
    
    // === layer 2 ===
    for (int i = 0; i < _SPRITELIBRARY_BACKGROUNDLAYER2_SPRITELIST_LENGTH; i++) {
        DestroySprite(_SpriteLibrary_backgroundLayer2_spritelist[i]);
        _SpriteLibrary_backgroundLayer2_spritelist[i] = 0;
    }
    free(_SpriteLibrary_backgroundLayer2_spritelist);
    _SpriteLibrary_backgroundLayer2_spritelist = 0;
    
    // === layer 1 ===
    for (int i = 0; i < _SPRITELIBRARY_BACKGROUNDLAYER1_SPRITELIST_LENGTH; i++) {
        DestroySprite(_SpriteLibrary_backgroundLayer1_spritelist[i]);
        _SpriteLibrary_backgroundLayer1_spritelist[i] = 0;
    }
    free(_SpriteLibrary_backgroundLayer1_spritelist);
    _SpriteLibrary_backgroundLayer1_spritelist = 0;

    // === layer 0 ===
    for (int i = 0; i < _SPRITELIBRARY_BACKGROUNDLAYER0_SPRITELIST_LENGTH; i++) {
        DestroySprite(_SpriteLibrary_backgroundLayer0_spritelist[i]);
        _SpriteLibrary_backgroundLayer0_spritelist[i] = 0;
    }
    free(_SpriteLibrary_backgroundLayer0_spritelist);
    _SpriteLibrary_backgroundLayer0_spritelist = 0;



    // ============================= asteroid sprites =============================
    for (int i = 0; i < _SPRITELIBRARY_ASTEROID_SPRITELIST_LENGTH; i++) {
        DestroySprite(_SpriteLibrary_Asteroid_spritelist[i]);
        _SpriteLibrary_Asteroid_spritelist[i] = 0;
    }
    free(_SpriteLibrary_Asteroid_spritelist);
    _SpriteLibrary_Asteroid_spritelist = 0;


    // ============================= planet sprites =============================
    for(int i = 0; i < _SPRITELIBRARY_PLANET_SPRITELIST_LENGTH; i++){
        DestroySprite(_SpriteLibrary_Planet_spritelist[i]);
        _SpriteLibrary_Planet_spritelist[i] = 0;
    }
    free(_SpriteLibrary_Planet_spritelist);
    _SpriteLibrary_Planet_spritelist = 0;


    // ============================= star sprites =============================
    for(int i = 0; i < _SPRITELIBRARY_STAR_SPRITELIST_LENGTH; i++){
        DestroySprite(_SpriteLibrary_Star_spriteList[i]);
        _SpriteLibrary_Star_spriteList[i] = 0;
    }
    free(_SpriteLibrary_Star_spriteList);
    _SpriteLibrary_Star_spriteList = 0;


    // ============================= wormhole sprites =============================
    for(int i = 0; i < _SPRITELIBRARY_WORMHOLE_SPRITELIST_LENGTH; i++){
        DestroySprite(_SpriteLibrary_Wormhole_spriteList[i]);
        _SpriteLibrary_Wormhole_spriteList[i] = 0;
    }
    free(_SpriteLibrary_Wormhole_spriteList);
    _SpriteLibrary_Wormhole_spriteList = 0;


    // ============================= player sprite =============================
    DestroySprite(_SpriteLibrary_Player_ShipSprite);
    _SpriteLibrary_Player_ShipSprite = 0;


    // ============================= title sprite =============================
    // DestroySprite(_SpriteLibrary_Title_AsteroidSprite);
    // _SpriteLibrary_Title_AsteroidSprite = 0;



    // did it
    return 0;
}








