#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "TextureHolder.h"
#include "LevelManager.h"
#include <fstream>
#include <sstream>
#include "Player.h"
#include "Bullet.h"
#include "Warp.h"
#include "Enemy.h"
#include "Menu.h"
#include "MainMenu.h"
#include "PauseMenu.h"
#include "HUD.h"
#include "ScoreObserver.h"
#include "ScoreSystem.h"
#include "Pathfinding.h"
#include "Dinosaur.h"
#include "Cowboy.h"
#include "Android.h"
#include "OptionMenu.h"

using namespace sf;

class MarineMachine : public ScoreObserver
{
private:

	// Position and speed of scrolling story text
	Vector2f m_TextPosition;
	float m_ScrollSpeed = 50.f;

	vector<unique_ptr<Enemy>> enemies;
	int numEnemiesAlive = 0;

	// Variable for arena bounds
	IntRect arena;

	// Bullets
	Bullet bullets[100];
	Bullet enemyBullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;

	// Dodge variables
	bool isDodging = false;
	Time dodgeDuration = milliseconds(300);
	Time dodgeCooldown = seconds(2);
	Time lastDodgeTime;
	float originalSpeed;

	// Melee variables
	const float MELEE_LENGTH = 50.f; // Length of melee attack
	const float MELEE_WIDTH = 5.f;   // Width of the melee attack
	RectangleShape meleeAttackRect;   // Visual representation
	bool isMeleeAttacking = false;
	Time meleeCooldown = milliseconds(500);
	Time lastMeleeAttack;

	// Pathfinding
	Pathfinding* m_Pathfinding;

	// Mouse positions
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;

	// Time tracking
	Time gameTimeTotal;

	// Crosshair
	Sprite spriteCrosshair;
	Texture textureCrosshair;

	// Weapon tracking
	bool holdingPistol = true;
	bool holdingShot = false;
	bool holdingSmg = false;

	// Fire rates
	Time pistolFireRate = milliseconds(500);
	Time lastPistolShot;

	// The texture holder
	TextureHolder th;

	// The Marine
	Player marine;

	// Time warp
	Warp wp;

	// A class to manage all the levels
	LevelManager lm;

	const int TILE_SIZE = 50;
	const int VERTS_IN_QUAD = 4;

	// A regular RenderWindow
	RenderWindow m_Window;

	// The game view
	View m_MainView;

	// Declare a sprite and a Texture for the background
	Sprite m_BackgroundSprite;
	Texture m_BackgroundTexture;
	Sprite spriteMainMenu;
	Texture textureMainMenu;

	// A suitable font
	Font font;
	
	// Controls text for turorial level
	Text m_ControlsTextA;
	Text m_ControlsTextB;
	Text m_ControlsTextC;

	// Is the game currently playing?
	bool m_Playing = false;

	// Is it time for a new/first level?
	bool m_NewLevelRequired = true;

	// The vertex array for the level design
	VertexArray vaLevel;

	// The 2d array with the map for the level
	// A pointer to a pointer
	int** m_ArrayLevel = NULL;
	int** m_ArraySpawningPointsLevel = NULL;

	// Texture for the background and the level tiles
	Texture m_TextureTiles;

	// Private functions for internal use only
	void input();
	void update(float dtAsSeconds);
	void draw();

	// Load a new level
	void loadLevel();

	// Main menu
	MainMenu mainMenu;

	// Pause menu
	View m_PauseView;
	PauseMenu pauseMenu;

	// Game title graphic
	Text m_TitleText;

	// Create story menu
	View m_StoryView;
	Text m_StoryText;
	Sprite m_StorySprite;

	// Setup the HUD
	View m_HudView;
	HUD m_Hud;
	int m_PlayerScore = 0;

	// Setup the scoring system
	ScoreSystem m_ScoreSystem;
	View m_ScoreView;

	// Option Menu
	OptionMenu optionMenu;
	View m_OptionView;

	// Determines win condition of game
	bool m_HasReturned = false;

	// Collision
	void detectCollision();

public:
	// The MarineMachine constructor
	MarineMachine();

	// Change tilesheet
	void setTileSheets(int level);

	// Alter text for relevant level
	void setLevelText(int level);

	// Run will call all the private functions
	void run();

	// Set lore text for each level
	void setStoryTextPosition();

	// Scrolling text
	void scrollStoryText(float dt);

	// Find position for story text
	Vector2f getStoryTextPosition();

	// A boolean
	bool testBool;

	// Game states
	enum class State { MAIN_MENU, PAUSED, STORY_MENU, PLAYING, SCOREBOARD, OPTION};
	State state = State::MAIN_MENU;
	State m_StateBeforeOption = State::MAIN_MENU; // To return to previous state from options

	// Overriden score changer
	void onScoreChange(int newScore) override;

	// Reset game
	void resetGame();
};
