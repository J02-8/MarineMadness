#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace sf;

class Menu
{
public:
    Menu(float width, float height, const vector<string>& items);

    virtual void draw(RenderWindow& window);

    void moveUp();
    void moveDown();
    int getSelectedIndex() const;

protected:
    Font font;
    vector<Text> menuItems;
    int selectedIndex = 0;
    float menuWidth;
    float menuHeight;
};