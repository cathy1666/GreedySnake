#include <gl/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI				3.14159
#define WIN_WIDTH		48
#define WIN_HEIGHT		26

char map[WIN_HEIGHT][WIN_WIDTH];

enum FruitType { ORANGE = 0, STRAWBERRY = 1, APPLE = 2, BANANA = 3, GRAPE = 4};
enum Direction { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 };

//enum Direction head_dir = RIGHT; ###############
int head_dir = RIGHT;
int snakeX[100], snakeY[100];
int snake_length = 0;// only body length

int score = 0;
int gameover = 0;
int pause = 0;
int REFRESH_TIMER = 380;

/* you may have to change the include to <glut.h> or
elsewhere depending on where it is stored on your system */

//-------------------------------------------------------------------------------

void DrawCircle(float x, float y, float r)
{
	float cx, cy, theta;

	glBegin(GL_POLYGON);
	for (theta = -180.0; theta <= 180.0; theta += 1.0)
	{
		cx = x + r * cos(theta);
		cy = y + r * sin(theta);
		glVertex2f(cx, cy);
	}
	glEnd();
}
void DrawArc(double x, double y, double start_angle, double end_angle, double delta_angle, double radius, int fill)
{
	double i;
	if (fill)
	{
		glBegin(GL_TRIANGLE_FAN);
	}
	else
	{
		glBegin(GL_LINE_STRIP);
	}
	for (i = start_angle; i <= end_angle; i += delta_angle)
	{
		double vx = x + radius * cos(i);
		double vy = y + radius * sin(i);
		glVertex2d(vx, vy);
	}
	glEnd();
}
void DrawPie(double x, double y, double start_angle, double end_angle, double delta_angle, double radius, int fill)
{
	DrawArc(x, y, start_angle, end_angle, delta_angle, radius, fill);
	if (fill)
	{
		glBegin(GL_TRIANGLE_FAN);
	}
	else
	{
		glBegin(GL_LINES);
	}
	glVertex2d(x, y);
	glVertex2d(x + radius*cos(start_angle), y + radius*sin(start_angle));
	if (!fill)
	{
		glVertex2d(x, y);
	}
	glVertex2d(x + radius*cos(end_angle), y + radius*sin(end_angle));

	glEnd();
}

