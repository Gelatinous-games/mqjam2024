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
    sprintf(buffer,"%d",ms);
    DrawText(buffer,30,40,10);
}