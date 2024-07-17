#include "sound_manager.h"


static void setAllTracksVolume(float volume){
    MASTER_VOLUME = volume;
    for (int i = 0; i < TRACK_COUNT; i++)
    {
        TRACKS[i]->scaleOfBaseVolume = volume;
    }
    
    SetMasterVolume(volume);
}

static void scaleAllTracksVolume(float scale) {
    MASTER_VOLUME = MASTER_VOLUME * scale;
    for (int i = 0; i < TRACK_COUNT; i++)
    {
        TRACKS[i]->scaleOfBaseVolume = TRACKS[i]->scaleOfBaseVolume * scale;
    }
    
    SetMasterVolume(MASTER_VOLUME);
}

static void prepareSounds(){
    TRACKS = (Sound_Track **)malloc(sizeof(Sound_Track*)*TRACK_COUNT);

    // init
    TRACKS[BGMUSIC_LOOP_ID]    = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[BGMENUMUSIC_SOUND_ID] = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[HIT_SOUND_ID]        = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[THRUST_LOOP_ID]     = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[STAR_PROXIMITY_LOOP_ID] = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[THRUST_END_ID]      = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[THRUST_START_ID]    = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[DEATH_SOUND_ID]     = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[BUTTON_SOUND_ID]     = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[SHIELD_HIT_SOUND_ID]     = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[SHIELD_REGEN_SOUND_ID]     = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[UI_HOVER_SOUND_ID]     = (Sound_Track *)malloc(sizeof(Sound_Track));
    TRACKS[WIN_SOUND_ID]     = (Sound_Track *)malloc(sizeof(Sound_Track));

    // paths
    TRACKS[BGMUSIC_LOOP_ID]->path    = "resources/SFX/MQGameJam_Music_loop.wav";
    TRACKS[BGMENUMUSIC_SOUND_ID]->path    = "resources/SFX/MUSIC_muffled.wav";
    TRACKS[HIT_SOUND_ID]->path        = "resources/SFX/HitAsteroid.wav";
    TRACKS[THRUST_LOOP_ID]->path     = "resources/SFX/Thrust_loop.wav";
    TRACKS[STAR_PROXIMITY_LOOP_ID]->path = "resources/SFX/NearingStar.wav";
    TRACKS[THRUST_END_ID]->path      = "resources/SFX/Thrust_end.wav";
    TRACKS[THRUST_START_ID]->path    = "resources/SFX/Thrust_start.wav";
    TRACKS[DEATH_SOUND_ID]->path     = "resources/SFX/Death.wav";
    TRACKS[BUTTON_SOUND_ID]->path     = "resources/SFX/ButtonSelect.wav";
    TRACKS[SHIELD_HIT_SOUND_ID]->path     = "resources/SFX/ShieldHit.wav";
    TRACKS[SHIELD_REGEN_SOUND_ID]->path     = "resources/SFX/ShieldRegen.wav";
    TRACKS[UI_HOVER_SOUND_ID]->path     = "resources/SFX/UI_hover.wav";
    TRACKS[WIN_SOUND_ID]->path     = "resources/SFX/Win.wav";

    // looping
    TRACKS[BGMUSIC_LOOP_ID]->loopingTrack    = 1;
    TRACKS[BGMENUMUSIC_SOUND_ID]->loopingTrack = 1;
    TRACKS[HIT_SOUND_ID]->loopingTrack       = 0;
    TRACKS[THRUST_LOOP_ID]->loopingTrack     = 1;
    TRACKS[STAR_PROXIMITY_LOOP_ID]->loopingTrack = 1;
    TRACKS[THRUST_END_ID]->loopingTrack      = 0;
    TRACKS[THRUST_START_ID]->loopingTrack    = 0;
    TRACKS[DEATH_SOUND_ID]->loopingTrack     = 0;
    TRACKS[BUTTON_SOUND_ID]->loopingTrack     = 0;
    TRACKS[SHIELD_HIT_SOUND_ID]->loopingTrack     = 0;
    TRACKS[SHIELD_REGEN_SOUND_ID]->loopingTrack     = 0;
    TRACKS[UI_HOVER_SOUND_ID]->loopingTrack     = 0;
    TRACKS[WIN_SOUND_ID]->loopingTrack     = 0;
    // volumes
    TRACKS[BGMUSIC_LOOP_ID]->baseVolume    = 0.7f;
    TRACKS[BGMENUMUSIC_SOUND_ID]->baseVolume = 0.6f;
    TRACKS[HIT_SOUND_ID]->baseVolume        = 0.5;
    TRACKS[THRUST_LOOP_ID]->baseVolume     = 1.0f;
    TRACKS[STAR_PROXIMITY_LOOP_ID]->baseVolume = 1.0f;
    TRACKS[THRUST_END_ID]->baseVolume      = 1.0f;
    TRACKS[THRUST_START_ID]->baseVolume    = 1.0f;
    TRACKS[DEATH_SOUND_ID]->baseVolume     = 1.0f;
    TRACKS[BUTTON_SOUND_ID]->baseVolume     = 1.0f;
    TRACKS[SHIELD_HIT_SOUND_ID]->baseVolume     = 0.5f;
    TRACKS[SHIELD_REGEN_SOUND_ID]->baseVolume     = 0.5f;
    TRACKS[UI_HOVER_SOUND_ID]->baseVolume     = 1.0f;
    TRACKS[WIN_SOUND_ID]->baseVolume     = 1.0f;

    TRACKS[BGMUSIC_LOOP_ID]->scaleOfBaseVolume    = 1.0f;
    TRACKS[BGMENUMUSIC_SOUND_ID]->scaleOfBaseVolume = 0.0f;
    TRACKS[HIT_SOUND_ID]->scaleOfBaseVolume        = 0.0f;
    TRACKS[THRUST_LOOP_ID]->scaleOfBaseVolume     = 0.0f;
    TRACKS[STAR_PROXIMITY_LOOP_ID]->scaleOfBaseVolume     = 0.0f;
    TRACKS[THRUST_END_ID]->scaleOfBaseVolume      = 0.0f;
    TRACKS[THRUST_START_ID]->scaleOfBaseVolume    = 0.0f;
    TRACKS[DEATH_SOUND_ID]->scaleOfBaseVolume     = 0.0f;
    TRACKS[BUTTON_SOUND_ID]->scaleOfBaseVolume     = 0.0f;
    TRACKS[SHIELD_HIT_SOUND_ID]->scaleOfBaseVolume     = 0.0f;
    TRACKS[SHIELD_REGEN_SOUND_ID]->scaleOfBaseVolume     = 0.0f;
    TRACKS[UI_HOVER_SOUND_ID]->scaleOfBaseVolume     = 0.0f;
    TRACKS[WIN_SOUND_ID]->scaleOfBaseVolume     = 0.0f;

    // load
    TRACKS[BGMUSIC_LOOP_ID]->track     = LoadSound(TRACKS[BGMUSIC_LOOP_ID]->path);     // Load WAV audio file
    TRACKS[BGMENUMUSIC_SOUND_ID]->track = LoadSound(TRACKS[BGMENUMUSIC_SOUND_ID]->path); // Load WAV audio file
    TRACKS[HIT_SOUND_ID]->track         = LoadSound(TRACKS[HIT_SOUND_ID]->path);         // Load WAV audio file
    TRACKS[THRUST_LOOP_ID]->track      = LoadSound(TRACKS[THRUST_LOOP_ID]->path);      // Load WAV audio file
    TRACKS[STAR_PROXIMITY_LOOP_ID]->track = LoadSound(TRACKS[STAR_PROXIMITY_LOOP_ID]->path); // Load WAV audio file
    TRACKS[THRUST_END_ID]->track       = LoadSound(TRACKS[THRUST_END_ID]->path);       // Load WAV audio file
    TRACKS[THRUST_START_ID]->track     = LoadSound(TRACKS[THRUST_START_ID]->path);     // Load WAV audio file
    TRACKS[DEATH_SOUND_ID]->track      = LoadSound(TRACKS[DEATH_SOUND_ID]->path);      // Load WAV audio file
    TRACKS[BUTTON_SOUND_ID]->track      = LoadSound(TRACKS[BUTTON_SOUND_ID]->path);      // Load WAV audio file
    TRACKS[SHIELD_HIT_SOUND_ID]->track      = LoadSound(TRACKS[SHIELD_HIT_SOUND_ID]->path);      // Load WAV audio file
    TRACKS[SHIELD_REGEN_SOUND_ID]->track      = LoadSound(TRACKS[SHIELD_REGEN_SOUND_ID]->path);      // Load WAV audio file
    TRACKS[UI_HOVER_SOUND_ID]->track      = LoadSound(TRACKS[UI_HOVER_SOUND_ID]->path);      // Load WAV audio file
    TRACKS[WIN_SOUND_ID]->track      = LoadSound(TRACKS[WIN_SOUND_ID]->path);      // Load WAV audio file
}

