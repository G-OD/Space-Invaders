#ifndef PLAYER_H
#define PLAYER_H

#include "Vector2.h"
#include "Controls.h"
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

class Player
{
	public:
		Controls controls;
		Vector2 pos;
		float size;
		float speed;
		Clock::time_point lastShot = Clock::now();
		bool dead = false;

		Player(float x, float y, float tmpSize, float tmpSpeed);
};

#endif