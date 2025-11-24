#include "OptionMenu.h"
#include "SoundManager.h"
#include <SFML/Audio.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

extern SoundManager soundManager2;

OptionMenu::OptionMenu(float width, float height)
{
    font.loadFromFile("fonts/ByteBounce.ttf");

    Vector2f center(width / 2.f, height / 2.f);
    setupTexts(center);
    setupSlider(center, 600.f, 20.f);
}

void OptionMenu::setupTexts(Vector2f center)
{
    // Title
    m_Title.setFont(font);
    m_Title.setCharacterSize(72);
    m_Title.setFillColor(Color::Cyan);
    m_Title.setString("Options");
    FloatRect titleRect = m_Title.getLocalBounds();
    m_Title.setOrigin(titleRect.width / 2.f, titleRect.height / 2.f);
    m_Title.setPosition(center.x, center.y - 220.f);

    // Volume label
    m_VolLabel.setFont(font);
    m_VolLabel.setCharacterSize(40);
    m_VolLabel.setFillColor(Color::White);
    m_VolLabel.setString("Master Volume");
    FloatRect volumeLableRect = m_VolLabel.getLocalBounds();
    m_VolLabel.setOrigin(volumeLableRect.width / 2.f, volumeLableRect.height / 2.f);
    m_VolLabel.setPosition(center.x, center.y - 80.f);

    // Help text
    m_Help.setFont(font);
    m_Help.setCharacterSize(28);
    m_Help.setFillColor(Color::White);
    m_Help.setString("Left : Decrease   Right : Increase   M : Mute   Esc : Back");
    FloatRect helpRect = m_Help.getLocalBounds();
    m_Help.setOrigin(helpRect.width / 2.f, helpRect.height / 2.f);
    m_Help.setPosition(center.x, center.y + 120.f);

    // Volume value
    m_VolValue.setFont(font);
    m_VolValue.setCharacterSize(36);
    m_VolValue.setFillColor(Color::Yellow);
    m_VolValue.setPosition(center.x, center.y + 40.f);
}

// Setup the slider components
void OptionMenu::setupSlider(Vector2f center, float trackWidth, float trackHeight)
{
    Vector2f trackPos(center.x - trackWidth / 2.f, center.y - 20.f);

	// Track background
    m_Track.setSize(Vector2f(trackWidth, trackHeight));
    m_Track.setPosition(trackPos);
    m_Track.setFillColor(Color(100, 100, 100));
    m_Track.setOutlineColor(Color::Black);
    m_Track.setOutlineThickness(2.f);

	// Filled portion
    m_Filled.setSize(Vector2f(0.f, trackHeight));
    m_Filled.setPosition(trackPos);
    m_Filled.setFillColor(Color(50, 200, 50));

	// Knob
    m_Knob.setRadius(14.f);
    m_Knob.setOrigin(14.f, 14.f);
    m_Knob.setFillColor(Color::White);
    m_Knob.setOutlineColor(Color::Black);
    m_Knob.setOutlineThickness(2.f);
}

// Handle key events for volume control
void OptionMenu::handleEvent(const Event& event)
{
	// Only process key released events
    if (event.type != Event::KeyReleased)
    {
        return;
    }

    // Increase volume
    if (event.key.code == Keyboard::Right)
    {
        float vol = Listener::getGlobalVolume();
        vol = std::min(100.f, vol + 5.f);
        Listener::setGlobalVolume(vol);

        // Keep global SoundManager in sync
        soundManager2.setGlobalVolume(vol);
    }
    // Decrease volume
    else if (event.key.code == Keyboard::Left)
    {
        float vol = Listener::getGlobalVolume();
        vol = std::max(0.f, vol - 5.f);
        Listener::setGlobalVolume(vol);

        soundManager2.setGlobalVolume(vol);
    }
    // Mute toggle
    else if (event.key.code == Keyboard::M)
    {
        float vol = Listener::getGlobalVolume();
        if (vol > 0.f)
        {
            m_prevVolume = vol;
            vol = 0.f;
        }
        else
        {
            vol = m_prevVolume;
        }
        Listener::setGlobalVolume(vol);

        soundManager2.setGlobalVolume(vol);
    }
}

void OptionMenu::draw(RenderWindow& window)
{
    // Update slider based on current volume
    float curVol = Listener::getGlobalVolume();
    float trackWidth = m_Track.getSize().x;
    m_Filled.setSize(Vector2f((curVol / 100.f) * trackWidth, m_Track.getSize().y));
    m_Knob.setPosition(m_Track.getPosition().x + m_Filled.getSize().x,
        m_Track.getPosition().y + m_Track.getSize().y / 2.f);

	// Update volume text
    int volInt = static_cast<int>(round(curVol));
    m_VolValue.setString(to_string(volInt) + "%");
    FloatRect vvBounds = m_VolValue.getLocalBounds();
    m_VolValue.setOrigin(vvBounds.width / 2.f, vvBounds.height / 2.f);

    // Draw elements
    window.draw(m_Title);
    window.draw(m_VolLabel);
    window.draw(m_Track);
    window.draw(m_Filled);
    window.draw(m_Knob);
    window.draw(m_VolValue);
    window.draw(m_Help);
}