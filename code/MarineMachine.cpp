#include "MarineMachine.h"
#include "Enemy.h"
#include "SoundManager.h"
#include "Pathfinding.h"

using namespace sf;

Enemy** DinoSpawner(int numDinos, IntRect arena);
Enemy** CowboySpawner(int numCowboys, IntRect arena);
Enemy** AndroidSpawner(int numCowboys, IntRect arena);

SoundManager soundManager;

MarineMachine::MarineMachine()
	: mainMenu(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height),
	pauseMenu(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height)
{
	
	m_Pathfinding = new Pathfinding();

	// Get the screen resolution and create an SFML window and View
	Vector2f resolution;
	
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	m_Window.create(VideoMode(resolution.x, resolution.y), "Marine Madness", Style::Fullscreen);

	// Initialise the full screen view
	m_MainView.setSize(resolution);

	// Create story view
	m_StoryView.setSize(resolution);

	// Setup HUD view
	m_HudView.setSize(resolution);
	m_HudView.setCenter(resolution.x / 2, resolution.y / 2);

	m_PauseView.setSize(resolution);
	m_PauseView.setCenter(resolution.x / 2, resolution.y / 2);

	// Zoom in on player
	m_MainView.zoom(0.8);

	// Can this graphics card use shaders?
	if (!Shader::isAvailable())
	{
		// Time to get a new PC
		m_Window.close();
	}

	m_BackgroundTexture = TextureHolder::GetTexture(
		"graphics/background22.jpg");

	// Load the texture for the background vertex array
	m_TextureTiles = TextureHolder::GetTexture(
		"graphics/tile-sheet0.png");

	m_StorySprite.setTexture(m_BackgroundTexture);
	m_StorySprite.setPosition(-470,-50);

	font.loadFromFile("fonts/ByteBounce.ttf");

	m_TitleText.setFont(font);
	m_TitleText.setCharacterSize(85);
	m_TitleText.setFillColor(Color::Magenta);
	m_TitleText.setPosition(410, 50);
	m_TitleText.setString("<+= M-A-R-I-N-E M-A-D-N-E-S-S =+>");

	setStoryTextPosition();
	m_StoryText.setFont(font);
	m_StoryText.setCharacterSize(55);
	m_StoryText.setFillColor(Color::Yellow);
	m_StoryText.setPosition(getStoryTextPosition());

	// Hide mouse cursor and setup crosshair
	m_Window.setMouseCursorVisible(false);
	textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	// Initialize time
	gameTimeTotal = Time::Zero;
}

// Function for each level tiles
void MarineMachine::setTileSheets(int level)
{
	// Change sheet for each level
	switch (level)
	{
		case 1:
			// Archaic Anarchy
			m_TextureTiles = TextureHolder::GetTexture("graphics/tile-sheet1.png");
			break;

		case 2:
			// Wild West
			m_TextureTiles = TextureHolder::GetTexture("graphics/tile-sheet2.png");
			break;

		case 3:
			// Fracture Future
			m_TextureTiles = TextureHolder::GetTexture("graphics/tile-sheet3.png");
			break;

		default:
			// Tutorial/Warehouse
			m_TextureTiles = TextureHolder::GetTexture("graphics/tile-sheet0.png");
			break;
	}
}

