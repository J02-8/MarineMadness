#include "MarineMachine.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>

using namespace sf;

int score = 0;

float hitDelay = 0.0f;

void MarineMachine::update(float dtAsSeconds)
{
	int levelNum = 0;

	//start of a new game we will need a new level
	if (m_NewLevelRequired)
	{
		loadLevel();
	}

	// Update the player
	marine.update(dtAsSeconds, Mouse::getPosition());

	m_MainView.setCenter(marine.getCenter());
	m_Window.setView(m_MainView); // Apply the centered view immediately

	// Has the marine player touched time warp?
	if (marine.getPosition().intersects(wp.getPosition()))
	{
		levelNum++;
		lm.setCurrentLevel(levelNum);
	}

}