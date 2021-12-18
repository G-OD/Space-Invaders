#ifndef ENEMY_H
#define ENEMY_H

#include "Vector2.h"

class Enemy
{
	public:
		Vector2 pos;
		float size;

		Enemy(float x, float y, float size2);		
};

#endif