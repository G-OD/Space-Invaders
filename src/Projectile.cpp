#include "Projectile.h"

Projectile::Projectile(float x, float y, float vel2, float size2, bool player2)
{
	pos.x = x;
	pos.y = y;
	vel = vel2;
	size = size2;
	player = player2;
}