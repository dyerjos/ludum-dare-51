#include "raylib.h"
#include "raymath.h"
#include "screens.h"
#include "common.h"

extern int endGameConditions;

//--------------------------------------
// Constants
//--------------------------------------
#define MAX_ARROWS		50
#define MAX_ENEMIES		5			// TODO: 50 if spawning works
#define MAX_PLAYERS     2
#define GRAVITY       	9.81f
#define GROUND_LEVEL	400
#define ARROW_SIZE		(Vector2) {5, -20}
#define DELTA_FPS       60
#define ENTITY_SIZE		(Vector2) { 5, 25 }
#define ARROW_LEN		15
#define TEN_SEC			600 		// 60fps for 10 seconds
#define P1_ORIGIN_X		40			// TODO: change back to 40 (650 for testing)

//--------------------------------------
// Structs
//--------------------------------------

typedef struct {
	// visuals:
	Vector2 head;
	Vector2 tail;
	Color color;
	// Texture arrow_sprite;

	// misc:
	bool isActive;
	bool isInFlight;
	bool isCollided;
	Vector2 speed;
	int range;
	int potential_end_x;

	// arrowhead; TODO: a struct for DrawTriangle() 3 vectors?
	// arrow can point to enemy position once collided to follow enemey pos?
} projectile;

typedef struct bow {
	// cubic bezier curve properties:
	// Vector2 top_limb_tip;
	// Vector2 bottom_limb_tip;
	// Vector2 mid_string;
	// Vector2 string_start_control_pos;
	// Vector2 string_end_control_pos;

	// aim:
    int aimingAngle;
    int aimingPower;
    // int previousAngle;
    // int previousPower;

	// visuals:
	Color color;
	float thickness;

	// misc:
	// bool isPlayer;
} bow;

typedef struct {
	Vector2 position;
	Vector2 size;
	bool isActive;
	bool isAlive;
	int deathTime;
	bool isCollided;
	int speed;
	int health;
	int hasFired;
	enum weapon { BOW, SWORD } weapon_type;
	enum owner { PLAYER_1, PLAYER_2, AI } owner_type;
	struct bow bow;
	// struct sword;
	Color color;
	// Texture sprite;
} entity;

// void DrawLineBezierCubic(Vector2 startPos=top_limb_tip, Vector2 endPos=bottom_limb_tip, Vector2 startControlPos, Vector2 endControlPos, float thick, Color color);


//--------------------------------------
// Module Variables Definition (local)
//--------------------------------------

static int framesCounter = 0;
static int finishScreen = 0;

static bool pause = false;

entity player[MAX_PLAYERS] = { 0 };
projectile arrow[MAX_ARROWS] = { 0 };
entity enemy[MAX_ENEMIES] = { 0 };

static int playerTurn = 0;
static int enemyTurn = 0;
static int playersAlive = MAX_PLAYERS;
static int enemiesLeft = MAX_ENEMIES;


static Vector2 mousePosOnClick;
static Vector2 mousePosOnRelease;
static Vector2 currentMousePosition;
static bool isAiming = 0;
static int numberOfActiveArrows = 0;
static int numberOfActiveEnemies = 0;


//--------------------------------------
// Module Functions Declaration (local)
//--------------------------------------

static void InitPlayers(void);
static void InitEnemies(void);
static void ActivateArrow(void);
static void PlayerAim(int playerTurn);
static void UpdateArrow(void);
static void DrawPlayers(void);
static void DrawEnemies(void);
static void DrawArrows(void);
static void DrawAim(void);
static void DrawHUD(void);
static void DebugInfo(void);
static void BowAimed(void);
static bool arrowIsMoving(void);

//--------------------------------------
// Gameplay Screen Functions Definition
//--------------------------------------

void InitGameplayScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;

	pause = false;
	entity player[MAX_PLAYERS] = { 0 };
	projectile arrow[MAX_ARROWS] = { 0 };
	entity enemy[MAX_ENEMIES] = { 0 };
	playerTurn = 0;
	enemyTurn = 0;
	playersAlive = MAX_PLAYERS;
	enemiesLeft = MAX_ENEMIES;
	isAiming = 0;
	numberOfActiveArrows = 0;
	numberOfActiveEnemies = 0;

	InitPlayers();
	InitEnemies();
}

