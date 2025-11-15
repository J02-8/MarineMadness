#include "MarineMachine.h"
#include "Enemy.h"
#include "SoundManager.h"
#include "Pathfinding.h"
#include "EnemySpawner.h"

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

	state = State::MAIN_MENU;

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

	m_StoryText.setFont(font);
	m_StoryText.setCharacterSize(55);
	m_StoryText.setFillColor(Color::White);
	m_StoryText.setPosition(-70, 350);

	// Hide mouse cursor and setup crosshair
	m_Window.setMouseCursorVisible(false);
	textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	// Initialize time
	gameTimeTotal = Time::Zero;
}

void MarineMachine::loadLevel()
{
	arena.width = 500;
	arena.height = 500;

	int level = lm.getCurrentLevel();
	setTileSheets(level);
	int count = 5; // TEMP

	switch (level)
	{
	case 1:
		EnemySpawner<Dinosaur>(enemies, count, arena);
		break;
	case 2:
		EnemySpawner<Cowboy>(enemies, count, arena);
		break;
	case 3:
		EnemySpawner<Android>(enemies, count, arena);
		break;
	default:
		enemies.clear();
		break;
	}

	numEnemiesAlive = enemies.size();
	

	m_Playing = false;

	// Delete the previously allocated memory
	for (int i = 0; i < lm.getLevelSize().y; ++i)
	{
		delete[] m_ArrayLevel[i];

	}
	delete[] m_ArrayLevel;

	// Load the next 2d array with the map for the level
	// And repopulate the vertex array as well
	m_ArrayLevel = lm.nextLevel(vaLevel);

	// Initialize pathfinding with level data (add after m_ArrayLevel is loaded)
	m_Pathfinding->setLevelData(m_ArrayLevel, lm.getLevelSize(), lm.getTileSize());

	// Get level's pixel bounds from LevelManager. 
	FloatRect arenaBounds = lm.getArenaBounds();

	// Get tile size
	float tileSize = lm.getTileSize(); 

	// Pass the arena data to player
	marine.setArena(arenaBounds, tileSize);

	// Pass arena date to warp
	wp.setArena(arenaBounds, tileSize);

	for (int i = 0; i < lm.getLevelSpawningPointsSize().y; ++i)
	{
		delete[] m_ArraySpawningPointsLevel[i];

	}
	delete[] m_ArraySpawningPointsLevel;

	// load spawning points data
	//analyse 2d array for location of dots
	//add dots to a list
	// each dot is an allocated object with a vector2f position
	// e.g. if a dot is in cell 6,8 then its x loc is 6*50+ 11 = 311
	// its y value is 8*50+11=411
	m_ArraySpawningPointsLevel = lm.nextLevelSpawningPoints();


	// Spawn Player
	marine.spawn(lm.getPlayerStartPosition());

	// Spawn Warp
	wp.spawn(lm.getWarpStartPosition());

	// Make sure this code isn't run again
	m_NewLevelRequired = false;

	// Add observer
	m_ScoreSystem.addObserver(this);
	
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
			"HOLY SHIIIT IT'S A DINOSAUR WTF??!" <<
			"\nThis machine allows any user to travel through time." <<
			"\nSo be cautious when near the device" <<
			"\n" <<
			"\nWhen ready, press E to start level 1.";
		m_StoryText.setString(storyStream.str());
		break;

	case 2:
		// Wild West text
		storyStream <<
			"It's high noon partner." <<
			"\nThis machine allows any user to travel through time." <<
			"\nSo be cautious when near the device" <<
			"\n" <<
			"\nWhen ready, press E to start level 2.";
		m_StoryText.setString(storyStream.str());
		break;

	case 3:
		// Fracture Future text
		storyStream <<
			"I am the TERMINATOR." <<
			"\nThis machine allows any user to travel through time." <<
			"\nSo be cautious when near the device" <<
			"\n" <<
			"\nWhen ready, press E to start level 3.";
		m_StoryText.setString(storyStream.str());
		break;

	case 4:
		// Endscreen text
		storyStream <<
			"And so, you pick up the device and finally disarm it." <<
			"\nKnowing the device's potential, you carefully locked it away." <<
			"\nSo that it could never send another victim in a time loop." <<
			"\n" <<
			"\nTHE-END!." <<
			"\nPress Q to finish game and return to main menu";
		m_StoryText.setString(storyStream.str());
		break;
	}
}

