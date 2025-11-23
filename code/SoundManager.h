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
    SoundBuffer reloadBuffer;
    SoundBuffer reloadFailedBuffer;
    SoundBuffer dodgeBuffer;
    SoundBuffer meleeBuffer;
    SoundBuffer walkBuffer;
    SoundBuffer lazerBuffer;
    SoundBuffer boomBuffer;
    SoundBuffer doorBuffer;
    SoundBuffer warpBuffer;

	// The Sounds
    Sound hitSound;
    Sound splatSound;
    Sound shootSound;
    Sound reloadSound;
    Sound reloadFailedSound;
    Sound dodgeSound;
    Sound meleeSound;
    Sound walkSound;
    Sound lazerSound;
    Sound boomSound;
    Sound doorSound;
    Sound warpSound;

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
    void playReload();
    void playReloadFailed();
    void playDodge();
    void playMelee();
    void playWalk();
    void playLazer();
    void playBoom();
    void playDoor();
    void playWarp();

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
