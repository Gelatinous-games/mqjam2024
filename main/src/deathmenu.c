#include "deathmenu.h"


int _DeathMenu_Init(){

    ButtonColorBG = DARKGRAY;
    ButtonColorHover = DARKBLUE;

    // ************************************************************************************
    // ************************************************************************************
    // ******** RESTART BUTTON

    // ====================================================================================
    // === frame
    deathMenuFrameSize = (Vector2){
       2*WINDOW_WIDTH/3, 2*WINDOW_HEIGHT/3
    };
    deathMenuFramePos = (Vector2){
        (WINDOW_WIDTH-deathMenuFrameSize.x)/2, (WINDOW_HEIGHT-deathMenuFrameSize.y)/2
    };
    deathMenuFrameBoundsOutline = (Rectangle){
        deathMenuFramePos.x, deathMenuFramePos.y,
        deathMenuFrameSize.x, deathMenuFrameSize.y
    };
    deathMenuFrameBoundsBG = (Rectangle){
        deathMenuFramePos.x-(MENU_ELEMENT_PADDING/2), deathMenuFramePos.y-(MENU_ELEMENT_PADDING/2),
        deathMenuFrameSize.x+(MENU_ELEMENT_PADDING), deathMenuFrameSize.y+(MENU_ELEMENT_PADDING)
    };

    // ************************************************************************************
    // ************************************************************************************
    // ******** RESTART BUTTON

    // ====================================================================================
    // === button label
    restartButtonTextSize = (Vector2){
       MeasureText(restartButtonLabel, BUTTON_FONT_SIZE), BUTTON_FONT_SIZE
    };
    restartButtonTextPos = (Vector2){
        (WINDOW_WIDTH-(restartButtonTextSize.x/2))/2, (WINDOW_HEIGHT-(restartButtonTextSize.y/2))/2
    };
    restartButtonTextBounds = (Rectangle){
        restartButtonTextPos.x, restartButtonTextPos.y,
        restartButtonTextSize.x, restartButtonTextSize.y
    };

    // ====================================================================================
    // === button
    restartButtonSize = (Vector2){
       restartButtonTextSize.x, restartButtonTextSize.y
    };
    restartButtonPos = (Vector2){
        (WINDOW_WIDTH-restartButtonSize.x)/2, (WINDOW_HEIGHT-restartButtonSize.y)/2
    };
    restartButtonBoundsOutline = (Rectangle){
        restartButtonPos.x-(MENU_ELEMENT_PADDING/2), restartButtonPos.y-(MENU_ELEMENT_PADDING/2),
        restartButtonSize.x+(MENU_ELEMENT_PADDING), restartButtonSize.y+(MENU_ELEMENT_PADDING)
    };
    restartButtonBoundsBG = (Rectangle){
        restartButtonPos.x-MENU_ELEMENT_PADDING, restartButtonPos.y-MENU_ELEMENT_PADDING,
        restartButtonSize.x+(2*MENU_ELEMENT_PADDING), restartButtonSize.y+(2*MENU_ELEMENT_PADDING)
    };

    // ************************************************************************************
    // ************************************************************************************

    return 0;
}

int _DeathMenu_Update(float DeltaTime){

    // ************************************************************************************
    // ************************************************************************************
    // ******** RESTART BUTTON

    if(
        ( IsKeyDown(KEY_SPACE) ) ||
        ( MouseOverRectangle(restartButtonBoundsBG) && IsMouseButtonDown(MOUSE_BUTTON_LEFT) )
    ){
        _DeathMenu_ScheduleRestartGame();
    }

    // ************************************************************************************
    // ************************************************************************************
    // ...
    return 0;
}

int _DeathMenu_Draw(){
    // ...
    
    // ************************************************************************************
    // ************************************************************************************
    // ******** DEATH MENU FRAME

    DrawRectangleRec(deathMenuFrameBoundsBG, DEATH_MENU_BG_COLOUR);
    DrawRectangleRoundedLines(deathMenuFrameBoundsOutline, 0.0f, 4, 3.0f, BLACK);

    // ************************************************************************************
    // ************************************************************************************
    // ******** RESTART BUTTON

    Color restartButtonColor = (MouseOverRectangle(restartButtonBoundsBG))? ButtonColorHover : ButtonColorBG;

    DrawRectangleRec(restartButtonBoundsBG, restartButtonColor);
    DrawRectangleRoundedLines(restartButtonBoundsOutline, 0.0f, 4, 3.0f, BLACK);

    DrawText(restartButtonLabel, restartButtonTextPos.x, restartButtonTextPos.y, MENU_ELEMENT_PADDING, WHITE);

    // ************************************************************************************
    // ************************************************************************************

    return 0;
}

int _DeathMenu_Cleanup(){
    // ...
    return 0;
}




void _DeathMenu_ScheduleRestartGame(){
    //...
    // ...
    NEXT_FRAME_GAME_STARTS = 1;
    // __GAMEMANAGER_REF->awaitDestroy = 1;
}


