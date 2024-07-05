#pragma once
// a registry of all objects. do an #include for it here.
// we should change the name of this if it doesnt include all obj code files. can we still do that?

enum OBJECT_FLAG {
    FLAG_PLAYER_OBJECT = 1,
    FLAG_ENEMY_OBJECT = 2,
    FLAG_NEUTRAL_OBJECT = 4,
    FLAG_ASTEROID = 8,
    FLAG_GRAVITY_WELL = 16,
    FLAG_UNDEFINED_OBJ = 32,
    FLAG_PARTICLE = 64,
    FLAG_BACKGROUND = 128,
    FLAG_WORMHOLE = 256,
};

enum LAYER_ID {
    LAYER_BACKGROUND_STARSCAPE_0 = 0,
    LAYER_BACKGROUND_STARSCAPE_1 = 1,
    LAYER_BACKGROUND_STARSCAPE_2 = 2,
    LAYER_BACKGROUND_OBJECTS = 3,
    LAYER_WORMHOLE = 4,
    LAYER_ASTEROIDS = 5,
    LAYER_STARS = 6,
    LAYER_PARTICLES = 7,
    LAYER_PLAYER = 8,
    LAYER_GUI = 9,

    // DO NOT, I REPEAT, DO NOT REMOVE OR OVERWRITE LAYER_BOOKEND!
    // THIS SHOULD ALWAYS BE AT THE END OF THE ENUM!!! DO NOT REMOVE OR OVERWRITE ME!!! 
    // If any of the above layers are written out-of-order, this will not work properly. If you have 
    // defined layers out-of-order, set LAYER_BOOKEND to the number of layers.
    // If they're all defined in order, LAYER_BOOKEND will be set to the number of layers automatically,
    // so you can relax.
    LAYER_BOOKEND       
};