static void startSounds(){
    for (int i = 0; i < TRACK_COUNT; i++)
    {
        /* code */
        if (TRACKS[i]->loopingTrack)
        {
            /* code */
            PlaySound(TRACKS[i]->track);      // Play WAV sound
            SetSoundVolume(TRACKS[i]->track, (TRACKS[i]->baseVolume)*(TRACKS[i]->scaleOfBaseVolume));
        }
        
    }
    
}

static void soundUpdate(){
    for (int i = 0; i < TRACK_COUNT; i++)
    {
        /* code */
        if (TRACKS[i]->loopingTrack && !IsSoundPlaying(TRACKS[i]->track))
        {
            /* code */
            PlaySound(TRACKS[i]->track);      // Play WAV sound
            SetSoundVolume(TRACKS[i]->track, (TRACKS[i]->baseVolume)*(TRACKS[i]->scaleOfBaseVolume));
        }
        
    }
}


static void cleanupSounds(){

    // unload
    UnloadSound(TRACKS[BGMUSIC_LOOP_ID]->track);     // Unload sound data
    UnloadSound(TRACKS[BGMENUMUSIC_SOUND_ID]->track);     // Unload sound data
    UnloadSound(TRACKS[HIT_SOUND_ID]->track);         // Unload sound data
    UnloadSound(TRACKS[THRUST_LOOP_ID]->track);      // Unload sound data
    UnloadSound(TRACKS[STAR_PROXIMITY_LOOP_ID]->track); // Unload sound data
    UnloadSound(TRACKS[THRUST_END_ID]->track);       // Unload sound data
    UnloadSound(TRACKS[THRUST_START_ID]->track);     // Unload sound data
    UnloadSound(TRACKS[DEATH_SOUND_ID]->track);      // Unload sound data
    UnloadSound(TRACKS[BUTTON_SOUND_ID]->track);      // Unload sound data
    UnloadSound(TRACKS[SHIELD_HIT_SOUND_ID]->track);      // Unload sound data
    UnloadSound(TRACKS[SHIELD_REGEN_SOUND_ID]->track);      // Unload sound data
    UnloadSound(TRACKS[UI_HOVER_SOUND_ID]->track);      // Unload sound data
    UnloadSound(TRACKS[WIN_SOUND_ID]->track);      // Unload sound data

    // deinit
    free(TRACKS[BGMUSIC_LOOP_ID]);
    free(TRACKS[BGMENUMUSIC_SOUND_ID]);
    free(TRACKS[HIT_SOUND_ID]);
    free(TRACKS[THRUST_LOOP_ID]);
    free(TRACKS[STAR_PROXIMITY_LOOP_ID]);
    free(TRACKS[THRUST_END_ID]);
    free(TRACKS[THRUST_START_ID]);
    free(TRACKS[DEATH_SOUND_ID]);
    free(TRACKS[BUTTON_SOUND_ID]);
    free(TRACKS[SHIELD_HIT_SOUND_ID]);
    free(TRACKS[SHIELD_REGEN_SOUND_ID]);
    free(TRACKS[UI_HOVER_SOUND_ID]);
    free(TRACKS[WIN_SOUND_ID]);


    free(TRACKS);

}