void UpdateGameplayScreen(void)
{
	if (IsKeyPressed(KEY_SPACE)) pause = !pause;

	if (!pause) {
		if (playersAlive == 0) {
			endGameConditions = 2;
			// finishScreen = 1; // don't use end screen
		}
		if (enemiesLeft == 0) {
			endGameConditions = 1;
			// finishScreen = 1; // don't use end screen
		}
		if (!player[playerTurn].hasFired)
			PlayerAim(playerTurn);
		else if (arrowIsMoving()){
			UpdateArrow();
		} else {
			// reset some things for next turn:
			player[playerTurn].hasFired = 0;
			int nextPlayer = !playerTurn;
			if (player[nextPlayer].isAlive)
				playerTurn = nextPlayer;
		}
	}

}

void DrawGameplayScreen(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), 450, GRAY); // sky
	DrawRectangle(0, GROUND_LEVEL, GetScreenWidth(), 450 - GROUND_LEVEL, DARKGREEN); // ground

	if (pause) DrawTextEx(font, "PAUSED", (Vector2){ GetScreenWidth()/2, GetScreenHeight()/2 }, font.baseSize*0.5, 4, MAROON);

	DebugInfo();

	DrawPlayers();
	DrawEnemies();
	DrawArrows();
	DrawAim();
	DrawHUD();
}

void UnloadGameplayScreen(void)
{
	MemFree(player);
	MemFree(enemy);
	MemFree(arrow);
}

int FinishGameplayScreen(void)
{
    return finishScreen;
}

//--------------------------------------
// Module Method Definition (local)
//--------------------------------------

//* local methods (static methods):

static void InitPlayers(void)
{
	for (int i = 0; i < MAX_PLAYERS; i++) {
		Vector2 position;
		Color color;
		int owner_type;

		if (i == PLAYER_1) {
			owner_type = PLAYER_1;
			position = (Vector2) { P1_ORIGIN_X, ( GROUND_LEVEL - (ENTITY_SIZE.y / 2) ) };
			color = RED;
		}
		else {
			owner_type = PLAYER_2;
			position = (Vector2) { 140, ( GROUND_LEVEL - (ENTITY_SIZE.y / 2) ) };
			color = BLUE;
		}

		player[i].position = position;
		player[i].size = ENTITY_SIZE;
		player[i].isActive = 1;
		player[i].isCollided = 0;
		player[i].speed = 0;
		player[i].health = 10;
		player[i].owner_type = owner_type;
		player[i].weapon_type = BOW;
		player[i].isAlive = 1;
		player[i].deathTime = 0;
		player[i].color = color;
		player[i].hasFired = 0;
		// .sprite = player_1_sprite;

		// cubic bezier curve properties:
		// player[i].bow.top_limb_tip = (Vector2) { 0, 0 };
		// player[i].bow.bottom_limb_tip = (Vector2) { 0, 0 };
		// player[i].bow.mid_string = (Vector2) { 0, 0 };
		// player[i].bow.string_start_control_pos = (Vector2) { 0, 0 };
		// player[i].bow.string_end_control_pos = (Vector2) { 0, 0 };
		// aim:
		player[i].bow.aimingAngle = 0;
		player[i].bow.aimingPower = 0;

		// visuals:
		player[i].bow.color = BLACK;
		player[i].bow.thickness = 1.0f;
		// misc:
		// player[i].bow.isPlayer = 1;
	}
}

static void InitEnemies(void)
{
	for (int i = 0; i < 5; i++) {
		enemy[i].position = (Vector2) {700 + (numberOfActiveEnemies * (16 + GetRandomValue(-2, 2))), (GROUND_LEVEL - (ENTITY_SIZE.y / 2))};
		enemy[i].size = ENTITY_SIZE;
		enemy[i].isActive = 1;
		enemy[i].isCollided = 0;
		enemy[i].speed = 5;
		enemy[i].health = 1;
		enemy[i].owner_type = AI;
		enemy[i].weapon_type = BOW;
		enemy[i].isAlive = 1;
		enemy[i].color = RAYWHITE;
		numberOfActiveEnemies += 1;
	}
}

