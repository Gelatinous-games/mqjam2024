// a registry of all objects. do an #include for it here.

enum OBJECT_FLAG {
    PLAYER_OBJECT = 1,
    ENEMY_OBJECT = 2,
    NEUTRAL_OBJECT = 4,
    ASTEROID = 8,
    GRAVITY_WELL = 16,
    UNDEFINED = 32
};

#include "src/objs/Object_Example.c"