void DrawOrange(float x, float y)
{
	x += 0.5;
	y += 0.5;

	//head
	glColor3ub(128, 64, 64);
	glBegin(GL_POLYGON);
	glVertex2f(x + 0.0, y + 0.5);
	glVertex2f(x + 0.0, y + 0.6);
	glVertex2f(x + 0.1, y + 0.8);
	glVertex2f(x + 0.2, y + 0.85);
	glVertex2f(x + 0.25, y + 0.8);
	glVertex2f(x + 0.1, y + 0.7);
	glVertex2f(x + 0.1, y + 0.5);
	glEnd();
	
	//leaf
	glColor3ub(0, 160, 0);
	glBegin(GL_POLYGON);
	glVertex2f(x - 0.4, y + 0.9);
	glVertex2f(x - 0.1, y + 0.8);
	glVertex2f(x, y + 0.5);
	glEnd();
	glColor3ub(0, 120, 0);
	glBegin(GL_POLYGON);
	glVertex2f(x - 0.4, y + 0.9);
	glVertex2f(x - 0.3, y + 0.6);
	glVertex2f(x, y + 0.5);
	glEnd();

	//pulp
	glColor3ub(255, 128, 0);
	DrawCircle(x, y, 0.5);
	glColor3ub(200, 100, 0);
	glBegin(GL_POINTS);
	glVertex2f(x, y + 0.1);
	glVertex2f(x + 0.1, y + 0.3);
	glVertex2f(x + 0.3, y + 0.1);
	glEnd();
}
void DrawStrawberry(float x, float y)
{
	x += 0.5;
	y += 0.5;

	//leaf
	glColor3ub(0, 130, 0);
	glBegin(GL_POLYGON);
	glVertex2f(x - 0.3, y + 0.5);
	glVertex2f(x - 0.3, y + 0.75);
	glVertex2f(x - 0.1 , y + 0.65);
	glVertex2f(x       , y + 0.75);
	glVertex2f(x + 0.1 , y + 0.65);
	glVertex2f(x + 0.3, y + 0.75);
	glVertex2f(x + 0.3, y + 0.5);
	glEnd();

	//pulp
	glColor3ub(255, 50, 70);
	DrawCircle(x - 0.25, y + 0.25, 0.25);
	DrawCircle(x + 0.25, y + 0.25, 0.25);
	DrawCircle(x       , y - 0.3 , 0.20);
	glBegin(GL_POLYGON);
	glVertex2f(x - 0.5 , y + 0.22);
	glVertex2f(x - 0.25, y + 0.5);
	glVertex2f(x + 0.25, y + 0.5);
	glVertex2f(x + 0.5 , y + 0.22);
	glVertex2f(x + 0.25, y - 0.3);
	glVertex2f(x - 0.25, y - 0.3);
	glEnd();
	glColor3ub(200, 40, 60);
	glBegin(GL_POINTS);
	glVertex2f(x - 0.2 , y + 0.2);
	glVertex2f(x       , y + 0.2);
	glVertex2f(x + 0.2 , y + 0.2);
	glVertex2f(x - 0.12, y);
	glVertex2f(x + 0.12, y);
	glVertex2f(x       , y - 0.2);
	glEnd();

#if 0
	glPushMatrix();

	glTranslatef(x, y, 0);
	glScalef(0.8, 0.8, 1.0);
	glTranslatef(-x, -y, 0);

	glColor3f(0.0, 0.65, 0.0);
	DrawArc(x, y + 0.8, 0, PI, PI / 180, 1.16, 1);

	glColor3f(0.6, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(x - 1.2f, y + 1.2f, 0.0f);
	glVertex3f(x, y - 1.2f, 0.0f);
	glVertex3f(x + 1.2f, y + 1.2f, 0.0f);
	glEnd();

	glColor3f(0.0, 0.65, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex2f(x - 1.2, y + 1.2);
	glVertex2f(x - 0.4, y + 1.2);
	glVertex2f(x - 0.8, y + 0.4);

	glVertex2f(x - 0.4, y + 1.2);
	glVertex2f(x + 0.4, y + 1.2);
	glVertex2f(x, y + 0.4);

	glVertex2f(x + 0.4, y + 1.2);
	glVertex2f(x + 1.2, y + 1.2);
	glVertex2f(x + 0.8, y + 0.4);
	glEnd();
	DrawCircle(x, y + 2.4, 0.16);
	glBegin(GL_QUADS);
	glVertex3f(x - 0.08f, y + 2.4f, 0.0f);
	glVertex3f(x + 0.16f, y + 2.4f, 0.0f);
	glVertex3f(x + 0.16f, y + 1.6f, 0.0f);
	glVertex3f(x - 0.16f, y + 1.6f, 0.0f);
	glEnd();
	glPopMatrix();
#endif
}
void DrawGrape(float x, float y)
{
	x += 0.5;
	y += 0.5;

	//head
	x -= 0.05;
	y -= 0.1;
	glColor3ub(128, 64, 64);
	glBegin(GL_POLYGON);
	glVertex2f(x + 0.0, y + 0.5);
	glVertex2f(x + 0.0, y + 0.6);
	glVertex2f(x + 0.1, y + 0.8);
	glVertex2f(x + 0.2, y + 0.85);
	glVertex2f(x + 0.25, y + 0.8);
	glVertex2f(x + 0.1, y + 0.7);
	glVertex2f(x + 0.1, y + 0.5);
	glEnd();
	x += 0.05;
	y += 0.1;

	//leaf
	x += 0.05;
	y += 0.05;
	glColor3ub(0, 160, 0);
	glBegin(GL_POLYGON);
	glVertex2f(x + 0.1,y + 0.9);
	glVertex2f(x + 0.4,y + 0.9);
	glVertex2f(x + 0.1,y + 0.6);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(x - 0.1, y + 0.9);
	glVertex2f(x + 0.1, y + 0.75);
	glVertex2f(x + 0.1, y + 0.6);
	glVertex2f(x - 0.1, y + 0.6);
	glEnd();
	glColor3ub(0, 120, 0);
	glBegin(GL_POLYGON);
	glVertex2f(x + 0.4, y + 0.9);
	glVertex2f(x + 0.4, y + 0.6);
	glVertex2f(x + 0.1, y + 0.6);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(x + 0.1,	y + 0.6);
	glVertex2f(x + 0.25,y + 0.6);
	glVertex2f(x + 0.4, y + 0.4);
	glVertex2f(x + 0.1, y + 0.4);
	glEnd();
	x -= 0.05;
	y -= 0.05;

	//pulp
	glColor3ub(150, 70, 200);
	DrawCircle(x - 0.4,	y + 0.25,	 0.2);
	DrawCircle(x,		y + 0.25,	 0.2);
	DrawCircle(x + 0.4,	y + 0.25,	 0.2);
	DrawCircle(x - 0.2,	y - 0.05,	 0.2);
	DrawCircle(x + 0.2,	y - 0.05,	 0.2);
	DrawCircle(x,		y - 0.35,	 0.2);
}
void DrawBanana(float x, float y)
{
	x += 0.5;
	y += 0.5;

	glColor3ub(255, 200, 0);
	glBegin(GL_POLYGON);
	glVertex3f(x + 0.35f,	y + 0.25f,	0.0f);
	glVertex3f(x + 0.35f,	y + 0.6f,	 0.0f);
	glVertex3f(x + 0.65f,	y + 0.4f,	 0.0f);
	glVertex3f(x + 0.5f,	y + 0.3f,	 0.0f);
	glVertex3f(x + 0.45f,	y + 0.1f,	 0.0f);
	glVertex3f(x + 0.4f,	y - 0.2f,	 0.0f);
	glVertex3f(x + 0.1f,	y - 0.4f,	 0.0f);
	glVertex3f(x - 0.35f,	y - 0.5f,	 0.0f);
	glVertex3f(x - 0.45f,	y - 0.4f,	 0.0f);
	glVertex3f(x - 0.5f,	y - 0.2f,	 0.0f);
	glVertex3f(x - 0.45f,	y - 0.3f,	 0.0f);
	glVertex3f(x - 0.3f,	y - 0.2f,	 0.0f);
	glVertex3f(x,			y - 0.2f,	 0.0f);
	glVertex3f(x + 0.2f,	y + 0.15f,	0.0f);
	glEnd();

#if 0
	int i;
	float j, k, m, n;
	glPushMatrix();

	glColor3f(0.5f, 0.20f, 0.05f);
	glBegin(GL_POLYGON);
	glVertex3f(x - 1.07f, y + 0.92f, 0.0f);
	glVertex3f(x + 1.28f, y + 0.92f, 0.0f);
	glVertex3f(x + 1.28f, y + 0.44f, 0.0f);
	glVertex3f(x - 1.07f, y + 0.44f, 0.0f);
	glEnd();

	DrawCircle(x - 1.07f, y + 0.68, 0.24);
	DrawCircle(x + 1.28f, y + 0.68, 0.24);
	glColor3f(1.0, 0.8, 0.0);
	for (i = 0; i<4; i++)
	{
		if (i % 2 == 0)
			glColor3f(1.0, 1.0, 0.0);
		else
			glColor3f(1.0, 0.8, 0.0);
		j = i*0.47;
		k = i*0.47;
		m = i*0.47;
		n = i*0.47;
		glBegin(GL_POLYGON);
		glVertex3f(x - 1.07f + j, y + 0.44f, 0.0f);
		glVertex3f(x - 0.64f + k, y + 0.44f, 0.0f);
		glVertex3f(x - 0.72f + m, y - 1.14f, 0.0f);
		glVertex3f(x - 1.26f + n, y - 1.14f, 0.0f);
		glEnd();
		DrawCircle(x - 1.1f + j, y - 1.14, 0.28);
	}
	glPopMatrix();
#endif
}
void DrawApple(float x, float y)
{
	x += 0.5;
	y += 0.5;

	glPushMatrix();

	glColor3f(0.0f, 0.2f, 0.0f);
	DrawArc(x + 0.64, y + 3.6, 220 * PI / 180, 250 * PI / 180, PI / 180, 2.4, 1);
	DrawArc(x - 1.76, y - 0.4, 40 * PI / 180, 70 * PI / 180, PI / 180, 2.4, 1);

	//pulp
	glColor3f(0.8, 0.0, 0.0);
	DrawCircle(x, y, 0.6);

	//
	glColor3f(0.5f, 0.20f, 0.05f);
	DrawArc(x + 1.28, y + 0.64, 120 * PI / 180, PI, PI / 180, 1.28, 0);
	DrawArc(x, y + 1.76, 250 * PI / 180, 290 * PI / 180, PI / 180, 1.2, 0);

	glPopMatrix();
}

void DrawGrass(float x, float y)
{
	y += 0.5;

	glPushMatrix();
	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.7, 0.0);
	glVertex2f(x - 1.0, y + 0.5);
	glColor3f(0.0, 0.5, 0.0);
	glVertex2f(x - 1.0, y - 0.5);
	glColor3f(0.0, 0.2, 0.0);
	glVertex2f(x, y - 0.5);

	glColor3f(0.0, 0.7, 0.0);
	glVertex2f(x, y + 0.5);
	glColor3f(0.0, 0.5, 0.0);
	glVertex2f(x - 0.5, y - 0.5);
	glColor3f(0.0, 0.2, 0.0);
	glVertex2f(x + 0.5, y - 0.5);

	glColor3f(0.0, 0.7, 0.0);
	glVertex2f(x + 1.0, y + 0.5);
	glColor3f(0.0, 0.5, 0.0);
	glVertex2f(x, y - 0.5);
	glColor3f(0.0, 0.2, 0.0);
	glVertex2f(x + 1.0, y - 0.5);
	glEnd();
	glPopMatrix();
}
void DrawBrick(float x, float y, int color)
{
	if (color == 1) 	glColor3ub(200, 60, 0);
	else if (color == 2) glColor3ub(50, 200, 0);

	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + 1.0, y);
	glVertex2f(x + 1.0, y + 1.0);
	glVertex2f(x, y + 1.0);
	glEnd();

	glColor3ub(0, 0, 0);

	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x + 1.0, y);
	glVertex2f(x + 1.0, y + 1.0);
	glVertex2f(x, y + 1.0);
	glEnd();
}
void DrawSnackHead(float x, float y, int dir)
{
	x += 0.5;
	y += 0.5;

	switch(dir)
	{
	case UP:
		//Tongue
		glColor3ub(255, 0, 0);
		glBegin(GL_LINES);
		glVertex2f(x + 0.0, y + 0.7); 
		glVertex2f(x + 0.0, y + 1.1); 
		glVertex2f(x + 0.0, y + 1.1); 
		glVertex2f(x + 0.2, y + 1.3); 
		glVertex2f(x + 0.0, y + 1.1); 
		glVertex2f(x - 0.2, y + 1.3); 
		glEnd();
		//head
		glColor3ub(0, 120, 0);
		DrawCircle(x, y, 0.75);
		//Eyes
		glColor3ub(0, 0, 0);
		DrawCircle(x + 0.25, y + 0.25, 0.1);
		DrawCircle(x - 0.25, y + 0.25, 0.1);
		break;

	case DOWN:
		//Tongue
		glColor3ub(255, 0, 0);
		glBegin(GL_LINES);
		glVertex2f(x + 0.0, y - 0.7);
		glVertex2f(x + 0.0, y - 1.1);
		glVertex2f(x + 0.0, y - 1.1);
		glVertex2f(x + 0.2, y - 1.3);
		glVertex2f(x + 0.0, y - 1.1);
		glVertex2f(x - 0.2, y - 1.3);
		glEnd();
		//head
		glColor3ub(0, 120, 0);
		DrawCircle(x, y, 0.75);
		//Eyes
		glColor3ub(0, 0, 0);
		DrawCircle(x + 0.25, y - 0.25, 0.1);
		DrawCircle(x - 0.25, y - 0.25, 0.1);
		break;

	case LEFT:
		//Tongue
		glColor3ub(255, 0, 0);
		glBegin(GL_LINES);
		glVertex2f(x - 0.7, y + 0.0);
		glVertex2f(x - 1.1, y + 0.0);
		glVertex2f(x - 1.1, y + 0.0);
		glVertex2f(x - 1.3, y + 0.2);
		glVertex2f(x - 1.1, y + 0.0);
		glVertex2f(x - 1.3, y - 0.2);
		glEnd();
		//head
		glColor3ub(0, 120, 0);
		DrawCircle(x, y, 0.75);
		//Eyes
		glColor3ub(0, 0, 0);
		DrawCircle(x - 0.25, y + 0.25, 0.1);
		DrawCircle(x - 0.25, y - 0.25, 0.1);
		break;

	case RIGHT:
		//Tongue
		glColor3ub(255, 0, 0);
		glBegin(GL_LINES);
		glVertex2f(x + 0.7, y + 0.0);
		glVertex2f(x + 1.1, y + 0.0);
		glVertex2f(x + 1.1, y + 0.0);
		glVertex2f(x + 1.3, y + 0.2);
		glVertex2f(x + 1.1, y + 0.0);
		glVertex2f(x + 1.3, y - 0.2);
		glEnd();
		//head
		glColor3ub(0, 120, 0);
		DrawCircle(x, y, 0.75);
		//Eyes
		glColor3ub(0, 0, 0);
		DrawCircle(x + 0.25, y + 0.25, 0.1);
		DrawCircle(x + 0.25, y - 0.25, 0.1);
		break;
	}

#if 0
	//white eyes
	glColor3ub(255, 255, 255);
	glBegin(GL_POLYGON);
	glVertex2f(x, y + 0.15);
	glVertex2f(x - 0.1, y + 0.05);
	glVertex2f(x - 0.4, y + 0.05);
	glVertex2f(x - 0.5, y + 0.15);
	glVertex2f(x - 0.5, y + 0.25);
	glVertex2f(x - 0.3, y + 0.45);
	glVertex2f(x - 0.1, y + 0.45);
	glVertex2f(x, y + 0.25);
	glVertex2f(x + 0.1, y + 0.45);
	glVertex2f(x + 0.3, y + 0.45);
	glVertex2f(x + 0.5, y + 0.25);
	glVertex2f(x + 0.5, y + 0.15);
	glVertex2f(x + 0.4, y + 0.05);
	glVertex2f(x + 0.1, y + 0.05);
	glEnd();

	//black eyes
	glColor3ub(0, 0, 0);
	DrawCircle(x - 0.25, y + 0.15, 0.15);
	DrawCircle(x + 0.25, y + 0.15, 0.15);

	//Mouth
	glPointSize(2.0);
	glBegin(GL_POINTS);
	glVertex2f(x - 0.45, y - 0.2);
	glVertex2f(x - 0.4, y - 0.3);
	glVertex2f(x - 0.3, y - 0.35);
	glVertex2f(x - 0.2, y - 0.4);
	glVertex2f(x - 0.1, y - 0.45);
	glVertex2f(x, y - 0.45);
	glVertex2f(x + 0.1, y - 0.45);
	glVertex2f(x + 0.2, y - 0.4);
	glVertex2f(x + 0.3, y - 0.35);
	glVertex2f(x + 0.4, y - 0.3);
	glVertex2f(x + 0.45, y - 0.2);
	glEnd();
#endif
}
void DrawSnackBody(float x, float y)
{
	x += 0.5;
	y += 0.5;

	glColor3ub(0, 120, 0);
	DrawCircle(x, y, 0.6);
}
void DrawGrid()
{
	int i;
	glColor3ub(200, 200, 200);

	glBegin(GL_LINES);
	for (i = 0; i < WIN_WIDTH; i++)
	{
		glVertex2f(i, 0);
		glVertex2f(i, WIN_HEIGHT);
	}
	for (i = 0; i < WIN_HEIGHT; i++)
	{
		glVertex2f(0, i);
		glVertex2f(WIN_WIDTH, i);
	}
	glEnd();
}

