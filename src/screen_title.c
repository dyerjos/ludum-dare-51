#include "raylib.h"

// #define RAYGUI_IMPLEMENTATION // required for raylibGui
// #include "raygui.h"     		// required for raylibGui

#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

// Title Screen Initialization logic
void InitTitleScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;
}

// Title Screen Update logic
void UpdateTitleScreen(void)
{
    // Press enter or tap to change to GAMEPLAY screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        //finishScreen = 1;   // OPTIONS
        finishScreen = 2;   // GAMEPLAY
        // PlaySound(fxCoin);
    }
}

// Title Screen Draw logic
void DrawTitleScreen(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
    DrawTextEx(font, "Artful Arrows", (Vector2){ 20, 10 }, font.baseSize*1.5, 4, BLACK);
    DrawText("PRESS ENTER or TAP to JUMP to start the game", 120, 220, 20, BLACK);
}

// Title Screen Unload logic
void UnloadTitleScreen(void)
{
	//
}

// Title Screen should finish?
int FinishTitleScreen(void)
{
    return finishScreen;
}