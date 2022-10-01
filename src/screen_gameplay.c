#include "raylib.h"
#include "screens.h"

//* constants/defines for main gameplay:
// #define TIME_LIMIT_SECONDS      180

//----------------------------------------------------------------------------------
// Structs
//----------------------------------------------------------------------------------

// * structs

// * enums

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//* local variables:


//----------------------------------------------------------------------------------
// Module Functions Definition (local)
//----------------------------------------------------------------------------------

//* local methods (static methods):

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;

	//* load textures, initialize player, level, variables, load musicstream and play music
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!

	/*
		for testing screens:
		// Press enter or tap to change to ENDING screen
		if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
		{
			finishScreen = 1;
			PlaySound(fxCoin);
		}
	*/
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);

	// TODO: Delete this debug text later
    DrawTextEx(font, "GAMEPLAY SCREEN", (Vector2){ 20, 10 }, font.baseSize*0.5, 4, MAROON);
    // DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: unload texture, sound, music stream
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}

//----------------------------------------------------------------------------------
// Module Functions Declaration (local)
//----------------------------------------------------------------------------------

//* local methods