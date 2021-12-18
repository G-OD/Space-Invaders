#include <iostream>
#include <vector>
#include <chrono>

#include "Graphics.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define FPS 60


int timeStart;
int oldTimeStart = 0;
float deltaTime;
int fps = 0;
float timescale = 1.0f;

Player player = Player(0.0f, -WINDOW_WIDTH, 10.0f, 2.0f);
std::vector<Enemy> enemies;
bool enemyRight = false;
bool enemyDown = false;
std::vector<Projectile> projectiles;



void reshape(int, int);
void display();
void resetGame();
void timer(int);
void keyboard(unsigned char, int, int);
void keyboardUp(unsigned char, int, int);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Space Invaders");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);

	resetGame();

	glutMainLoop();

	return 0;
}

void reshape(int width, int height)
{
	if (height == 0) height = 1;

	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-WINDOW_WIDTH * aspect, WINDOW_WIDTH * aspect, -WINDOW_HEIGHT, WINDOW_HEIGHT, 0.0f, 1.0f);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Player
	if (player.dead == false)
		DrawSquare(player.pos.x, player.pos.y, player.size);

	// Enemies
	for (int i = 0; i < (int)enemies.size(); ++i)
		DrawSquare(enemies[i].pos.x, enemies[i].pos.y, enemies[i].size);

	// Projectiles
	for (int i = 0; i < (int)projectiles.size(); ++i)
		DrawSquare(projectiles[i].pos.x, projectiles[i].pos.y, projectiles[i].size);

	// DeltaTime
	timeStart = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (((float)timeStart - (float)oldTimeStart) / 10.0f) * timescale;
	oldTimeStart = timeStart;

	glutSwapBuffers();
}

void resetGame()
{
	// Delete all
	projectiles.clear();
	enemies.clear();

	// Reset life
	player.dead = false;

	// Respawn all enemies
	for (float i = 0.0f; i < WINDOW_HEIGHT; i += 100.0f)
	{
		for (float j = -WINDOW_WIDTH + 100.0f; j <= WINDOW_WIDTH - 100.0f; j += 50.0f)
		{
			enemies.push_back(Enemy(j, i, 10.0f));
		}
	}

	glutPostRedisplay();
}

