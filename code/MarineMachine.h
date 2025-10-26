#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "TextureHolder.h"
#include "LevelManager.h"
#include "Zombie.h"
#include <fstream>
#include <sstream>
#include "Player.h"
#include "Bullet.h"
#include "Warp.h"
#include "Pickup.h"

using namespace sf;

class MarineMachine
{
private:
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
	

public:
	// The MarineMachine constructor
	MarineMachine();

	// Change tilesheet
	void setTileSheets(int level);

	// Run will call all the private functions
	void run();

	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	State state = State::GAME_OVER;
};

#pragma once
