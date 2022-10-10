#include "raylib.h"
#include "screens.h"

extern int endGameConditions;
//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Ending Screen Functions Definition
//----------------------------------------------------------------------------------

// Ending Screen Initialization logic
void InitEndingScreen(void)
{
    // TODO: Initialize ENDING screen variables here!
    framesCounter = 0;
    finishScreen = 0;

	// PlayMusicStream(music);

}

// Ending Screen Update logic
void UpdateEndingScreen(void)
{
    // Press enter or tap to return to TITLE screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
    }
}

// Ending Screen Draw logic
void DrawEndingScreen(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);

	if (endGameConditions == 1)
		DrawTextEx(font, "YOU WIN!", (Vector2){ 20, 10 }, font.baseSize*0.5, 4, DARKBLUE);
	else if (endGameConditions == 2)
		DrawTextEx(font, "YOU LOSE!", (Vector2){ 20, 10 }, font.baseSize*0.5, 4, MAROON);

    DrawText("Press ENTER or TAP to return to the main menu", 120, 220, 20, DARKBLUE);
}

// Ending Screen Unload logic
void UnloadEndingScreen(void)
{
    // TODO: Unload ENDING screen variables here!
}

// Ending Screen should finish?
int FinishEndingScreen(void)
{
    return finishScreen;
}
