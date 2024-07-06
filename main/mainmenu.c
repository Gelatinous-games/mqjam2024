
#include "raylib.h"


// project header includes
#include "src/settings.h"
#include "src/sound.h"


#include "src/base.h"
#include "src/obj_register.h"

#define BUTTON_FONT_SIZE 20
#define BUTTONLABELPADDING 10


// uses top left for text

Vector2 startButtonPos;
Vector2 startButtonSize;
Rectangle startButtonBounds;

Vector2 startButtonTextPos;
Vector2 startButtonTextSize;
Rectangle startButtonTextBounds;
const char *startButtonLabel = "START";

int _MainMenu_Init(){
    // button label
    startButtonTextSize = (Vector2){
       MeasureText(startButtonLabel, BUTTON_FONT_SIZE), BUTTON_FONT_SIZE
    };
    startButtonTextPos = (Vector2){
        WINDOW_WIDTH/2, WINDOW_HEIGHT/2
    };
    startButtonTextBounds = (Rectangle){
        startButtonTextPos.x - startButtonTextSize.x/2, startButtonTextPos.y - startButtonTextSize.y/2,
        startButtonTextSize.x, startButtonTextSize.y
    };
    // button
    startButtonPos = (Vector2){
        WINDOW_WIDTH/2, WINDOW_HEIGHT/2
    };
    startButtonSize = (Vector2){
       startButtonTextSize.x+(2*BUTTONLABELPADDING), startButtonTextSize.y+(2*BUTTONLABELPADDING)
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

    DrawText(startButtonLabel, startButtonTextPos.x, startButtonTextPos.y, BUTTONLABELPADDING, WHITE);


    return 0;
}

int _MainMenu_Cleanup(){
    // ...
    return 0;
}