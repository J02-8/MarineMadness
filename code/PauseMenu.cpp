#include "PauseMenu.h"
using namespace sf;

PauseMenu::PauseMenu(float width, float height)
    : Menu(width, height, { "Resume", "Save", "Options", "Exit to Main Menu"})
{
    dimBackground.setSize(Vector2f(width, height));
    dimBackground.setFillColor(Color(0, 0, 0, 150)); // Transparant
}

void PauseMenu::draw(RenderWindow& window)
{
    window.draw(dimBackground);
    Menu::draw(window);
}