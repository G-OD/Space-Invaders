#include "Graphics.h"

void DrawSquare(float x, float y, float size)
{
	glBegin(GL_POLYGON);
		glColor3f(1.0f, 1.0f, 1.0f);
			glVertex2f(x - size, y + size);
			glVertex2f(x + size, y + size);
			glVertex2f(x + size, y - size);
			glVertex2f(x - size, y - size);
	glEnd();
}