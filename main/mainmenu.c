
#include "raylib.h"


// project header includes
#include "src/settings.h"
#include "src/sound.h"


#include "src/base.h"
#include "src/obj_register.h"

Vector2 startButtonPos;
Vector2 startButtonSize;
Rectangle startButtonBounds;
const char *startButtonLabel = "START";


int _MainMenu_Init(){
    // button
    startButtonPos = (Vector2){
        WINDOW_WIDTH/2, WINDOW_HEIGHT/2
    };
    startButtonSize = (Vector2){
       200, 50
    };

    startButtonBounds = (Rectangle){
        startButtonPos.x - startButtonSize.x/2, startButtonPos.y - startButtonSize.y/2,
        startButtonSize.x, startButtonSize.y
    };



    return 0;
}

int _MainMenu_Update(float DeltaTime){
    // ...
    return 0;
}

int _MainMenu_Draw(){
    // ...
    

    // DrawRectangleRec(startButtonBounds, WHITE);
    DrawRectangleRoundedLines(startButtonBounds, 0.0f, 4, 3.0f, DARKGRAY);

    DrawText(startButtonLabel, WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 20, WHITE);

    return 0;
}

int _MainMenu_Cleanup(){
    // ...
    return 0;
}