static void playSoundOnce(int soundID){
    
    PlaySound(TRACKS[soundID]->track);
    
}

static void setTrackVolume(int soundID, float volume){
    
    TRACKS[soundID]->scaleOfBaseVolume = volume;
    SetSoundVolume(TRACKS[soundID]->track, (TRACKS[soundID]->baseVolume)*(TRACKS[soundID]->scaleOfBaseVolume));
    
}

static float getTrackVolume(int soundID){
    
    return TRACKS[soundID]->scaleOfBaseVolume;
    
}

static void SoundManager_EnableMenuMusic(){
    setTrackVolume(BGMUSIC_LOOP_ID,0.0f);
    setTrackVolume(BGMENUMUSIC_SOUND_ID,1.0f);
}

static void SoundManager_EnableGameMusic(){
    setTrackVolume(BGMUSIC_LOOP_ID,1.0f);
    setTrackVolume(BGMENUMUSIC_SOUND_ID,0.0f);
}

static void SoundManagerHandleDeath(){
    // silence any impact sounds
    setTrackVolume(HIT_SOUND_ID,0.0f);
    // max volume scale
    setTrackVolume(THRUST_START_ID, 0.0f);
    setTrackVolume(THRUST_LOOP_ID, 0.0f);
    setTrackVolume(THRUST_END_ID, 5.0f);
    if (CURRENT_PLAYER_THRUST_STATE != PLAYER_STATE_NOTHRUST)
    {
        playSoundOnce(THRUST_END_ID);
    }
    
    setTrackVolume(DEATH_SOUND_ID, 1.0f);
    // tell it to play if it isnt, and never played
    if( !(IsSoundPlaying(TRACKS[DEATH_SOUND_ID]->track)) && !PLAYED_DEATH_SOUND_BEFORE ){
        playSoundOnce(DEATH_SOUND_ID);
        PLAYED_DEATH_SOUND_BEFORE = 1;
    }
}

static void SoundManagerHandleImpact(enum IMPACT_TYPE impactTypeData){
    switch(impactTypeData){
        // ..
        default:
        case DEATH_IMPACT:
        case HULL_IMPACT:
        playSoundOnce(HIT_SOUND_ID);
        break;
        case SHIELD_IMPACT:
        playSoundOnce(SHIELD_HIT_SOUND_ID);
    }
}