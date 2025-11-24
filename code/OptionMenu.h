#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class OptionMenu
{
public:
    OptionMenu(float width, float height);
    void draw(RenderWindow& window);

    // Handle key events for volume control
    void handleEvent(const Event& event);

private:
    Font font;
    Text m_Title;
    Text m_VolLabel;
    Text m_VolValue;
    Text m_Help;

    RectangleShape m_Track;
    RectangleShape m_Filled;
    CircleShape m_Knob;

    float m_prevVolume = 100.f; // used for mute toggle

    void setupTexts(Vector2f center);
    void setupSlider(Vector2f center, float trackWidth, float trackHeight);
};