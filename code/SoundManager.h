#pragma once
#include <SFML/Audio.hpp>

using namespace sf;

class SoundManager
{
private:
	// The buffers
    SoundBuffer hitBuffer;
    SoundBuffer splatBuffer;
    SoundBuffer shootBuffer;
    SoundBuffer shotGunBuffer;
    SoundBuffer reloadBuffer;
    SoundBuffer reloadFailedBuffer;
    SoundBuffer powerupBuffer;
    SoundBuffer pickupBuffer;
    SoundBuffer dodgeBuffer;
    SoundBuffer meleeBuffer;

	// The Sounds
    Sound hitSound;
    Sound splatSound;
    Sound shootSound;
    Sound shotgunSound;
    Sound reloadSound;
    Sound reloadFailedSound;
    Sound powerupSound;
    Sound pickupSound;
    Sound dodgeSound;
    Sound meleeSound;

public:

	SoundManager();

    // Play Sound Functions
    void playHit();
    void playSplat();
    void playShoot();
    void playShotgun();
    void playReload();
    void playReloadFailed();
    void playPowerup();
    void playPickup();
    void playDodge();
    void playMelee();

    // Set the volume
    void setGlobalVolume(float volume);
};

#pragma once
