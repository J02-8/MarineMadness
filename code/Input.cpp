#include "MarineMachine.h"
#include "SoundManager.h"
#include <iostream>

using namespace std;

// Access the SoundManager instance defined in Update.cpp
extern SoundManager soundManager2;

// Local SoundManager used by input and gameplay code
SoundManager soundMan;

bool fromMainMenu = false;

void MarineMachine::input()
{
	Event event;

	while (m_Window.pollEvent(event))
	{

		if (event.type == Event::Closed)
		{
			m_Window.close(); // Allows the window's X button to work
		}

		if (state == State::MAIN_MENU)
		{
			if (event.type == Event::KeyReleased)
			{
				if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W)
				{
					mainMenu.moveUp();
				}

				if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S)
				{
					mainMenu.moveDown();
				}

				if (event.key.code == Keyboard::Enter)
				{
					switch (mainMenu.getSelectedIndex())
					{
					case 0: // Start the game
						resetGame();
						state = State::STORY_MENU;
						fromMainMenu = true;
						break;

					case 1: // Load
						break;

					case 2: // Scoreboard
						state = State::SCOREBOARD;
						break;

					case 3: // Options
						m_StateBeforeOption = state;
						state = State::OPTION;
						break;

					case 4: // Exit the game
						m_Window.close();
						break;

					default:
						break;
					}
				}
			}
		}

		// Scoreboard input
		if (state == State::SCOREBOARD)
		{
			if (event.key.code == Keyboard::Escape)
			{
				state = State::MAIN_MENU;
			}
		}

		if (state == State::OPTION)
		{
			if (event.type == Event::KeyReleased)
			{
				// Return to the saved previous state (MAIN_MENU/PAUSED)
				if (event.key.code == Keyboard::Escape)
				{
					state = m_StateBeforeOption;
				}
				else
				{
					optionMenu.handleEvent(event);
				}
			}
		}

		// Show some context/lore in this state
		if (state == State::STORY_MENU)
		{
			if (!m_HasReturned)
			{
				if (event.key.code == Keyboard::E)
				{
					// Run the game
					state = State::PLAYING;

					// Play door sound on game start
					if (fromMainMenu)
					{
						soundMan.playDoor();
						fromMainMenu = false;
					}

					// Play track for relevant point in game
					switch (lm.getCurrentLevel())
					{
					case 1:
						soundMan.stopTracks();
						break;

					case 2:	// Archaic Anarchy
						soundMan.stopTracks();
						soundMan.playTrack1();
						break;

					case 3:	// Wild West
						soundMan.stopTracks();
						soundMan.playTrack2();
						break;

					case 4:	// Fracture Future
						soundMan.stopTracks();
						soundMan.playTrack3();
						break;
					}
				}
			}
			else
			{
				if (event.key.code == Keyboard::Q)
				{
					// Finish the game
					state = State::MAIN_MENU;
					soundMan.stopTracks();
				}
			}

		}

		// Allow player to move in this state
		if (state == State::PLAYING)
		{
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
			{
				state = State::PAUSED;
				return;
			}

			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				marine.moveUp();
				soundMan.playWalk();
			}
			else
			{
				marine.stopUpMovement();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				marine.moveDown();
				soundMan.playWalk();
			}
			else
			{
				marine.stopDownMovement();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				marine.moveLeft();
				soundMan.playWalk();
			}
			else
			{
				marine.stopLeftMovement();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				marine.moveRight();
				soundMan.playWalk();
			}
			else
			{
				marine.stopRightMovement();
			}

			// MELEE INPUT
			if (Mouse::isButtonPressed(Mouse::Right) &&
				gameTimeTotal - lastMeleeAttack >= meleeCooldown)
			{
				isMeleeAttacking = true;
				lastMeleeAttack = gameTimeTotal;
				soundMan.playMelee();

				// Calculate melee direction
				Vector2f playerCenter = marine.getCenter();
				Vector2f direction = mouseWorldPosition - playerCenter;
				float length = sqrt(direction.x * direction.x + direction.y * direction.y); //distance from player to mouse
				direction /= length; // ensure melee only extends a short range / normalize 

				// Set up melee rectangle
				meleeAttackRect.setSize(Vector2f(MELEE_LENGTH, MELEE_WIDTH));
				meleeAttackRect.setOrigin(0, MELEE_WIDTH / 2);
				meleeAttackRect.setPosition(playerCenter);
				meleeAttackRect.setRotation(atan2(direction.y, direction.x) * 180 / 3.141);
				meleeAttackRect.setFillColor(Color(255, 255, 255, 100));
			}


			// SHOOTING INPUT
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds() - lastPistolShot.asMilliseconds() >= pistolFireRate.asMilliseconds())
				{
					if (bulletsInClip > 0)
					{
						// Fire the bullet
						bullets[currentBullet].shoot(
							marine.getCenter().x, marine.getCenter().y,
							mouseWorldPosition.x, mouseWorldPosition.y);

						currentBullet++;
						if (currentBullet > 99) { currentBullet = 0; }

						bulletsInClip--;
						lastPistolShot = gameTimeTotal;

						soundMan.playShoot();
					}
				}
			}

			// RELOAD INPUT
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::R)
			{
				int bulletsNeeded = clipSize - bulletsInClip;

				if (bulletsSpare >= bulletsNeeded)
				{
					bulletsSpare -= bulletsNeeded;
					bulletsInClip += bulletsNeeded;
					soundMan.playReload();
				}
				else if (bulletsSpare > 0)
				{
					bulletsInClip += bulletsSpare;
					bulletsSpare = 0;
					soundMan.playReload();
				}
				else
				{
					soundMan.playReloadFailed();
				}
			}


			// Handle dodge input (Left Shift)
			if (Keyboard::isKeyPressed(Keyboard::LShift) &&
				!isDodging &&
				(gameTimeTotal - lastDodgeTime >= dodgeCooldown))
			{
				isDodging = true;
				lastDodgeTime = gameTimeTotal;
				originalSpeed = marine.getSpeed(); // Store current speed
				marine.setSpeed(originalSpeed * 2); // Double speed
				soundMan.playDodge(); // Play dodge sound
			}

			// Handle the player quitting
			if (event.key.code == (Keyboard::Escape))
			{
				m_Window.close();
			}

		}

		// Pause input
		if (state == State::PAUSED)
		{
			if (event.type == Event::KeyReleased)
			{
				if (event.key.code == Keyboard::Up)
				{
					pauseMenu.moveUp();
				}
				if (event.key.code == Keyboard::Down)
				{
					pauseMenu.moveDown();
				}

				if (event.key.code == Keyboard::Enter)
				{
					switch (pauseMenu.getSelectedIndex())
					{
					case 0: // Resume
						state = State::PLAYING;
						break;

					case 1: // Save
						break;

					case 2: // Options
						m_StateBeforeOption = state;
						state = State::OPTION;
						break;

					case 3: // Exit to Main Menu
						state = State::MAIN_MENU;
						soundMan.stopTracks();
						break;

					default:
						break;
					}
				}
			}
		}
	}
}