//-------------------------------------------------------------------------------

void NewFruit()
{
	int fruitX, fruitY;
	//enum FruitType fruit_type;
	int fruit_type;//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	srand(time(NULL));
	do {
		fruitX = rand() % WIN_WIDTH;
		fruitY = rand() % WIN_HEIGHT;
	} while (fruitX == snakeX[0] || fruitY == snakeY[0] || map[fruitY][fruitX] != ' ');
	fruit_type = rand() % 5;//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	switch (fruit_type)
	{
		case ORANGE:
			map[fruitY][fruitX] = 'o';
			break;
		case STRAWBERRY:
			map[fruitY][fruitX] = 's';
			break;
		case APPLE:
			map[fruitY][fruitX] = 'a';
			break;
		case BANANA:
			map[fruitY][fruitX] = 'b';
			break;
		case GRAPE:
			map[fruitY][fruitX] = 'g';
			break;
	}
	printf("%d (%d , %d)\n",fruit_type , fruitX, fruitY);
}
void ReadMap(char* map_name)
{
	int a, b;
	int i, j;
	FILE *mapp = NULL;
	char ch;
	mapp = fopen(map_name, "r");
	if (mapp == NULL)
	{
		printf("File open error!!\n");
		system("PAUSE");
		exit(1);
	}

	for (j = WIN_HEIGHT - 1; j >= 0; j -= 1)
	{
		for (i = 0; i < WIN_WIDTH; i += 1) 
		{
			if ((ch = fgetc(mapp)) != EOF) 
			{
				map[j][i] = ch;
			}
		}
		ch = fgetc(mapp);//change line		
	}
	fclose(mapp);
	for (i = 0; i < 5; i++)
	{
		NewFruit();
	}
	for (b = 0; b < WIN_HEIGHT; b++) 
	{
		for (a = 0; a < WIN_WIDTH; a++) 
		{
			printf("%c", map[b][a]);
		}
		printf("\n");
	}
}
void GameOver()
{
	gameover = 1;
	ReadMap("map2.txt");
	printf("score = %d", score);
}
void NewGame()
{
	snakeX[0] = 10;
	snakeY[0] = 10;
	snake_length = 0;// only body length
	head_dir = RIGHT;
	score = 0;
	gameover = 0;
	pause = 0;
	REFRESH_TIMER = 200;
	ReadMap("map.txt");
}
void CollisionDetect()
{
	int i;
	if (pause == 0)
	{
		for (i = 2; i < snake_length; i++)
			if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
				GameOver();
		switch (map[snakeY[0]][snakeX[0]])
		{
		case '+':
			GameOver();
			break;
		case 'G':
		case ' ':
			break;
		case 'o':
		case 's':
		case 'a':
		case 'b':
		case 'g':
			score += 1;
			map[snakeY[0]][snakeX[0]] = ' ';
			NewFruit();
			snakeX[snake_length + 1] = snakeX[snake_length];
			snakeY[snake_length + 1] = snakeY[snake_length];
			snake_length++;
			//REFRESH_TIMER *= 0.8; //add speed
			break;
		}
	}
}

