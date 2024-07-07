#include "mainmenu.h"


int _MainMenu_Init(){



    ButtonColorBG = DARKGRAY;
    ButtonColorHover = DARKBLUE;

    // ************************************************************************************
    // ************************************************************************************
    // ******** START BUTTON

    // ====================================================================================
    // === button label
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

    // ====================================================================================
    // === button
    startButtonSize = (Vector2){
       startButtonTextSize.x, startButtonTextSize.y
    };
    startButtonPos = (Vector2){
        (WINDOW_WIDTH-startButtonSize.x)/2, (WINDOW_HEIGHT-startButtonSize.y)/2
    };
    startButtonBoundsOutline = (Rectangle){
        startButtonPos.x-(MENU_ELEMENT_PADDING/2), startButtonPos.y-(MENU_ELEMENT_PADDING/2),
        startButtonSize.x+(MENU_ELEMENT_PADDING), startButtonSize.y+(MENU_ELEMENT_PADDING)
    };
    startButtonBoundsBG = (Rectangle){
        startButtonPos.x-MENU_ELEMENT_PADDING, startButtonPos.y-MENU_ELEMENT_PADDING,
        startButtonSize.x+(2*MENU_ELEMENT_PADDING), startButtonSize.y+(2*MENU_ELEMENT_PADDING)
    };

    // ************************************************************************************
    // ************************************************************************************

    return 0;
}

int _MainMenu_Update(float DeltaTime){

    // ************************************************************************************
    // ************************************************************************************
    // ******** START BUTTON

    if(
        ( IsKeyDown(KEY_SPACE) ) ||
        ( MouseOverRectangle(startButtonBoundsBG) && IsMouseButtonDown(MOUSE_BUTTON_LEFT) )
    ){
        // _MainMenu_ScheduleGameStart();
        __TITLEMANAGER_REF->awaitDestroy = 1;

    }

    // ************************************************************************************
    // ************************************************************************************
    // ...
    return 0;
}

int _MainMenu_Draw(){
    // ...
    

    // ************************************************************************************
    // ************************************************************************************
    // ******** START BUTTON

    Color StartButtonColor = (MouseOverRectangle(startButtonBoundsBG))? ButtonColorHover : ButtonColorBG;

    DrawRectangleRec(startButtonBoundsBG, StartButtonColor);
    DrawRectangleRoundedLines(startButtonBoundsOutline, 0.0f, 4, 3.0f, BLACK);

    DrawText(startButtonLabel, startButtonTextPos.x, startButtonTextPos.y, MENU_ELEMENT_PADDING, WHITE);

    // ************************************************************************************
    // ************************************************************************************

    return 0;
}

int _MainMenu_Cleanup(){
    // ...
    return 0;
}