static void ActivateArrow(void)
{
	int arrowId = numberOfActiveArrows; // arrow 1 is index 0
	player[playerTurn].hasFired = 1;
	arrow[arrowId].isInFlight = 1;
	numberOfActiveArrows += 1;
	arrow[arrowId].tail.x = player[playerTurn].position.x;
	arrow[arrowId].tail.y = player[playerTurn].position.y;
	Vector2 arrow_pos = Vector2Add(player[playerTurn].position,  ARROW_SIZE);
	arrow[arrowId].head.x = arrow_pos.x;
	arrow[arrowId].head.y = arrow_pos.y;
	arrow[arrowId].speed.x = cos(player[playerTurn].bow.aimingAngle * DEG2RAD) * player[playerTurn].bow.aimingPower * 3 / DELTA_FPS;
	arrow[arrowId].speed.y = -sin(player[playerTurn].bow.aimingAngle * DEG2RAD) * player[playerTurn].bow.aimingPower * 3 / DELTA_FPS;
	arrow[arrowId].isActive = 1;
	arrow[arrowId].range = fabs((pow(arrow[arrowId].head.x, 2) * sin(2 * player[playerTurn].bow.aimingAngle)) / GRAVITY);
	arrow[arrowId].potential_end_x = fabsf(arrow[arrowId].range + arrow[arrowId].head.x);
}

static void BowAimed()
{
	player[playerTurn].bow.aimingPower = sqrt(pow(mousePosOnRelease.x - mousePosOnClick.x, 2) + pow(mousePosOnRelease.y - mousePosOnClick.y, 2));

	if (player[playerTurn].bow.aimingPower < 50)
		player[playerTurn].bow.aimingPower = 50;
	else if (player[playerTurn].bow.aimingPower > 800)
		player[playerTurn].bow.aimingPower = 800;

	ASSERT(player[playerTurn].bow.aimingPower > 0, "aimingPower should be greater than zero");

	player[playerTurn].bow.aimingAngle = asin((mousePosOnRelease.y - mousePosOnClick.y)/player[playerTurn].bow.aimingPower) * RAD2DEG;

	ASSERT(player[playerTurn].bow.aimingAngle > -90, "aimingAngle should be greater than -90");
	ASSERT(player[playerTurn].bow.aimingAngle < 90, "aimingAngle should be less than 90");
}


static void PlayerAim(int playerTurn)
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isAiming == 0 && !Vector2Equals(GetMousePosition(), Vector2Zero())) {
		isAiming = 1;
		mousePosOnClick = GetMousePosition();
	}
	else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		isAiming = 0;
		mousePosOnRelease = GetMousePosition();
		bool clickWithNoDrag = mousePosOnClick.x != mousePosOnRelease.x &&  mousePosOnClick.y != mousePosOnRelease.y;
		if (clickWithNoDrag) {
			BowAimed();
			ActivateArrow();
		}
	} else {
		currentMousePosition = GetMousePosition();
	}
}

