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
#include "Pickup.h"
#include "Enemy.h"
#include "MainMenu.h"
#include "HUD.h"
#include "ScoreObserver.h"
#include "ScoreSystem.h"
#include "Pathfinding.h"
#include "Dinosaur.h"
#include "Cowboy.h"
#include "Android.h"

using namespace sf;



class MarineMachine : public ScoreObserver
{
private:

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

	View m_MainView;

	// Declare a sprite and a Texture for the background
	Sprite m_BackgroundSprite;
	Texture m_BackgroundTexture;
	Sprite spriteMainMenu;
	Texture textureMainMenu;

	Sprite m_TitleGraphic;
	Texture m_TitleGraphicTexture;

	Font font;
	Text mainMenuText;
	Text pauseMenuText;

	Text scoreText;
	Text livesText;

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

	//bool detectCollisions(PlayableCharacter& character);

	MainMenu mainMenu;

	// Setup the HUD
	View m_HudView;
	HUD m_Hud;
	int m_PlayerScore = 0;

	// Setup the scoreing system
	ScoreSystem m_ScoreSystem;

public:
	// The MarineMachine constructor
	MarineMachine();

	// Change tilesheet
	void setTileSheets(int level);

	// Run will call all the private functions
	void run();

	Text storyText;
	Sprite storySprite;

	enum class State { MAIN_MENU, PAUSED, STORY_MENU, GAME_OVER, PLAYING };
	State state = State::MAIN_MENU;

	void onScoreChange(int newScore) override;
};
