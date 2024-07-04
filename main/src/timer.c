#include <stdio.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "./settings.h"

void drawTimer() {
    struct timeval current;
    gettimeofday(&current, NULL);
    int ms = (current.tv_sec - timerStart.tv_sec) * 1000000 + current.tv_usec - timerStart.tv_usec;
    char buffer[50];
    int timerMins = ms/60000000;
    int timerSecs = ms/1000000;
    int timerMs = ms/1000;
    sprintf(buffer,"%02d:%02d.%03d",timerMins,timerSecs%60,timerMs%1000);
    DrawText(buffer,1,1,30,WHITE);
}