static void UpdateArrow(void)
{
	for (int i = 0; i < MAX_ARROWS; i++) {
		if (arrow[i].isActive && !arrow[i].isCollided){
			arrow[i].head.x += arrow[i].speed.x;
			arrow[i].head.y += arrow[i].speed.y;
			arrow[i].speed.y += GRAVITY/DELTA_FPS;
		}
		if (arrow[i].head.y >= GROUND_LEVEL && !arrow[i].isCollided) {
			TraceLog(LOG_INFO, "collision with ground");
			arrow[i].isCollided = 1;
			arrow[i].isInFlight = 0;
		} else {
			Vector2 end_pos = (Vector2) {arrow[i].potential_end_x, GROUND_LEVEL};
			arrow[i].tail = Vector2MoveTowards(arrow[i].head, end_pos, ARROW_LEN);
			// TODO: try wife's idea of having head and tail on same initial vector but with time diff
		}
		// Collisions with players
		for (int j = 0; j < MAX_PLAYERS; j++)
		{
			if (j != playerTurn && !enemyTurn){
				if(player[j].isAlive){
					int threshold = 10;
					Vector2 player_top_pos = (Vector2) {player[j].position.x, GROUND_LEVEL - ENTITY_SIZE.y};
					Vector2 player_bottom_pos = (Vector2) {player[j].position.x, GROUND_LEVEL};
					if(player[j].isAlive && CheckCollisionPointLine(arrow[i].head, player_top_pos, player_bottom_pos, threshold)) {
						arrow[i].isCollided = 1;
						TraceLog(LOG_WARNING, "collision with player");
						arrow[i].head.y += 15;
						arrow[i].tail.y += 15;
						arrow[i].isInFlight = 0;
						player[j].isAlive = 0; // dead
						player[j].size = (Vector2) {25, 5};
						player[j].position.y += 10;
						player[j].deathTime = framesCounter;
						playersAlive -= 1;
					}
				}
			}
		}
		// Collisions with Enemies
		for (int j = 0; j < MAX_ENEMIES; j++)
		{
			int threshold = 10;
			Vector2 enemy_top_pos = (Vector2) {enemy[j].position.x, GROUND_LEVEL - ENTITY_SIZE.y};
			Vector2 enemy_bottom_pos = (Vector2) {enemy[j].position.x, GROUND_LEVEL};
			if(enemy[j].isAlive && CheckCollisionPointLine(arrow[i].head, enemy_top_pos, enemy_bottom_pos, threshold)) {
				arrow[i].isCollided = 1;
				TraceLog(LOG_WARNING, "collision with enemy");
				int random_offset = GetRandomValue(0, 10);
				arrow[i].head.y += 20;
				arrow[i].tail.y += 20;
				arrow[i].isInFlight = 0;
				enemy[j].isAlive = 0; // dead
				enemy[j].size = (Vector2) {25, 5};
				enemy[j].position.y += (10 + random_offset);
				enemy[j].deathTime = framesCounter;
				enemiesLeft -= 1;
			}
		}
	}
}

static bool arrowIsMoving(void)
{
	for (int i = 0; i < MAX_ARROWS; i++) {
		if (arrow[i].isInFlight)
			return 1;
	}
	return 0;
}

static void DrawPlayers(void)
{
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (player[i].isAlive) {
			DrawRectangle(player[i].position.x - player[i].size.x/2, player[i].position.y - player[i].size.y/2, player[i].size.x, player[i].size.y, player[i].color);
		}
		else {
			DrawRectangle(player[i].position.x - player[i].size.x/2, player[i].position.y - player[i].size.y/2, player[i].size.x, player[i].size.y, BLACK);
		}
	}
}

static void DrawEnemies(void)
{
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy[i].isAlive) {
			DrawRectangle(enemy[i].position.x - enemy[i].size.x/2, enemy[i].position.y - enemy[i].size.y/2, enemy[i].size.x, enemy[i].size.y, enemy[i].color);
		}
		else {
			DrawRectangle(enemy[i].position.x - enemy[i].size.x/2, enemy[i].position.y - enemy[i].size.y/2, enemy[i].size.x, enemy[i].size.y, BLACK);
		}
	}
}

static void DrawArrows(void)
{
	for (int i = 0; i < MAX_ARROWS; i++) {
		if (arrow[i].isActive){
			DrawLineV(arrow[i].tail, arrow[i].head, GOLD);
		}
	}
}

static void DrawAim(void)
{
	if (isAiming) {
		DrawLineEx(mousePosOnClick, currentMousePosition, 1.0f, GREEN);
	} else {
		DrawLineEx(mousePosOnClick, mousePosOnRelease, 1.0f, LIME);
	}
}

static void DrawHUD(void)
{
	if (endGameConditions == 0){
		char *xx = malloc(64);
		sprintf(xx, "PLAYER %i:", playerTurn + 1);
		DrawTextEx(font, xx, (Vector2){ 20, 10 }, font.baseSize*0.5, 4, player[playerTurn].color);
	} else if (endGameConditions == 1)
		DrawTextEx(font, "YOU WIN!", (Vector2){ 20, 10 }, font.baseSize*0.5, 4, DARKBLUE);
	else if (endGameConditions == 2)
		DrawTextEx(font, "YOU LOSE!", (Vector2){ 20, 10 }, font.baseSize*0.5, 4, MAROON);


}

