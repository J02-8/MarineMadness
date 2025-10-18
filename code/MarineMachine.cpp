#include "MarineMachine.h"
using namespace sf;

MarineMachine::MarineMachine()
{
	// Get the screen resolution and create an SFML window and View
	Vector2f resolution;
	state = State::PLAYING;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;


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
	//m_MainView.zoom(1.5);

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


	font.loadFromFile("fonts/zombiecontrol.ttf");

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

}

void MarineMachine::loadLevel()
{
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
	marine.spawn(Vector2f(200, 900));

	// Spawn Warp
	wp.spawn(Vector2f(500, 900));

	// Make sure this code isn't run again
	m_NewLevelRequired = false;

	
}

void MarineMachine::input()
{
	Event event;

	while (m_Window.pollEvent(event))
	{
		/*
		if (state == State::MAIN_MENU)
		{
			// Handle the player levelling up
			if (event.key.code == Keyboard::Num1)
			{
				// Increase fire rate
				lm.setCurrentLevel(0);
				m_NewLevelRequired = true;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{

				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				state = State::GAME_OVER;
			}
		}
		*/
		if (event.type == sf::Event::Closed)
		{
			m_Window.close(); // Allows the window's X button to work
		}

		if (event.key.code == Keyboard::Num1)
		{
			lm.setCurrentLevel(0);
			m_NewLevelRequired = true;
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
		//m_GameTimeTotal += dt;
		// Make a decimal fraction from the delta time
		float dtAsSeconds = dt.asSeconds();
		
		input();
		update(dtAsSeconds);
		draw();
	}
}
