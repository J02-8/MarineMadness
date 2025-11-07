#include "MarineMachine.h"
#include "Enemy.h"
#include "SoundManager.h"
#include "Pathfinding.h"


using namespace sf;

Enemy** DinoSpawner(int numDinos, IntRect arena);
Enemy** CowboySpawner(int numCowboys, IntRect arena);
Enemy** AndroidSpawner(int numCowboys, IntRect arena);

SoundManager soundManager;

MarineMachine::MarineMachine() : mainMenu(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height)
{
	
	m_Pathfinding = new Pathfinding();
	// Get the screen resolution and create an SFML window and View
	Vector2f resolution;
	
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	state = State::MAIN_MENU;

	m_Window.create(VideoMode(resolution.x, resolution.y), "Marine Madness", Style::Fullscreen);
	//	m_Window.setFramerateLimit(30);
		//sf::Window::setFramerateLimit(unsigned int 	limit)


	// Initialize the full screen view
	m_MainView.setSize(resolution);

	// Setup HUD view
	m_HudView.setSize(resolution);
	m_HudView.setCenter(resolution.x / 2, resolution.y / 2);

	//m_HudView.reset(
		//FloatRect(0, 0, resolution.x, resolution.y));

	//m_HudView.setViewport(sf::FloatRect(0.75f, 0.f, 0.5f, 2.2f));

	//m_HudBackground.setSize(sf::Vector2f(resolution.x * 0.7f, resolution.y * 0.5f));
	//m_HudBackground.setFillColor(sf::Color::Black);

	// Zoom in on player
	m_MainView.zoom(0.7);

	// Can this graphics card use shaders?
	if (!sf::Shader::isAvailable())
	{
		// Time to get a new PC
		m_Window.close();
	}

	m_BackgroundTexture = TextureHolder::GetTexture(
		"graphics/background.jpg");

	// Load the texture for the background vertex array
	m_TextureTiles = TextureHolder::GetTexture(
		"graphics/tile-sheet0.png");

	m_TitleGraphicTexture = TextureHolder::GetTexture(
		"graphics/title-logo.png");

	m_TitleGraphic.setTexture(m_TitleGraphicTexture);
	m_TitleGraphic.setPosition(860, 100);

	storySprite.setTexture(m_BackgroundTexture);
	storySprite.setPosition(-870, 300);

	font.loadFromFile("fonts/ByteBounce.ttf");

	storyText.setFont(font);
	storyText.setCharacterSize(45);
	storyText.setFillColor(Color::White);
	storyText.setPosition(-450, 700);
	std::stringstream storyStream;
	storyStream <<
		"You are Sgt. Ronald McRevolver, investigating an abandoned wearhouse." <<
		"\nYou were informed by the chief that you have retrieve a stolen machine." <<
		"\nThis machine allows any user to travel through time." <<
		"\nSo be cautious when near the device" <<
		"\n" <<
		"\nWhen ready, press E to enter the building.";
	storyText.setString(storyStream.str());


	// Initialize bullets
	currentBullet = 0;
	bulletsSpare = 24;
	bulletsInClip = 6;
	clipSize = 6;

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

	
	

	//clear dinos
	if (dinosaurs != nullptr) {
		for (int i = 0; i < numDinosaurs; i++) {
			delete dinosaurs[i];  // Delete each individual dinosaur
		}
		delete[] dinosaurs;  // Delete the array of pointers
		dinosaurs = nullptr;
	}

	//clear cowboys 
	if (cowboys != nullptr) {
		for (int i = 0; i < numCowboys; i++) {
			delete cowboys[i];  // Delete each individual dinosaur
		}
		delete[] cowboys;  // Delete the array of pointers
		cowboys = nullptr;
	}

	//clear androids
	if (androids != nullptr) {
		for (int i = 0; i < numAndroids; i++) {
			delete androids[i];  // Delete each individual android
		}
		delete[] androids;  // Delete the array of pointers
		dinosaurs = nullptr;
	}

	// Only spawn dinosaurs on level 1
	if (lm.getCurrentLevel() == 1) {
		numDinosaurs = 5; // Adjust number as needed
		dinosaurs = DinoSpawner(numDinosaurs, arena);
		numDinosaursAlive = numDinosaurs;
	}
	else {
		// No dinosaurs on other levels
		numDinosaurs = 0;
		numDinosaursAlive = 0;
		dinosaurs = nullptr;
	}

	// Only spawn cowboys on level 2
	if (lm.getCurrentLevel() == 2) {
		numCowboys = 5; // Adjust number as needed
		cowboys = CowboySpawner(numCowboys, arena);
		numCowboysAlive = numCowboys;
	}
	else {
		// No cowboys on other levels
		numCowboys = 0;
		numCowboysAlive = 0;
		cowboys = nullptr;
	}

	// Only spawn androids on level 3
	if (lm.getCurrentLevel() == 3) {
		numAndroids = 5; // Adjust number as needed
		androids = AndroidSpawner(numAndroids, arena);
		numAndroidsAlive = numAndroids;
	}
	else {
		// No androids on other levels
		numAndroids = 0;
		numAndroidsAlive = 0;
		androids = nullptr;
	}
	

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
	sf::FloatRect arenaBounds = lm.getArenaBounds();

	// Get tile size
	float tileSize = lm.getTileSize(); // You need a getter for TILE_SIZE

	// Pass the arena data to the player
	marine.setArena(arenaBounds, tileSize);

	// Pass arena date to the warp
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

void MarineMachine::input()
{
	Event event;

	while (m_Window.pollEvent(event))
	{
		
		if (event.type == sf::Event::Closed)
		{
			m_Window.close(); // Allows the window's X button to work
		}

		if (state == State::MAIN_MENU)
		{
			if (event.type == sf::Event::KeyReleased)
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
					switch (mainMenu.mainMenuPressed())
					{
					case 0: // // Start the game
						state = State::STORY_MENU;
						break;
					case 4: // Exit the game
						m_Window.close();
					default:
						break;
					}
				}
			}
		}

		// Show some context/lore in this state
		if (state == State::STORY_MENU)
		{
			if (event.key.code == Keyboard::E)
			{
				state = State::PLAYING;
			}

		}

		// Allow player to move in this state
		if (state == State::PLAYING)
		{
			bool joyMenuPressed = sf::Joystick::isButtonPressed(0, 7);

			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				marine.moveUp();
				soundManager.playWalk();
			}
			else
			{
				marine.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				marine.moveDown();
				soundManager.playWalk();
			}
			else
			{
				marine.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				marine.moveLeft();
				soundManager.playWalk();
			}
			else
			{
				marine.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				marine.moveRight();
				soundManager.playWalk();
			}
			else
			{
				marine.stopRight();
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
			if (event.key.code == (Keyboard::Escape) || joyMenuPressed)
			{
				m_Window.close();
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

		// Update particle positions for the main menu
		if (state == State::MAIN_MENU)
		{
			mainMenu.updateParticles(dtAsSeconds);
		}

		update(dtAsSeconds);

		m_Hud.update(bulletsInClip, bulletsSpare, m_PlayerScore);

		draw();
	}
}
