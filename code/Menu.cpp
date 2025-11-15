#include "Menu.h"

using namespace sf;
using namespace std;

Menu::Menu(float width, float height, const vector<string>& items)
    : menuWidth(width), menuHeight(height)
{
    font.loadFromFile("fonts/ByteBounce.ttf");

    menuItems.resize(items.size());

    for (int i = 0; i < items.size(); i++)
    {
        menuItems[i].setFont(font);
        menuItems[i].setString(items[i]);
        menuItems[i].setCharacterSize(60);
        menuItems[i].setFillColor(Color::White);
        menuItems[i].setPosition(200.0f, 300 + i * 100);
    }

    menuItems[0].setFillColor(Color::Blue);
}

void Menu::moveUp()
{
    menuItems[selectedIndex].setFillColor(Color::White);

    selectedIndex--;
    if (selectedIndex < 0)
    {
        selectedIndex = menuItems.size() - 1;
    }

    menuItems[selectedIndex].setFillColor(Color::Blue);
}

void Menu::moveDown()
{
    menuItems[selectedIndex].setFillColor(Color::White);

    selectedIndex++;
    if (selectedIndex >= menuItems.size())
    {
        selectedIndex = 0;
    }

    menuItems[selectedIndex].setFillColor(Color::Blue);
}

void Menu::draw(RenderWindow& window)
{
    for (auto& item : menuItems)
    {
        window.draw(item);
    }
}

int Menu::getSelectedIndex() const
{
    return selectedIndex;
}