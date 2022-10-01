#include "raylib.h"
#include "screens.h"
#include "common.h" 	// Beej assert macro

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

static int logoPositionX = 0;
static int logoPositionY = 0;

static int lettersCount = 0;

static int topSideRecWidth = 0;
static int leftSideRecHeight = 0;

static int bottomSideRecWidth = 0;
static int rightSideRecHeight = 0;

// static int state = 0;              // Logo animation states
enum logo_state { RAYLIB_I, RAYLIB_II, RAYLIB_III, RAYLIB_IIII, ROJAFOX_LOGO };
static enum logo_state state = RAYLIB_I;

static Texture2D rojafox_logo = { 0 };

static float alpha = 1.0f;         // Useful for fading

//----------------------------------------------------------------------------------
// Logo Screen Functions Definition
//----------------------------------------------------------------------------------

// Logo Screen Initialization logic
void InitLogoScreen(void)
{
    finishScreen = 0;
    framesCounter = 0;
    lettersCount = 0;

    logoPositionX = GetScreenWidth()/2 - 128;
    logoPositionY = GetScreenHeight()/2 - 128;

    topSideRecWidth = 16;
    leftSideRecHeight = 16;
    bottomSideRecWidth = 16;
    rightSideRecHeight = 16;

	// state = RAYLIB_I; // TODO: switch back
	state = ROJAFOX_LOGO;

    alpha = 1.0f;

	rojafox_logo = LoadTexture("resources/fox_.png");
}

// Logo Screen Update logic
void UpdateLogoScreen(void)
{
    if (state == RAYLIB_I)                 // State 0: Top-left square corner blink logic
    {
        framesCounter++;

        if (framesCounter == 80)
        {
            state = RAYLIB_II;
            framesCounter = 0;      // Reset counter... will be used later...
        }
    }
    else if (state == RAYLIB_II)            // State 1: Bars animation logic: top and left
    {
        topSideRecWidth += 8;
        leftSideRecHeight += 8;

        if (topSideRecWidth == 256) state = RAYLIB_III;
    }
    else if (state == RAYLIB_III)            // State 2: Bars animation logic: bottom and right
    {
        bottomSideRecWidth += 8;
        rightSideRecHeight += 8;

        if (bottomSideRecWidth == 256) state = RAYLIB_IIII;
    }
    else if (state == RAYLIB_IIII)            // State 3: "raylib" text-write animation logic
    {
        framesCounter++;

        if (lettersCount < 10)
        {
            if (framesCounter/12)   // Every 12 frames, one more letter!
            {
                lettersCount++;
                framesCounter = 0;
            }
        }
        else    // When all letters have appeared, just fade out everything
        {
            if (framesCounter > 200)
            {
                alpha -= 0.02f;

                if (alpha <= 0.0f)
                {
                    alpha = 0.0f;
                    // finishScreen = 1;   // Jump to next screen
					state = ROJAFOX_LOGO;
                }
            }
        }
    }
	else if (state == ROJAFOX_LOGO)            // State 4: my custom logo screen
    {
        framesCounter++;

		if (framesCounter > 150)
		{
			finishScreen = 1;   // Jump to next screen
		}
    }
}

// Logo Screen Draw logic
void DrawLogoScreen(void)
{
    if (state == RAYLIB_I)         // Draw blinking top-left square corner
    {
        if ((framesCounter/10)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
    }
    else if (state == RAYLIB_II)    // Draw bars animation: top and left
    {
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
        DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
    }
    else if (state == RAYLIB_III)    // Draw bars animation: bottom and right
    {
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
        DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);

        DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
        DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
    }
    else if (state == RAYLIB_IIII)    // Draw "raylib" text-write animation + "powered by"
    {
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(BLACK, alpha));
        DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(BLACK, alpha));

        DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(BLACK, alpha));
        DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(BLACK, alpha));

        DrawRectangle(GetScreenWidth()/2 - 112, GetScreenHeight()/2 - 112, 224, 224, Fade(RAYWHITE, alpha));

        DrawText(TextSubtext("raylib", 0, lettersCount), GetScreenWidth()/2 - 44, GetScreenHeight()/2 + 48, 50, Fade(BLACK, alpha));

        if (framesCounter > 20) DrawText("powered by", logoPositionX, logoPositionY - 27, 20, Fade(DARKGRAY, alpha));
    }
	else if (state == ROJAFOX_LOGO)
	{
		DrawText("RojaFox", GetScreenWidth()/2 - 120, GetScreenHeight()/2 - 160, 50, BLACK);

		// Fox logo ---------------
		static const int screenWidth = 800;
		static const int screenHeight = 450;
    	float imageScale = 1.1f;
		float logo_height = 164.0f;
		float logo_width = 123.0f;

		DrawTextureEx(rojafox_logo, (Vector2){ screenWidth / 2 - logo_width * imageScale / 2, screenHeight / 2 - logo_height * imageScale / 2 }, 0.0f, imageScale, WHITE);
	}
}

// Logo Screen Unload logic
void UnloadLogoScreen(void)
{
    // Unload LOGO screen variables here!
	UnloadTexture(rojafox_logo);
}

// Logo Screen should finish?
int FinishLogoScreen(void)
{
    return finishScreen;
}
