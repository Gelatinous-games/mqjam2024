#include <stdio.h>
#include <time.h>

#include "raylib.h"

#include "./settings.h"
#include "./misc_util.h"

unsigned initialWormholeMS = 0;
unsigned initialReturnMS = 0;
unsigned storedWormholeMS = 0;
unsigned storedReturnMS = 0;
char doTick = 1;

char *getTimerString(char *buffer, unsigned millis);
void resetTimers();

void drawTimer() {
    // colours
    Color toWormholeColor = WHITE;
    Color returnColor = DARKGRAY;
    
    // make buffers
    char toWormholeTimer[50];
    char returnTimer[50];

    
    // // gram the time stuff
    // struct timeval current;
    // gettimeofday(&current, NULL);

    // determine if updating
    if (doTick) {
        // which direction
        if(TO_WORMHOLE){
            if(initialWormholeMS==0) initialWormholeMS = currMillis()*1000;
            // ...
            storedWormholeMS = currMillis()*1000 - initialWormholeMS;
            
        }
        else {
            if(initialReturnMS==0) initialReturnMS = currMillis()*1000;
            storedReturnMS = currMillis()*1000 - initialReturnMS;
            toWormholeColor = DARKGRAY;
            returnColor = WHITE;
        }
    }
    // make the string
    getTimerString(toWormholeTimer,storedWormholeMS);
    getTimerString(returnTimer,storedReturnMS);
    // draw
    DrawText(toWormholeTimer,5,5,36,toWormholeColor);
    DrawText(returnTimer,5,45,36,returnColor);
}

char *getTimerString(char *buffer, unsigned millis){
    unsigned timerMins = millis/60000000;
    unsigned timerSecs = millis/1000000;
    unsigned timerMs = millis/1000;
    sprintf(buffer,"%02u:%02u.%03u",timerMins,timerSecs%60,timerMs%1000);
    return buffer;
}

void resetTimers(){
    // ...
    initialWormholeMS = 0;
    initialReturnMS = 0;
}