void timer(int state)
{
	// Enemy variables
	int furthest = 0;
	float speed = 1.0f;

	if (player.controls.RESET == true)
	{
		resetGame();
	}

	// While player not dead
	if (player.dead == false)
	{
		// Left
		if (player.controls.LEFT == true && player.pos.x > -WINDOW_WIDTH + player.size)
		{
			player.pos.x -= player.speed * deltaTime;
			glutPostRedisplay();
		}
		// Right
		if (player.controls.RIGHT == true && player.pos.x < WINDOW_WIDTH - player.size)
		{
			player.pos.x += player.speed * deltaTime;
			glutPostRedisplay();
		}
		// Shoot
		if (player.controls.SHOOT == true)
		{
			int lastShotDuration = (int)std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - player.lastShot).count();
			if (lastShotDuration * timescale >= 100) // Delay for each shot
			{
				player.lastShot = Clock::now();
				projectiles.push_back(Projectile(player.pos.x, player.pos.y + (player.size + 5.0f), 8.0f, 5.0f, true));
				glutPostRedisplay();
			}
		}
	}

	/*
		Enemy actions
	*/
	for (int i = 0; i < (int)enemies.size(); ++i)
	{
		// Get furtherst right
		if (enemyRight == true) {
			if (enemies[furthest].pos.x < enemies[i].pos.x) {
				furthest = i;
			}
		}
		else {
			if (enemies[furthest].pos.x > enemies[i].pos.x) {
				furthest = i;
			}
		}

		// Shoot
		int chance;

		if (enemies.size() >= 85) {
			chance = 1000;
			speed = 1.0f;
		}
		else if (enemies.size() > 60) {
			chance = 500;
			speed = 1.5f;
		}
		else if (enemies.size() > 50) {
			chance = 250;
			speed = 2.0f;
		}
		else if (enemies.size() > 25) {
			chance = 100;
			speed = 3.0f;
		}
		else if (enemies.size() > 15) {
			chance = 100;
			speed = 3.0f;
		}
		else if (enemies.size() > 0) {
			chance = 10;
			speed = 5.0f;
		}

		if (player.dead == false)
		{
			bool willShoot = rand() % chance == 0;
			if (willShoot)
			{
				projectiles.push_back(Projectile(enemies[i].pos.x, enemies[i].pos.y - (enemies[i].size + 5.0f), -8.0f, 5.0f, false));
				glutPostRedisplay();
			}
		}
	}

	if (enemies.size() > 0)
	{
		if (enemies[furthest].pos.x > WINDOW_WIDTH - enemies[furthest].size)
		{
			enemyRight = false;
			enemyDown = true;
		}
		else if (enemies[furthest].pos.x < -WINDOW_WIDTH + enemies[furthest].size)
		{
			enemyRight = true;
			enemyDown = true;
		}

		for (int i = 0; i < (int)enemies.size(); ++i)
		{
			if (enemies[i].pos.y - enemies[i].size < player.pos.y + player.size)
			{
				player.dead = true;
				glutPostRedisplay();
			}

			if (enemyRight == true) {
				enemies[i].pos.x += speed * deltaTime;
				glutPostRedisplay();
			}
			else {
				enemies[i].pos.x -= speed * deltaTime;
				glutPostRedisplay();
			}
			if (enemyDown == true) {
				enemies[i].pos.y -= speed * 5 * deltaTime;
				glutPostRedisplay();
			}
		}

		enemyDown = false;
	}

	/*
		Collisions
	*/
	for (int i = 0; i < (int)projectiles.size(); ++i)
	{
		bool destroyed = false;

		for (int j = 1; j < (int)projectiles.size(); ++j)
		{
			if (j == i)
				continue;

			if (projectiles[i].pos.x - projectiles[i].size < projectiles[j].pos.x + projectiles[j].size &&
				projectiles[i].pos.x + projectiles[i].size > projectiles[j].pos.x - projectiles[j].size &&
				projectiles[i].pos.y + projectiles[i].size > projectiles[j].pos.y - projectiles[j].size &&
				projectiles[i].pos.y - projectiles[i].size < projectiles[j].pos.y + projectiles[j].size)
			{
				destroyed = true;

                if (i > j)
                {
                    projectiles.erase(projectiles.begin() + i);
                    projectiles.erase(projectiles.begin() + j);
                }
                else
                {
                    projectiles.erase(projectiles.begin() + j);
                    projectiles.erase(projectiles.begin() + i);
                }

				glutPostRedisplay();
				break;
			}
		}

		if (destroyed == true) {
			destroyed = false;
			continue;
		}

		if (projectiles[i].pos.x < -WINDOW_WIDTH || projectiles[i].pos.x > WINDOW_WIDTH ||
			projectiles[i].pos.y < -WINDOW_HEIGHT || projectiles[i].pos.y > WINDOW_HEIGHT)
		{
			projectiles.erase(projectiles.begin() + i);
			glutPostRedisplay();
		}
		else
		{
			// Player
			if (!destroyed)
			{
				if (projectiles[i].pos.x - projectiles[i].size < player.pos.x + player.size &&
					projectiles[i].pos.x + projectiles[i].size > player.pos.x - player.size &&
					projectiles[i].pos.y + projectiles[i].size > player.pos.y - player.size &&
					projectiles[i].pos.y - projectiles[i].size < player.pos.y + player.size)
				{
					destroyed = true;
					projectiles.erase(projectiles.begin() + i);
					player.dead = true;
					glutPostRedisplay();
				}

				// Enemy
				if (!destroyed)
				{
					for (int j = 0; j < (int)enemies.size(); ++j)
					{
						if (projectiles[i].player == false)
							continue;

						if (projectiles[i].pos.x - projectiles[i].size < enemies[j].pos.x + enemies[j].size &&
							projectiles[i].pos.x + projectiles[i].size > enemies[j].pos.x - enemies[j].size &&
							projectiles[i].pos.y + projectiles[i].size > enemies[j].pos.y - enemies[j].size &&
							projectiles[i].pos.y - projectiles[i].size < enemies[j].pos.y + enemies[j].size)
						{
							destroyed = true;
							projectiles.erase(projectiles.begin() + i);
							enemies.erase(enemies.begin() + j);
							glutPostRedisplay();
							break;
						}
					}
				}

				if (!destroyed)
				{
					projectiles[i].pos.y += projectiles[i].vel * deltaTime;
					glutPostRedisplay();
				}
			}
		}
	}


	std::cout << "Enemies: " << enemies.size() << std::endl;
	std::cout << "Projectiles: " << projectiles.size() << std::endl;

	glutTimerFunc(1000 / FPS, timer, 0);
}

void keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
		case 27:
			exit(0);
			break;
		case '1':
			timescale = 1.0f;
			break;
		case '2':
			timescale = 0.5f;
			break;
		case '3':
			timescale = 0.2f;
			break;
		case 'r':
			player.controls.RESET = true;
			break;
		case 'a':
			player.controls.LEFT = true;
			break;
		case 'd':
			player.controls.RIGHT = true;
			break;
		case 32:
			player.controls.SHOOT = true;
			break;
	}
}
void keyboardUp(unsigned char c, int x, int y)
{
	switch (c)
	{
		case 'r':
			player.controls.RESET = false;
			break;
		case 'a':
			player.controls.LEFT = false;
			break;
		case 'd':
			player.controls.RIGHT = false;
			break;
		case 32:
			player.controls.SHOOT = false;
			break;
	}
}