static void DebugInfo(void)
{
	int debug_window_offset = 460;

	char *s = malloc(32);
	sprintf(s, "mouse position : {%i, %i}", GetMouseX(), GetMouseY());
	DrawText(s, 20, debug_window_offset + 0, 10, BLACK);

	char *m = malloc(32);
	sprintf(m, "last left click pos : {%i, %i}", (int)mousePosOnClick.x, (int)mousePosOnClick.y);
	DrawText(m, 20, debug_window_offset + 10, 10, BLACK);

	char *rel = malloc(32);
	sprintf(rel, "last left release pos : {%i, %i}", (int)mousePosOnRelease.x, (int)mousePosOnRelease.y);
	DrawText(rel, 20, debug_window_offset + 20, 10, BLACK);

	// char *dis = malloc(32);
	// sprintf(dis, "vector distance : %f", Vector2Distance(mousePosOnClick, currentMousePosition));
	// DrawText(dis, 20, debug_window_offset + 30, 10, BLACK);

	// int debugAimingAngle = asin((currentMousePosition.y - mousePosOnRelease.y)/player[playerTurn].bow.aimingPower) * RAD2DEG;
	char *ang = malloc(32);
	sprintf(ang, "aimingAngle : %i", player[playerTurn].bow.aimingAngle);
	DrawText(ang, 20, debug_window_offset + 40, 10, BLACK);

	// int debugAimingPower = sqrt(pow(currentMousePosition.x - mousePosOnRelease.x, 2) + pow(currentMousePosition.y - mousePosOnRelease.y, 2));
	char *pow = malloc(32);
	sprintf(pow, "aimingPower : %i", player[playerTurn].bow.aimingPower);
	DrawText(pow, 20, debug_window_offset + 50, 10, BLACK);

	char *f = malloc(32);
	sprintf(f, "player[playerTurn].hasFired: %i", player[playerTurn].hasFired);
	DrawText(f, 20, debug_window_offset + 60, 10, BLACK);

	char *ar = malloc(32);
	sprintf(ar, "arrowIsMoving: %i", arrowIsMoving());
	DrawText(ar, 20, debug_window_offset + 70, 10, BLACK);

	char *aa = malloc(32);
	sprintf(aa, "playerTurn: %i", playerTurn);
	DrawText(aa, 20, debug_window_offset + 80, 10, BLACK);

	char *ab = malloc(32);
	sprintf(ab, "enemiesLeft: %i", enemiesLeft);
	DrawText(ab, 20, debug_window_offset + 90, 10, BLACK);

	char *ac = malloc(32);
	sprintf(ac, "playersAlive: %i", playersAlive);
	DrawText(ac, 20, debug_window_offset + 100, 10, BLACK);

	char *ad = malloc(32);
	sprintf(ad, "numberOfActiveArrows: %i", numberOfActiveArrows);
	DrawText(ad, 20, debug_window_offset + 110, 10, BLACK);

	char *ae = malloc(32);
	sprintf(ae, "numberOfActiveEnemies: %i", numberOfActiveEnemies);
	DrawText(ae, 20, debug_window_offset + 120, 10, BLACK);

	// char *af = malloc(64);
	// sprintf(af, "arrow[0].potential_end_x: %i", arrow[0].potential_end_x);
	// DrawText(af, 20, debug_window_offset + 130, 10, BLACK);

	// char *aa = malloc(64);
	// sprintf(aa, "VARIABLE: %i", variable);
	// DrawText(aa, 20, debug_window_offset + 70, 10, BLACK);

	// char *aa = malloc(64);
	// sprintf(aa, "VARIABLE: %i", variable);
	// DrawText(aa, 20, debug_window_offset + 70, 10, BLACK);

	// player collisions
	// for (int i = 0; i < MAX_PLAYERS; i++)
	// {
	// 	Vector2 player_top_pos = (Vector2) {player[i].position.x, GROUND_LEVEL - ENTITY_SIZE.y};
	// 	Vector2 player_bottom_pos = (Vector2) {player[i].position.x, GROUND_LEVEL};
	// 	DrawLineEx(player_top_pos, player_bottom_pos, 10.0f, LIME);
	// }
}
