#include "MarineMachine.h"
#include "Enemy.h"
#include "SoundManager.h"

using namespace sf;

Enemy** DinoSpawner(int numDinos, IntRect arena);
Enemy** CowboySpawner(int numCowboys, IntRect arena);
SoundManager soundManager;

MarineMachine::MarineMachine() : mainMenu(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height)
{

	

	// Get the screen resolution and create an SFML window and View
	Vector2f resolution;
	
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	state = State::MAIN_MENU;


	m_Window.create(VideoMode(resolution.x, resolution.y),
		"Marine Madness",
		Style::Fullscreen);
	//	m_Window.setFramerateLimit(30);
		//sf::Window::setFramerateLimit(unsigned int 	limit)


		// Initialize the full screen view
	m_MainView.setSize(resolution);

	//m_HudView.reset(
		//FloatRect(0, 0, resolution.x, resolution.y));

	//m_HudView.setViewport(sf::FloatRect(0.75f, 0.f, 0.5f, 2.2f));

	//m_HudBackground.setSize(sf::Vector2f(resolution.x * 0.7f, resolution.y * 0.5f));
	//m_HudBackground.setFillColor(sf::Color::Black);

	// Two lines below used to create zoomed our screenshots for the book
	//m_BGMainView.zoom(2.5);

	// Zoom in on player
	m_MainView.zoom(0.7);

	// Can this graphics card use shaders?
	if (!sf::Shader::isAvailable())
	{
		// Time to get a new PC
		m_Window.close();
	}

	m_BackgroundTexture = TextureHolder::GetTexture(
		"graphics/background.png");

	// Associate the sprite with the texture
	m_BackgroundSprite.setTexture(m_BackgroundTexture);

	// Load the texture for the background vertex array
	m_TextureTiles = TextureHolder::GetTexture(
		"graphics/tile-sheet0.png");


	//textureMainMenu = TextureHolder::GetTexture("graphics/background-menu.png");
	spriteMainMenu.setTexture(textureMainMenu);
	spriteMainMenu.setPosition(0, 0);


	font.loadFromFile("fonts/ByteBounce.ttf");

	mainMenuText.setFont(font);
	mainMenuText.setCharacterSize(80);
	mainMenuText.setFillColor(Color::White);
	mainMenuText.setPosition(150, 250);
	std::stringstream mainMenuStream;
	mainMenuStream <<
		"Run Away !" <<
		"\n1 - Play Game" <<
		"\n2 - High Scores " <<
		"\n3- Exit";

	mainMenuText.setString(mainMenuStream.str());
	// pause menu options
	pauseMenuText.setFont(font);
	pauseMenuText.setCharacterSize(80);
	pauseMenuText.setFillColor(Color::White);
	pauseMenuText.setPosition(150, 250);
	std::stringstream pauseMenuStream;
	pauseMenuStream <<
		"Game Paused " <<
		"\nR - Resume" <<
		"\nM - Exit to Menu";

	pauseMenuText.setString(pauseMenuStream.str());

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

	// Only spawn dinosaurs on level 1
	if (levelNum == 1) {
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
	if (levelNum == 2) {
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

	// --- CRITICAL FIX: SETTING THE ARENA BOUNDARIES ---

	// 1. Get the level's pixel bounds from the LevelManager. 
	//    (You need a method like getArenaBounds() in your LevelManager)
	sf::FloatRect arenaBounds = lm.getArenaBounds();

	// 2. Get the tile size (Assuming a constant TILE_SIZE is used by LevelManager)
	float tileSize = lm.getTileSize(); // You need a getter for TILE_SIZE

	// 3. Pass the arena data to the player
	marine.setArena(arenaBounds, tileSize);

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
	marine.spawn(Vector2f(lm.getStartPosition()));

	// Spawn Warp
	wp.spawn(Vector2f(500, 100));

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
			m_TextureTiles = TextureHolder::GetTexture("graphics/tile-sheet1.png");
			break;

		case 2:
			m_TextureTiles = TextureHolder::GetTexture("graphics/tile-sheet2.png");
			break;

		case 3:
			m_TextureTiles = TextureHolder::GetTexture("graphics/tile-sheet3.png");
			break;

		default:
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
						lm.setCurrentLevel(0);
						m_NewLevelRequired = true;
						loadLevel();
						state = State::PLAYING;
						break;
					case 4: // Exit the game
						m_Window.close();
					default:
						break;
					}
				}
			}
		}

		if (state == State::PLAYING)
		{
			bool joyMenuPressed = sf::Joystick::isButtonPressed(0, 7);

			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				marine.moveUp();
			}
			else
			{
				marine.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				marine.moveDown();
			}
			else
			{
				marine.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				marine.moveLeft();
			}
			else
			{
				marine.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				marine.moveRight();
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
		draw();
	}
}
