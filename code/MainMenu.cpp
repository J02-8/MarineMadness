#include "MainMenu.h"

// Constructor
MainMenu::MainMenu(float width, float height)
{
    if (!font.loadFromFile("fonts/zombiecontrol.ttf"))
    {
        std::cout << "Failed to load font!\n";
    }

    std::string menuItems[] = { "Play", "Load", "Scoreboard", "Options", "Exit" };

    for (int i = 0; i < 5; i++)
    {
        mainMenu[i].setFont(font);
        mainMenu[i].setFillColor(Color::White);
        mainMenu[i].setString(menuItems[i]);
        mainMenu[i].setCharacterSize(50);
        mainMenu[i].setPosition(400, 200 + i * 100);
    }

    // Start with the first item selected
    mainMenuSelected = 0;
    mainMenu[mainMenuSelected].setFillColor(Color::Blue);
}

// Destructor
MainMenu::~MainMenu()
{

}

// Draw the main menu
void MainMenu::draw(RenderWindow& window)
{
    for (int i = 0; i < 5; i++)
    {
        window.draw(mainMenu[i]);
    }
}

// Handle moving up
void MainMenu::moveUp()
{
    // Unhighlight the current item
    mainMenu[mainMenuSelected].setFillColor(Color::White);

    // Move up
    mainMenuSelected--;

    // Wrap to bottom if necessary
    if (mainMenuSelected < 0)
        mainMenuSelected = 4;

    // Highlight new item
    mainMenu[mainMenuSelected].setFillColor(Color::Blue);
}

// Handle moving down
void MainMenu::moveDown()
{
    // Unhighlight the current item
    mainMenu[mainMenuSelected].setFillColor(Color::White);

    // Move down
    mainMenuSelected++;

    // Wrap to top if necessary
    if (mainMenuSelected > 4)
        mainMenuSelected = 0;

    // Highlight new item
    mainMenu[mainMenuSelected].setFillColor(Color::Blue);
}