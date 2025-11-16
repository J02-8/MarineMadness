#pragma once
#include "Menu.h"

class PauseMenu : public Menu
{
public:
    PauseMenu(float width, float height);

    void draw(RenderWindow& window) override;

private:
    RectangleShape dimBackground;
};