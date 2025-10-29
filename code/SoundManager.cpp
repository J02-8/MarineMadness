#include "SoundManager.h"
#include <SFML/Audio.hpp>

using namespace sf;

SoundManager::SoundManager()
{
    // Load buffers
    hitBuffer.loadFromFile("sound/hit.wav");
    splatBuffer.loadFromFile("sound/splat.wav");
    shootBuffer.loadFromFile("sound/shoot.wav");
    shotGunBuffer.loadFromFile("sound/shotGun.wav");
    reloadBuffer.loadFromFile("sound/reload.wav");
    reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
    powerupBuffer.loadFromFile("sound/powerup.wav");
    pickupBuffer.loadFromFile("sound/pickup.wav");
    dodgeBuffer.loadFromFile("sound/dodge.wav");
    meleeBuffer.loadFromFile("sound/melee.wav");
    walkBuffer.loadFromFile("sound/walk.wav");
    lazerBuffer.loadFromFile("sound/laser_shoot.wav");

    // Attach buffers to sounds
    hitSound.setBuffer(hitBuffer);
    splatSound.setBuffer(splatBuffer);
    shootSound.setBuffer(shootBuffer);
    shotgunSound.setBuffer(shotGunBuffer);
    reloadSound.setBuffer(reloadBuffer);
    reloadFailedSound.setBuffer(reloadFailedBuffer);
    powerupSound.setBuffer(powerupBuffer);
    pickupSound.setBuffer(pickupBuffer);
    dodgeSound.setBuffer(dodgeBuffer);
    meleeSound.setBuffer(meleeBuffer);
    walkSound.setBuffer(walkBuffer);
    lazerSound.setBuffer(lazerBuffer);
}

// Play Functions
void SoundManager::playHit() { hitSound.play(); }
void SoundManager::playSplat() { splatSound.play(); }
void SoundManager::playShoot() { shootSound.play(); }
void SoundManager::playShotgun() { shotgunSound.play(); }
void SoundManager::playReload() { reloadSound.play(); }
void SoundManager::playReloadFailed() { reloadFailedSound.play(); }
void SoundManager::playPowerup() { powerupSound.play(); }
void SoundManager::playPickup() { pickupSound.play(); }
void SoundManager::playDodge() { dodgeSound.play(); }
void SoundManager::playMelee() { meleeSound.play(); }
void SoundManager::playWalk() { walkSound.play(); }
void SoundManager::playLazer() { lazerSound.play(); }

// Set the volume of each sound to the global volume level
void SoundManager::setGlobalVolume(float volume)
{
    hitSound.setVolume(volume);
    splatSound.setVolume(volume);
    shootSound.setVolume(volume);
    shotgunSound.setVolume(volume);
    reloadSound.setVolume(volume);
    reloadFailedSound.setVolume(volume);
    powerupSound.setVolume(volume);
    pickupSound.setVolume(volume);
    dodgeSound.setVolume(volume);
    meleeSound.setVolume(volume);
    walkSound.setVolume(volume);
    lazerSound.setVolume(volume);
}