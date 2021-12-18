#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Vector2.h"

class Projectile
{
	public:
		Vector2 pos;
		float vel;
		float size;
		bool player;

		Projectile(float x, float y, float vel2, float size2, bool player2);
};

#endif