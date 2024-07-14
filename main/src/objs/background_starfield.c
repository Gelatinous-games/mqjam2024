#include "background_starfield.h"

int _BackgroundStars_Init(void *self, float DeltaTime)
{

    SetRandomSeed(GAME_TIME);
    // BACKGROUNDSTARFIELD_DATA->randomSeed =
    BACKGROUNDSTARFIELD_DATA->maxStars = 30;
    BACKGROUNDSTARFIELD_DATA->minStars = 10;
    BACKGROUNDSTARFIELD_DATA->numStars = GetRandomValue(BACKGROUNDSTARFIELD_DATA->minStars, BACKGROUNDSTARFIELD_DATA->maxStars);
    // get the random sequence for star position
    BACKGROUNDSTARFIELD_DATA->listRandomSequence = LoadRandomSequence(BACKGROUNDSTARFIELD_DATA->numStars * 2, 100, BACKGROUNDSTARFIELD_DATA->ObScale * (cameraBounds.x + 1) * 100);
    THIS->position = cameraPosition;
    BACKGROUNDSTARFIELD_DATA->starSize = 0.1f;
    BACKGROUNDSTARFIELD_DATA->tailLength = 30;

    // BACKGROUNDSTARFIELD_DATA->ObScale = scaleF;

    GetObjectWithFlagsExact(FLAG_PLAYER_OBJECT, 0, &player); // getting the player.
                                                             // Create the array of stars to hole their posistions.
    BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL = (BGStar_Data **)malloc(sizeof(BGStar_Data *) * BACKGROUNDSTARFIELD_DATA->numStars);
    for (int i = 0; i < BACKGROUNDSTARFIELD_DATA->numStars; ++i)
    {
        BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL[i] = malloc(sizeof(BGStar_Data));

        // laggy as all henk
        // // // remember it exists,
        // // BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL[i]->colour;
        // // now scribble over it
        // _PALETTE_Sprite_TintColor_brushes( &(BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL[i]->colour) );
        BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL[i]->colour = CLITERAL(Color){253, 249, 200, 100}; // yellow
    }
    return 0;
}

int _BackgroundStars_Update(void *self, float DeltaTime)
{
    // get the player's velocity
    THIS->velocity = (Vector2){-1.0f * ((cameraVelocity.x + PLAYER_OBJECT_REF->velocity.x)/2.0f), -0.5f * ((cameraVelocity.y + PLAYER_OBJECT_REF->velocity.y)/2.0f)};

    // move the stars across the screen
    for (int i = 0; i < BACKGROUNDSTARFIELD_DATA->numStars; i++)
    {
        BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL[i]->position = Vector2Add(BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL[i]->position, Vector2Scale(THIS->velocity, DeltaTime * BACKGROUNDSTARFIELD_DATA->ObScale));
    }
    // THIS->position = Vector2Add(THIS->position, Vector2Scale(THIS->velocity, DeltaTime));

    return 0;
}

int _BackgroundStars_Draw(void *self, float DeltaTime)
{
    _BackgroundStars_Populate(self);

    return 0;
}

int _BackgroundStars_Destroy(void *self, float DeltaTime)
{
    // free our data struct here. free anything contained.
    // free(BACKGROUNDSTARFIELD_DATA->sprite);
    UnloadRandomSequence(BACKGROUNDSTARFIELD_DATA->listRandomSequence);
    if (BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL)
    {
        for (int i = 0; i < BACKGROUNDSTARFIELD_DATA->numStars; ++i)
        {
            if ((BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL[i])){
                // ...
                free(BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL[i]);
                BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL[i] = 0;
            }

        }
        free(BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL);
        BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL = 0;
    }

    free(BACKGROUNDSTARFIELD_DATA);
    THIS->data_struct = 0;

    return 0;
}

GameObj_Base *CreateBackgroundStars(enum LAYER_ID layer, float objectScale)
{
    // scaleFactor
    scaleF = objectScale;
    GameObj_Base *obj_ptr = malloc(sizeof(GameObj_Base));

    // ============================================================
    // ==== setup the data scruct data
    obj_ptr->data_struct = malloc(sizeof(BackgroundStarField_Data));

    BackgroundStarField_Data *tmp = obj_ptr->data_struct;
    tmp->ObScale = objectScale;

    // ============================================================

    obj_ptr->Init_Func = &_BackgroundStars_Init;
    obj_ptr->Update_Func = &_BackgroundStars_Update;
    obj_ptr->Draw_Func = &_BackgroundStars_Draw;
    obj_ptr->Destroy_Func = &_BackgroundStars_Destroy;
    // ============================================================

    obj_ptr->awaitDestroy = 0;

    // properly set up flags here (bitwise)
    // consult the flag file (flags.md) for information on what each flag is.
    obj_ptr->flags = FLAG_BACKGROUND;

    obj_ptr->currentLayer = layer;

    // initialize vectors.
    obj_ptr->position = Vector2Zero();
    obj_ptr->velocity = Vector2Zero();
    obj_ptr->size = (Vector2){2, 2};

    return obj_ptr;
}





void _BackgroundStars_Populate(void *self)
{
    // BackgroundStarField_Data *data = THIS->data_struct;
    scaleFactor = (Vector2){BACKGROUNDSTARFIELD_DATA->ObScale, BACKGROUNDSTARFIELD_DATA->ObScale};
    // get the tail lenth with player velocity. ma
    int tailLength = Vector2Length(player->velocity) * 0.666667f;
    tailLength = tailLength == 0 ? 1 : tailLength;
    tailLength = tailLength < 0 ? -1 * tailLength : tailLength;

    for (int j = 0; j < BACKGROUNDSTARFIELD_DATA->numStars; ++j) // populate the screen with stars at random posistion
    {
        // set star's random seeded posistions
        float xVal = 0.01 * (float)BACKGROUNDSTARFIELD_DATA->listRandomSequence[j]; // add the tail
        float yVal = 0.01 * (float)BACKGROUNDSTARFIELD_DATA->listRandomSequence[BACKGROUNDSTARFIELD_DATA->numStars + j];

        // add the player velocity to the stars. make the stars move
        Vector2 pos = Vector2Subtract(BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL[j]->position, (Vector2){xVal, yVal});

        // camera left screen bounds reposition the start to the right
        if (pos.x < -16.0f * BACKGROUNDSTARFIELD_DATA->ObScale)
        {
            BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL[j]->position = (Vector2){16, 0};
        }

        // scale the pos with the screenFactor
        // pos = Vector2Scale(pos, BACKGROUNDSTARFIELD_DATA->ObScale);

        // draw STAR
        Vector2 v3 = (Vector2){pos.x, pos.y};
        Vector2 v2 = (Vector2){pos.x, pos.y - (BACKGROUNDSTARFIELD_DATA->starSize)};
        Vector2 v1 = (Vector2){pos.x + 1 * tailLength, pos.y - (BACKGROUNDSTARFIELD_DATA->starSize) * 3 * -(player->velocity.y)}; // tail point;
        RenderTriangleAbsolute(v1, v2, v3, BACKGROUNDSTARFIELD_DATA->BGSTAR_POOL[j]->colour);
    }
}