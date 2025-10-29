#include "Enemy.h"
#include "Android.h"
#include <ctime>

Enemy** AndroidSpawner(int numAndroids, IntRect arena)
{



	Enemy** Androids = new Enemy * [numAndroids];

	int maxY = arena.height - 20;
	int minY = arena.top + 20;
	int maxX = arena.width - 20;
	int minX = arena.left + 20;

	for (int i = 0; i < numAndroids; i++)
	{



		// Which side should the cowboy spawn
		srand((int)time(0) * i);
		int side = (rand() % 4);
		float x, y;

		switch (side)
		{
		case 0:
			// left
			x = minX;
			y = (rand() % maxY) + minY;
			break;

		case 1:
			// right
			x = maxX;
			y = (rand() % maxY) + minY;
			break;

		case 2:
			// top
			x = (rand() % maxX) + minX;
			y = minY;
			break;

		case 3:
			// bottom
			x = (rand() % maxX) + minX;
			y = maxY;
			break;
		}

		Androids[i] = new Android();

		// Spawn the new zombie into the array
		Androids[i]->spawn(x, y, i);


	}
	return Androids;

}