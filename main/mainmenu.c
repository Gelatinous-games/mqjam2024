
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
Rectangle startButtonBoundsBG;
Rectangle startButtonBoundsOutline;

Vector2 startButtonTextPos;
Vector2 startButtonTextSize;
Rectangle startButtonTextBounds;
const char *startButtonLabel = "START [ SPACEBAR ]";


Color ButtonColorBG;
Color ButtonColorHover;

int _MainMenu_Init();
int _MainMenu_Update(float DeltaTime);
int _MainMenu_Draw();
int _MainMenu_Cleanup();

void _MainMenu_HandleStartGame();

int _MainMenu_MouseOverButton(Rectangle buttonBounds);


int _MainMenu_Init(){

    ButtonColorBG = DARKGRAY;
    ButtonColorHover = DARKBLUE;

    // button label
    startButtonTextSize = (Vector2){
       MeasureText(startButtonLabel, BUTTON_FONT_SIZE), BUTTON_FONT_SIZE
    };
    startButtonTextPos = (Vector2){
        (WINDOW_WIDTH-(startButtonTextSize.x/2))/2, (WINDOW_HEIGHT-(startButtonTextSize.y/2))/2
    };
    startButtonTextBounds = (Rectangle){
        startButtonTextPos.x, startButtonTextPos.y,
        startButtonTextSize.x, startButtonTextSize.y
    };
    // button
    startButtonSize = (Vector2){
       startButtonTextSize.x, startButtonTextSize.y
    };
    startButtonPos = (Vector2){
        (WINDOW_WIDTH-startButtonSize.x)/2, (WINDOW_HEIGHT-startButtonSize.y)/2
    };
    startButtonBoundsOutline = (Rectangle){
        startButtonPos.x-(BUTTONLABELPADDING/2), startButtonPos.y-(BUTTONLABELPADDING/2),
        startButtonSize.x+(BUTTONLABELPADDING), startButtonSize.y+(BUTTONLABELPADDING)
    };
    startButtonBoundsBG = (Rectangle){
        startButtonPos.x-BUTTONLABELPADDING, startButtonPos.y-BUTTONLABELPADDING,
        startButtonSize.x+(2*BUTTONLABELPADDING), startButtonSize.y+(2*BUTTONLABELPADDING)
    };



    return 0;
}

int _MainMenu_Update(float DeltaTime){
    if(
        ( IsKeyDown(KEY_SPACE) ) ||
        ( _MainMenu_MouseOverButton(startButtonBoundsBG) && IsMouseButtonDown(MOUSE_BUTTON_LEFT) )
    ){
        _MainMenu_HandleStartGame();
    }
    // ...
    return 0;
}

int _MainMenu_Draw(){
    // ...
    
    Color StartButtonColor = (_MainMenu_MouseOverButton(startButtonBoundsBG))? ButtonColorHover : ButtonColorBG;

    DrawRectangleRec(startButtonBoundsBG, StartButtonColor);
    DrawRectangleRoundedLines(startButtonBoundsOutline, 0.0f, 4, 3.0f, BLACK);

    DrawText(startButtonLabel, startButtonTextPos.x, startButtonTextPos.y, BUTTONLABELPADDING, WHITE);


    return 0;
}

int _MainMenu_Cleanup(){
    // ...
    return 0;
}




void _MainMenu_HandleStartGame(){
    //...
    CURRENT_GAME_SCENE_STATE = GAME_SCENE_STATE_INGAME;
}



int _MainMenu_MouseOverButton(Rectangle buttonBounds){
    Vector2 mousePosition = GetMousePosition();
    if(
        ( buttonBounds.x <= mousePosition.x && ( buttonBounds.x+buttonBounds.width  ) >= mousePosition.x ) &&
        ( buttonBounds.y <= mousePosition.y && ( buttonBounds.y+buttonBounds.height ) >= mousePosition.y )
    ){
        return 1;
    }

    return 0;
}