void myinit()
{
	/* attributes */
	glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */

									  /* set up viewing */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, WIN_WIDTH*1.0, 0.0, WIN_HEIGHT*1.0);
	glMatrixMode(GL_MODELVIEW);
}
void display()
{
	int i, j, Three, Two, Num;
	char ch;
	FILE *mapp = NULL;
	glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */
	DrawGrid();
	//#####################
	//DrawGrape(8, 8);
	//DrawStrawberry(3, 5);
	//DrawBanana(8, 8);
	//DrawOrange(3, 3);
	//DrawApple(5, 3);
	//####################

	if (gameover == 0)
	{ 
		for (j = 0; j < WIN_HEIGHT; j++)
		{
			for (i = 0; i < WIN_WIDTH; i++)
			{
				switch (map[j][i])
				{
				case '+':
					DrawBrick(i, j, 1);
					break;
				case 'G':
					DrawGrass(i, j);
					break;
				}
			}
		}
		for (j = 0; j < WIN_HEIGHT; j++)
		{
			for (i = 0; i < WIN_WIDTH; i++)
			{
				switch (map[j][i])
				{
				case 'o':
					DrawOrange(i, j);
					break;
				case 's':
					DrawStrawberry(i, j);
					break;
				case 'a':
					//DrawApple(i, j);
					DrawOrange(i, j);
					break;
				case 'b':
					//DrawBanana(i, j);
					DrawOrange(i, j);
					break;
				case 'g':
					DrawGrape(i, j);
					break;
				}
			}
		}
		for (i = 1; i <= snake_length; i++)
			DrawSnackBody(snakeX[i], snakeY[i]);
		DrawSnackHead(snakeX[0], snakeY[0], head_dir);
	}
	if (gameover)
	{
		//glutChangeToMenuEntry(1, "Restart", 3);
		//glutAttachMenu(GLUT_RIGHT_BUTTON);
		for (j = 0; j < WIN_HEIGHT; j += 1)
			for (i = 0; i < WIN_WIDTH; i += 1)
				if (map[j][i] == '+')
					DrawBrick(i, j, 1);
		Three = score / 100;
		Two = (score % 100) / 10;

		if (Three == 0)	mapp = fopen("0.txt", "r");
		else if (Three == 1)	mapp = fopen("1.txt", "r");
		else if (Three == 2)	mapp = fopen("2.txt", "r");
		else if (Three == 3)	mapp = fopen("3.txt", "r");
		else if (Three == 4)	mapp = fopen("4.txt", "r");
		else if (Three == 5)	mapp = fopen("5.txt", "r");
		else if (Three == 6)	mapp = fopen("6.txt", "r");
		else if (Three == 7)	mapp = fopen("7.txt", "r");
		else if (Three == 8)	mapp = fopen("8.txt", "r");
		else if (Three == 9)	mapp = fopen("9.txt", "r");
		if (mapp == NULL)
		{
			printf("File open error!!\n");
			system("PAUSE");
			exit(1);
		}
		for (j = 4; j >= 0; j -= 1.0) {
			for (i = 0; i < 4; i += 1.0) {
				if ((ch = fgetc(mapp)) != EOF) {
					if (ch == '+')
						DrawBrick(i + 28.0, j + 2.0, 2);
				}
			}
			ch = fgetc(mapp);//change line
		}
		fclose(mapp);
		if (Two == 0)	mapp = fopen("0.txt", "r");
		else if (Two == 1)	mapp = fopen("1.txt", "r");
		else if (Two == 2)	mapp = fopen("2.txt", "r");
		else if (Two == 3)	mapp = fopen("3.txt", "r");
		else if (Two == 4)	mapp = fopen("4.txt", "r");
		else if (Two == 5)	mapp = fopen("5.txt", "r");
		else if (Two == 6)	mapp = fopen("6.txt", "r");
		else if (Two == 7)	mapp = fopen("7.txt", "r");
		else if (Two == 8)	mapp = fopen("8.txt", "r");
		else if (Two == 9)	mapp = fopen("9.txt", "r");
		if (mapp == NULL)
		{
			printf("File open error!!\n");
			system("PAUSE");
			exit(1);
		}
		for (j = 4; j >= 0; j -= 1.0) {
			for (i = 0; i < 4; i += 1.0) {
				if ((ch = fgetc(mapp)) != EOF) {
					if (ch == '+')
						DrawBrick(i + 33.0, j + 2.0, 2);
				}
			}
			ch = fgetc(mapp);//change line
		}
		fclose(mapp);
		mapp = fopen("0.txt", "r");
		if (mapp == NULL)
		{
			printf("File open error!!\n");
			system("PAUSE");
			exit(1);
		}
		for (j = 4; j >= 0; j -= 1.0) {
			for (i = 0; i < 4; i += 1.0) {
				if ((ch = fgetc(mapp)) != EOF) {
					if (ch == '+')
						DrawBrick(i + 38.0, j + 2.0, 2);
				}
			}
			ch = fgetc(mapp);//change line
		}
		fclose(mapp);
	}
	glFlush(); /* clear buffers */
	glutSwapBuffers();
}
void TimerFunction(int value)
{
	if (pause == 0 && gameover == 0)
	{
		int i;
		for (i = snake_length; i > 0; i--)
		{
			snakeX[i] = snakeX[i - 1];
			snakeY[i] = snakeY[i - 1];
		}
		switch (head_dir)
		{
		case UP:
			snakeY[0]++;
			break;
		case DOWN:
			snakeY[0]--;
			break;
		case LEFT:
			snakeX[0]--;
			break;
		case RIGHT:
			snakeX[0]++;
			break;
		}
		CollisionDetect();
	}
	glutPostRedisplay();
	glutTimerFunc(REFRESH_TIMER, TimerFunction, 1);

}
void SpecKeyboard(int key, int x, int y)
{
	if (gameover == 0)
	{
		if (pause == 0) 
		{
			switch (key) {
			case GLUT_KEY_DOWN:
				if (snake_length == 0 || head_dir != UP)
					head_dir = DOWN;
				break;
			case GLUT_KEY_UP:
				if (snake_length == 0 || head_dir != DOWN)
					head_dir = UP;
				break;
			case GLUT_KEY_LEFT:
				if (snake_length == 0 || head_dir != RIGHT)
					head_dir = LEFT;
				break;
			case GLUT_KEY_RIGHT:
				if (snake_length == 0 || head_dir != LEFT)
					head_dir = RIGHT;
				break;
			}
		}		
		switch (key) 
		{
			case GLUT_KEY_PAGE_UP:
				pause = 1;
				printf("\nGLUT_KEY_PAGE_UP PAUSE = %d\n", pause);
				break;
			case GLUT_KEY_PAGE_DOWN:
				pause = 0;
				printf("\nGLUT_KEY_PAGE_DOWN PAUSE = %d\n", pause);
				break;
		}
	}
}
void Mouse(int btn, int state, int x, int y)
{
#if 0
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{ 
		pause = 1; 
		printf("\nPAUSE@ = %d\n", pause); 
	}
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		pause = 0;
		printf("\nPAUSE@ = %d\n", pause);
	}
