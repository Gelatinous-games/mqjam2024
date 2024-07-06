#pragma once

#include "raylib.h"


int MouseOverRectangle(Rectangle buttonBounds);











#ifndef _menu_util
#define _menu_util











int MouseOverRectangle(Rectangle buttonBounds){
    Vector2 mousePosition = GetMousePosition();
    if(
        ( buttonBounds.x <= mousePosition.x && ( buttonBounds.x+buttonBounds.width  ) >= mousePosition.x ) &&
        ( buttonBounds.y <= mousePosition.y && ( buttonBounds.y+buttonBounds.height ) >= mousePosition.y )
    ){
        return 1;
    }

    return 0;
}

#endif