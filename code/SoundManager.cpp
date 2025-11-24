#include "SoundManager.h"
#include <SFML/Audio.hpp>

using namespace sf;

SoundManager::SoundManager()
{
    // Load buffers
    hitBuffer.loadFromFile("sound/hit.wav");
    splatBuffer.loadFromFile("sound/splat.wav");
    shootBuffer.loadFromFile("sound/shoot.wav");
    reloadBuffer.loadFromFile("sound/reload.wav");
    reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
    dodgeBuffer.loadFromFile("sound/dodge.wav");
    meleeBuffer.loadFromFile("sound/melee.wav");
    walkBuffer.loadFromFile("sound/walk.wav");
    lazerBuffer.loadFromFile("sound/laser_shoot.wav");
    boomBuffer.loadFromFile("sound/boom.wav");
    doorBuffer.loadFromFile("sound/metal-door.wav");
    warpBuffer.loadFromFile("sound/warp.wav");

    // Attach buffers to sounds
    hitSound.setBuffer(hitBuffer);
    splatSound.setBuffer(splatBuffer);
    shootSound.setBuffer(shootBuffer);
    reloadSound.setBuffer(reloadBuffer);
    reloadFailedSound.setBuffer(reloadFailedBuffer);
    dodgeSound.setBuffer(dodgeBuffer);
    meleeSound.setBuffer(meleeBuffer);
    walkSound.setBuffer(walkBuffer);
    lazerSound.setBuffer(lazerBuffer);
    boomSound.setBuffer(boomBuffer);
    doorSound.setBuffer(doorBuffer);
    warpSound.setBuffer(warpBuffer);

    // Load music
    track_0.openFromFile("sound/tracks/track_0.mp3");
    track_1.openFromFile("sound/tracks/track_1.mp3");
    track_2.openFromFile("sound/tracks/track_2.mp3");
    track_3.openFromFile("sound/tracks/track_3.mp3");
}

// Play Functions
void SoundManager::playHit() { hitSound.play(); }
void SoundManager::playSplat() { splatSound.play(); }
void SoundManager::playShoot() { shootSound.play(); }
void SoundManager::playReload() { reloadSound.play(); }
void SoundManager::playReloadFailed() { reloadFailedSound.play(); }
void SoundManager::playDodge() { dodgeSound.play(); }
void SoundManager::playMelee() { meleeSound.play(); }
void SoundManager::playWalk() { walkSound.play(); }
void SoundManager::playLazer() { lazerSound.play(); }
void SoundManager::playBoom() { boomSound.play(); }
void SoundManager::playDoor() { doorSound.play(); }
void SoundManager::playWarp() { warpSound.play(); }

// Play music functions
void SoundManager::playTrack0()
{
    // Play menu music
    if (track_0.getStatus() != Music::Playing)
    {
        track_0.play();
    }
}
void SoundManager::playTrack1() 
{
    // Play level 1, prehistoric music
    if (track_1.getStatus() != Music::Playing)
    {
        track_1.play();
    } 
}
void SoundManager::playTrack2() 
{ 
    // Play level 2, western music
    if (track_2.getStatus() != Music::Playing)
    {
        track_2.play();
    }
}
void SoundManager::playTrack3() 
{ 
    // Play level 3, future music
    if (track_3.getStatus() != Music::Playing)
    {
        track_3.play();
    }
}

// Set the volume of each sound to the global volume level
void SoundManager::setGlobalVolume(float volume)
{
    hitSound.setVolume(volume);
    splatSound.setVolume(volume);
    shootSound.setVolume(volume);
    reloadSound.setVolume(volume);
    reloadFailedSound.setVolume(volume);
    dodgeSound.setVolume(volume);
    meleeSound.setVolume(volume);
    walkSound.setVolume(volume);
    lazerSound.setVolume(volume);
    boomSound.setVolume(volume);
    doorSound.setVolume(volume);
    warpSound.setVolume(volume);
    track_1.setVolume(volume);
    track_2.setVolume(volume);
    track_3.setVolume(volume);
}

// Stop all currently playing music
void SoundManager::stopTracks()
{
    if (track_0.getStatus() == Music::Playing)
    {
        track_0.stop();
    }
    if (track_1.getStatus() == Music::Playing)
    {
        track_1.stop();
    }
    if (track_2.getStatus() == Music::Playing)
    {
        track_2.stop();
    }
    if (track_3.getStatus() == Music::Playing)
    {
        track_3.stop();
    }
}