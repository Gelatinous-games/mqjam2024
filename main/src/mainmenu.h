#pragma once


#include "raylib.h"


// project header includes
#include "settings.h"
#include "sound.h"
#include "menu.h"

#include "base.h"
#include "obj_register.h"


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
int _MainMenu_Update(void *titleManagerRef, float DeltaTime);
int _MainMenu_Draw();
int _MainMenu_Cleanup();
