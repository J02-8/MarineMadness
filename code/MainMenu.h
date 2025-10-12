#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>


using namespace std;
using namespace sf;

class MainMenu
{
public:
	MainMenu(float width, float hight);

	// Draws the main menu
	void draw(RenderWindow& window);
	
	// Move the menu selection up
	void moveUp();

	// Move the menu selection Down
	void moveDown();

	// Returns the index of the current selected item
	int mainMenuPressed()
	{
		return mainMenuSelected;
	}

	// Destructor
	~MainMenu();

private:
	int mainMenuSelected; // Which item is currently selected
	Font font; // Display font
	Text mainMenu[5]; // Array of menu items
};