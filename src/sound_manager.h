#pragma once

// standard library includes
#include <stdio.h>
#include <stdlib.h>

// library includes
#include "raylib.h"
#include "raymath.h"

// project includes
#include "settings.h"
#include "misc_util.h"


enum IMPACT_TYPE {
    HULL_IMPACT,
    SHIELD_IMPACT,
    DEATH_IMPACT
};

typedef struct {
    const char *path;
    Sound track;
    float baseVolume;
    float scaleOfBaseVolume;
    int loopingTrack;
} Sound_Track;


enum TRACK_INDEX_ID {
    BGMUSIC_LOOP_ID,
    BGMENUMUSIC_SOUND_ID,
    HIT_SOUND_ID,
    THRUST_LOOP_ID,
    STAR_PROXIMITY_LOOP_ID,
    THRUST_END_ID,
    THRUST_START_ID,
    DEATH_SOUND_ID,
    BUTTON_SOUND_ID,
    SHIELD_HIT_SOUND_ID,
    SHIELD_REGEN_SOUND_ID,
    UI_HOVER_SOUND_ID,
    WIN_SOUND_ID,
    


    // REMEMBER THE BOOKEND THING, IF U DONT THEN GOTO REGISTER IDK

    TRACK_COUNT
};

static Sound_Track **TRACKS;


static float MASTER_VOLUME = 0.5f;

static int PLAYED_DEATH_SOUND_BEFORE = 0;


static void setAllTracksVolume(float volume);
static void scaleAllTracksVolume(float scale);
static void prepareSounds();
static void startSounds();
static void soundUpdate();
static void cleanupSounds();
static void playSoundOnce(int soundID);
static void setTrackVolume(int soundID, float volume);
static float getTrackVolume(int soundID);
static void SoundManager_EnableMenuMusic();
static void SoundManager_EnableGameMusic();
static void SoundManagerHandleDeath();
static void SoundManagerHandleImpact(enum IMPACT_TYPE impactTypeData);