void MarineMachine::input()
{
	Event event;

	while (m_Window.pollEvent(event))
	{
		
		if (event.type == Event::Closed)
		{
			m_Window.close(); // Allows the window's X button to work
		}

		if (state == State::MAIN_MENU)
		{
			if (event.type == Event::KeyReleased)
			{
				if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W)
				{
					mainMenu.moveUp();
				}

				if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S)
				{
					mainMenu.moveDown();
				}

				if (event.key.code == Keyboard::Enter)
				{
					switch (mainMenu.getSelectedIndex())
					{
					case 0: // Start the game
						resetGame();
						state = State::STORY_MENU;
						break;

					case 1: // Load
						break;

					case 2: // Scoreboard
						break;

					case 3: // Options
						break;

					case 4: // Exit the game
						m_Window.close();
						break;

					default:
						break;
					}
				}
			}
		}

		// Show some context/lore in this state
		if (state == State::STORY_MENU)
		{
			if (!m_HasReturned)
			{
				if (event.key.code == Keyboard::E)
				{
					// Run the game
					state = State::PLAYING;
				}
			}
			else
			{
				if (event.key.code == Keyboard::Q)
				{
					// Finish the game
					state = State::MAIN_MENU;
				}
			} 

		}

		// Allow player to move in this state
		if (state == State::PLAYING)
		{
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
			{
				state = State::PAUSED;
				return;
			}

			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				marine.moveUp();
				soundManager.playWalk();
			}
			else
			{
				marine.stopUpMovement();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				marine.moveDown();
				soundManager.playWalk();
			}
			else
			{
				marine.stopDownMovement();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				marine.moveLeft();
				soundManager.playWalk();
			}
			else
			{
				marine.stopLeftMovement();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				marine.moveRight();
				soundManager.playWalk();
			}
			else
			{
				marine.stopRightMovement();
			}


			// SHOOTING INPUT
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds() - lastPistolShot.asMilliseconds() >= pistolFireRate.asMilliseconds())
				{
					if (bulletsInClip > 0)
					{
						// Fire the bullet
						bullets[currentBullet].shoot(
							marine.getCenter().x, marine.getCenter().y,
							mouseWorldPosition.x, mouseWorldPosition.y);

						currentBullet++;
						if (currentBullet > 99) { currentBullet = 0; }

						bulletsInClip--;
						lastPistolShot = gameTimeTotal;

						
				
						 soundManager.playShoot();
					}
				}
			}

			// RELOAD INPUT
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::R)
			{
				int bulletsNeeded = clipSize - bulletsInClip;

				if (bulletsSpare >= bulletsNeeded)
				{
					bulletsSpare -= bulletsNeeded;
					bulletsInClip += bulletsNeeded;
					 soundManager.playReload();
				}
				else if (bulletsSpare > 0)
				{
					bulletsInClip += bulletsSpare;
					bulletsSpare = 0;
					 soundManager.playReload();
				}
				else
				{
					 soundManager.playReloadFailed();
				}
			}


			// Handle dodge input (Left Shift)
			if (Keyboard::isKeyPressed(Keyboard::LShift) &&
				!isDodging &&
				(gameTimeTotal - lastDodgeTime >= dodgeCooldown))
			{
				isDodging = true;
				lastDodgeTime = gameTimeTotal;
				originalSpeed = marine.getSpeed(); // Store current speed
				marine.setSpeed(originalSpeed * 2); // Double speed
				soundManager.playDodge(); // Play dodge sound
			}


			// Handle the player quitting
			if (event.key.code == (Keyboard::Escape))
			{
				m_Window.close();
			}

		}

		if (state == State::PAUSED)
		{
			if (event.type == Event::KeyReleased)
			{
				if (event.key.code == Keyboard::Up)
				{
					pauseMenu.moveUp();
				}
				if (event.key.code == Keyboard::Down)
				{
					pauseMenu.moveDown();
				}

				if (event.key.code == Keyboard::Enter)
				{
					switch (pauseMenu.getSelectedIndex())
					{
					case 0: // Resume
						state = State::PLAYING;
						break;

					case 1: // Save
						break;

					case 2: // Options
						break;

					case 3: // Exit to Main Menu
						state = State::MAIN_MENU;
						break;

					default:
						break;
					}
				}
			}
		}
	}
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
	lm.setCurrentLevel(0);

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
