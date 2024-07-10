#include <stdio.h>
#include <time.h>

#include "raylib.h"

#include "./settings.h"
#include "./misc_util.h"

int storedMS = 0;
char doTick = 1;

void drawTimer() {
    struct timeval current;
    gettimeofday(&current, NULL);
    if (doTick) {
        storedMS = (current.tv_sec - timerStart.tv_sec) * 1000000 + current.tv_usec - timerStart.tv_usec;
    }
    int ms = storedMS;
    char buffer[50];
    int timerMins = ms/60000000;
    int timerSecs = ms/1000000;
    int timerMs = ms/1000;
    sprintf(buffer,"%02d:%02d.%03d",timerMins,timerSecs%60,timerMs%1000);
    DrawText(buffer,5,5,36,WHITE);
}