void MarineMachine::setLevelText(int level)
{
	// Story stream
	std::stringstream storyStream;

	// Change text for each part of game
	switch (level)
	{
	case 0:
		// Intro text
		storyStream <<
			"You, the player were informed that you must retrieve a stolen machine." <<
			"\nThis machine allows any user to travel through time." <<
			"\nSo be cautious when near the device" <<
			"\n" <<
			"\nWhen ready, press E to enter the warehouse.";
		m_StoryText.setString(storyStream.str());
		break;

	case 1:
		// Archaic Anarchy text
		storyStream <<
			"Era 1 -> Arhchaic Anarchy -> 32XX BC" <<
			"\n" <<
			"\nWATCH OUT! There are prehistoric reptiles that roam these lands." <<
			"\n" <<
			"\nYou can either kill all the dinosaurs OR flee to the distant warp." <<
			"\nThe choice is yours..." <<
			"\nWhen ready, press E to start level 1.";
		m_StoryText.setString(storyStream.str());
		break;

	case 2:
		// Wild West text
		storyStream <<
			"Era 2 -> Wild West -> 18XX" <<
			"\n" <<
			"\nYEEEHAWWW! There's some dastardly bandits hanging around." <<
			"\nBe cautious for each of these foes have guns." <<
			"\nYou can either kill all the cowboys OR flee to the distant warp." <<
			"\nThe choice is yours..." <<
			"\nWhen ready, press E to start level 2.";
		m_StoryText.setString(storyStream.str());
		break;

	case 3:
		// Fracture Future text
		storyStream <<
			"Era 3 -> Fracture Future -> 30XX" <<
			"\n" <<
			"\nCongradulations! You have made it to the final level." <<
			"\n" <<
			"\n" <<
			"\nYour final threat are an array of androids." <<
			"\n" <<
			"\nYou can either kill all these machines OR return home using the final warp." <<
			"\nThe choice is yours..." <<
			"\n" <<
			"\nWhen ready, press E to start level 3.";
		m_StoryText.setString(storyStream.str());
		break;

	case 4:
		// Endscreen text
		storyStream <<
			"Era 4 -> The Warehouse -> 20XX" <<
			"\n" <<
			"After returning to the warehouse, you pick up the device." <<
			"\nKnowing its potential, you carefully locked the device away." <<
			"\nSo that it could never send another victim in a time loop." <<
			"\n" <<
			"\n" <<
			"\nTHE-END!." <<
			"\n" <<
			"\nPress Q to finish game and return to main menu";
		m_StoryText.setString(storyStream.str());
		break;
	}
}

void MarineMachine::setStoryTextPosition()
{
	// Set position
	m_TextPosition.x = -70;
	m_TextPosition.y = 950;

	// Update text position
	m_StoryText.setPosition(m_TextPosition);
}

void MarineMachine::scrollStoryText(float dt)
{
	// Move text upwards
	m_TextPosition.y -= m_ScrollSpeed * dt;

	// Update text position while scrolling
	m_StoryText.setPosition(m_TextPosition);
}

Vector2f MarineMachine::getStoryTextPosition()
{
	// Retrieve story text position
	return m_TextPosition;
}

void MarineMachine::run()
{
	// Timing 	
	Clock clock;

	while (m_Window.isOpen())
	{
		Time dt = clock.restart();
		// Update the total game time
		 gameTimeTotal += dt;
		// Make a decimal fraction from the delta time
		float dtAsSeconds = dt.asSeconds();
		
		input();

		update(dtAsSeconds);

		m_Hud.update(bulletsInClip, bulletsSpare, m_PlayerScore, lm.getCurrentLevel(), marine.getHealth());

		draw();
	}
}

void MarineMachine::onScoreChange(int newScore)
{
	m_PlayerScore = newScore;
	m_Hud.update(bulletsInClip, bulletsSpare, m_PlayerScore, lm.getCurrentLevel(), marine.getHealth());
}

void MarineMachine::resetGame()
{
	// Reset the current level to 1
	lm.setCurrentLevel(lm.getCurrentLevel());

	// Set level text
	setLevelText(lm.getCurrentLevel());

	// Mark that we need a new level
	m_NewLevelRequired = true;

	// Reset the score
	m_PlayerScore = 0;

	// Reset player health
	marine.resetHealth();

	// Clear enemies
	enemies.clear();

	// Reset ammo
	bulletsSpare = 24;
	bulletsInClip = 6;
	currentBullet = 0;

	// Load the first level
	loadLevel();
}
