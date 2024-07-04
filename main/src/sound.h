#pragma once



// standard library includes
#include <stdio.h>
#include <stdlib.h>

// library includes
#include "raylib.h"
#include "raymath.h"

// project includes
#include "settings.h"

typedef struct {
    const char *path;
    Sound track;
    float baseVolume;
    float scaleOfBaseVolume;
} Sound_Track;

static Sound_Track **TRACKS;

#define BGMUSIC_LOOP_ID 0
#define HIT_LOOP_ID 1
#define THRUST_LOOP_ID 2
#define THRUST_END_ID 3
#define THRUST_START_ID 4
#define DEATH_SOUND_ID 5

#define TRACK_COUNT 6


static float MASTER_VOLUME = 0.5f;

static void setAllTracksVolume(float volume){
    MASTER_VOLUME = volume;
    for (int i = 0; i < TRACK_COUNT; i++)
    {
        TRACKS[i]->scaleOfBaseVolume = volume;
    }
    
    SetMasterVolume(volume);
}

static void prepareSounds(){
    TRACKS = (Sound_Track **)malloc(sizeof(Sound_Track*)*TRACK_COUNT);

    // init
    TRACKS[BGMUSIC_LOOP_ID]    = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[HIT_LOOP_ID]        = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[THRUST_LOOP_ID]     = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[THRUST_END_ID]      = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[THRUST_START_ID]    = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[DEATH_SOUND_ID]     = (Sound_Track *)malloc(sizeof(Sound_Track));

    // paths
    TRACKS[BGMUSIC_LOOP_ID]->path    = "resources/SFX/MQGameJam_Music_loop.wav";
    TRACKS[HIT_LOOP_ID]->path        = "resources/SFX/HitAsteroid.wav";
    TRACKS[THRUST_LOOP_ID]->path     = "resources/SFX/Thrust_loop.wav";
    TRACKS[THRUST_END_ID]->path      = "resources/SFX/Thrust_end.wav";
    TRACKS[THRUST_START_ID]->path    = "resources/SFX/Thrust_start.wav";
    TRACKS[DEATH_SOUND_ID]->path     = "resources/SFX/Death.wav";

    // volumes
    TRACKS[BGMUSIC_LOOP_ID]->baseVolume    = 1.0f;
    TRACKS[HIT_LOOP_ID]->baseVolume        = 1.0f;
    TRACKS[THRUST_LOOP_ID]->baseVolume     = 1.0f;
    TRACKS[THRUST_END_ID]->baseVolume      = 1.0f;
    TRACKS[THRUST_START_ID]->baseVolume    = 1.0f;
    TRACKS[DEATH_SOUND_ID]->baseVolume     = 1.0f;

    // load
    TRACKS[BGMUSIC_LOOP_ID]->track     = LoadSound(TRACKS[BGMUSIC_LOOP_ID]->path);     // Load WAV audio file
    TRACKS[HIT_LOOP_ID]->track         = LoadSound(TRACKS[HIT_LOOP_ID]->path);         // Load WAV audio file
    TRACKS[THRUST_LOOP_ID]->track      = LoadSound(TRACKS[THRUST_LOOP_ID]->path);      // Load WAV audio file
    TRACKS[THRUST_END_ID]->track       = LoadSound(TRACKS[THRUST_END_ID]->path);       // Load WAV audio file
    TRACKS[THRUST_START_ID]->track     = LoadSound(TRACKS[THRUST_START_ID]->path);     // Load WAV audio file
    TRACKS[DEATH_SOUND_ID]->track      = LoadSound(TRACKS[DEATH_SOUND_ID]->path);      // Load WAV audio file
}

static void startSounds(){
    PlaySound(TRACKS[BGMUSIC_LOOP_ID]->track);      // Play WAV sound
}


static void cleanupSounds(){

    // unload
    UnloadSound(TRACKS[BGMUSIC_LOOP_ID]->track);     // Unload sound data
    UnloadSound(TRACKS[HIT_LOOP_ID]->track);         // Unload sound data
    UnloadSound(TRACKS[THRUST_LOOP_ID]->track);      // Unload sound data
    UnloadSound(TRACKS[THRUST_END_ID]->track);       // Unload sound data
    UnloadSound(TRACKS[THRUST_START_ID]->track);     // Unload sound data
    UnloadSound(TRACKS[DEATH_SOUND_ID]->track);      // Unload sound data

    // deinit
    free(TRACKS[BGMUSIC_LOOP_ID]);
    free(TRACKS[HIT_LOOP_ID]);
    free(TRACKS[THRUST_LOOP_ID]);
    free(TRACKS[THRUST_END_ID]);
    free(TRACKS[THRUST_START_ID]);
    free(TRACKS[DEATH_SOUND_ID]);


    free(TRACKS);

}