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
    FLAG_PARTICLE = 64
};

enum LAYER_ID {
    LAYER_BACKGROUND_STARSCAPE = 0,
    LAYER_BACKGROUND_OBJECTS = 1,
    LAYER_BLACKHOLE = 2,
    LAYER_ASTEROIDS = 3,
    LAYER_STARS = 4,
    LAYER_PLAYER = 5,
    LAYER_PARTICLES = 6,
    LAYER_GUI = 7
};
