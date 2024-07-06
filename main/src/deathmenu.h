#pragma once


#include "raylib.h"


// project header includes
#include "settings.h"
#include "sound.h"
#include "menu.h"

#include "base.h"
#include "obj_register.h"


Vector2 deathMenuFramePos;
Vector2 deathMenuFrameSize;
Rectangle deathMenuFrameBoundsBG;
Rectangle deathMenuFrameBoundsOutline;


Vector2 restartButtonPos;
Vector2 restartButtonSize;
Rectangle restartButtonBoundsBG;
Rectangle restartButtonBoundsOutline;

Vector2 restartButtonTextPos;
Vector2 restartButtonTextSize;
Rectangle restartButtonTextBounds;
const char *restartButtonLabel = "CONTINUE [ SPACEBAR ]";


Color ButtonColorBG;
Color ButtonColorHover;

int _DeathMenu_Init();
int _DeathMenu_Update(float DeltaTime);
int _DeathMenu_Draw();
int _DeathMenu_Cleanup();

void _DeathMenu_HandleStartGame();