#endif
}
void MyMenu(int id)
{	
	pause = 1;
	printf("\nMyMenu start\nPAUSE@@@@ = %d\n", pause);
	switch (id)
	{
		case 1:
			break;
		case 2:
			exit(1);
			break;
		case 3:
			NewGame();
			break;
	}
	pause = 0;
	printf("\nMyMenu end\nPAUSE@@@@ = %d\n", pause);
}
int main(int argc, char** argv)
{
	snakeX[0] = 10;
	snakeY[0] = 10;
	/* standard GLUT initialization */
	ReadMap("map.txt");
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); /* default, not needed */
	glutInitWindowSize(960, 520); /* 960 x 520 pixel window */
	glutInitWindowPosition(300, 100); /* place window top left on display */
	glutCreateWindow("Greedy Snake"); /* window title */
	glutDisplayFunc(display); /* display callback invoked when window opened */

	glutMouseFunc(Mouse);

	glutCreateMenu(MyMenu);
	glutAddMenuEntry("Go on", 1);
	glutAddMenuEntry("Esc", 2);
	glutAddMenuEntry("Rstart", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutTimerFunc(REFRESH_TIMER, TimerFunction, 1);

	glutSpecialFunc(SpecKeyboard);

	myinit(); /* set attributes */
	glutMainLoop(); /* enter event loop */
}
