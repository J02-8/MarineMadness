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
    SoundBuffer walkBuffer;
    SoundBuffer lazerBuffer;
    SoundBuffer boomBuffer;

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
    Sound walkSound;
    Sound lazerSound;
    Sound boomSound;

    // The Background Music
    Music track_0;
    Music track_1;
    Music track_2;
    Music track_3;

public:

    // Constructor
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
    void playWalk();
    void playLazer();
    void playBoom();

    // Play Music Functions
    void playTrack0();
    void playTrack1();
    void playTrack2();
    void playTrack3();

    // Set the volume
    void setGlobalVolume(float volume);

    // Stop Music Function
    void stopTracks();
};

#pragma once
