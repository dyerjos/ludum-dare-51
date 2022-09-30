/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"

//* example of including external dependencies:
	// #include "external/raymath.h"
	// #include <stdio.h>

//* examples of constants for main gameplay:
	// #define TILE_REQUIRED_CLEAN_TIME        2           // Frames it takes to clean a dirt level
	// #define TILE_SCORE_BY_CLEANED_LEVEL     100         // Score by cleanied dirt level
	// #define TILE_REQUIRED_CLEAN_AREA        28*28       // Required are for actually cleaning tile

	// #define TILE_SIZE               36      // Tile size, it should match texture
	// #define MAX_TILES_X             32
	// #define MAX_TILES_Y             17

	// #define CAT_TARGET_RADIUS       3       // Target proximity radius
	// #define CAT_DIRT_CELL_RADIUS    2       // Cells around cat for dirt spreading

	// #define TIME_LIMIT_SECONDS      180     // Time to complete the level in seconds

	// #define MAX_SCORE_POPUPS        60      // Maximum simultaneous score pop-ups!

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

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    DrawTextEx(font, "GAMEPLAY SCREEN", (Vector2){ 20, 10 }, font.baseSize*3, 4, MAROON);
    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
	// * example: unload texture, sound, music stream
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