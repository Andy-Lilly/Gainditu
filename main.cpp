#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


const int VK_LEFT = 25;
const int VK_UP = 26;
const int VK_RIGHT = 27;
const int VK_DOWN = 28;

const float BOX_SIZE = 1.0f;
const float screen_size = 10.0f;
float _angle = 0.0f;
float dist=0.0f;

int width_in_pixels;
int height_in_pixels;

float player_x = 0.0f;
float player_y = 0.0f;
float player_dx = 0.0f;
float player_dy = 0.0f;
float player_ddx = 0.0f;
float player_ddy = 0.0f;

float ball_x = 0.0f;
float ball_y = 0.0f;
float ball_dx = 0.0f;
float ball_dy = 0.0f;
float ball_ddx = 0.0f;
float ball_ddy = 0.0f;

bool has_possession;

const float max_speed = .5;
const float max_accel = .1;
const float min_ball_speed = .1;

void handleMouse(int button, int state, int x, int y)
{
	float x_prime = screen_size * ((float) x)/((float) width_in_pixels);
	float y_prime = screen_size * ((float) y)/((float) height_in_pixels);
	
	ball_dx = x_prime - player_x;
	ball_dy = y_prime - player_y;
	
	printf("here\n");
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
			break;
		case 'j': player_ddx -= max_accel; break;
		case 'l': player_ddx += max_accel; break;
		case 'k': player_ddy -= max_accel; break;
		case 'i': player_ddy += max_accel; break;
		case 'b': ball_dx += ((float) (rand()%200 - 100))/20.0; 
			ball_dy += ((float) (rand()%200 - 100))/20.0;
			has_possession = false;
			break;
	}
}

void handleKeyrelease(unsigned char key, int x, int y) {
	switch (key) {
		case 'j': player_ddx = 0.0; break;
		case 'l': player_ddx = 0.0; break;
		case 'k': player_ddy = 0.0; break;
		case 'i': player_ddy = 0.0; break;
	}
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
}

void handleResize(int w, int h) {
	width_in_pixels = w;
	height_in_pixels = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-screen_size, screen_size, -screen_size, screen_size, -screen_size, screen_size);
}

void recomputePlayerValues() {
	if(player_ddx == 0.0 && player_dx > 0.0) {
		player_dx -= max_accel;
		if(player_dx < 0.0) {
			player_dx = 0.0;
		}
	}
	if(player_ddx == 0.0 && player_dx < 0.0) {
		player_dx += max_accel;
		if(player_dx > 0.0) {
			player_dx = 0.0;
		}
	}
	
	if(player_ddy == 0.0 && player_dy > 0.0) {
		player_dy -= max_accel;
		if(player_dy < 0.0) {
			player_dy = 0.0;
		}
	}
	
	if(player_ddy == 0.0 && player_dy < 0.0) {
		player_dy += max_accel;
		if(player_dy > 0.0) {
			player_dy = 0.0;
		}
	}
	
	player_dx += player_ddx;
	player_dy += player_ddy;
	
	player_x += player_dx;
	player_y += player_dy;
	
	if(player_x <= -BOX_SIZE * screen_size) {
		player_x = -BOX_SIZE * screen_size;
		player_dx = 0;
		player_ddx = 0;
	}
	
	if(player_x + BOX_SIZE >= BOX_SIZE * screen_size) {
		player_x = BOX_SIZE * (screen_size-1.0);
		player_dx = 0;
		player_ddx = 0;
	}
	
	if(player_y <= -BOX_SIZE * screen_size) {
		player_y = -BOX_SIZE * screen_size;
		player_dy = 0;
		player_ddy = 0;
	}
	
	if(player_y + BOX_SIZE >= BOX_SIZE * screen_size) {
		player_y = BOX_SIZE * (screen_size-1.0);
		player_dy = 0;
		player_ddy = 0;
	}
}

void recomputeBallValues() {
	if(ball_dx > 0.0) {
		ball_ddx = -0.1 * ball_dx * ball_dx;
	}
	
	if(ball_dx < 0.0) {
		ball_ddx = 0.1 * ball_dx * ball_dx;
	}
	
	if(ball_dy > 0.0) {
		ball_ddy = -0.1 * ball_dy * ball_dy;
	}
	
	if(ball_dy < 0.0) {
		ball_ddy = 0.1 * ball_dy * ball_dy;
	}
	
	
	ball_dx += ball_ddx;
	ball_dy += ball_ddy;
	
	if(ball_dx * ball_dx + ball_dy * ball_dy < min_ball_speed * min_ball_speed)
	{
		ball_dx = 0;
		ball_dy = 0;
		
	}

	ball_x += ball_dx;
	ball_y += ball_dy;
	
	if(ball_x <= -BOX_SIZE * screen_size) {
		ball_x = -BOX_SIZE * screen_size;
		ball_dx *= -.3;
		ball_dy *= .3;
		ball_ddx = -ball_ddx;
	}
	
	if(ball_x + BOX_SIZE/2 >= BOX_SIZE * screen_size) {
		ball_x = BOX_SIZE * (screen_size-0.5);
		ball_dx *= -.3;
		ball_dy *= .3;
		ball_ddx = -ball_ddx;
	}
	
	if(ball_y <= -BOX_SIZE * screen_size) {
		ball_y = -BOX_SIZE * screen_size;
		ball_dx *= .3;
		ball_dy *= -.3;
		ball_ddy = -ball_dy;
	}
	if(ball_y + BOX_SIZE/2 >= BOX_SIZE * screen_size) {
		ball_y = BOX_SIZE * (screen_size-0.5);
		ball_dx *= .3;		
		ball_dy *= -.3;
		ball_ddy = -ball_ddy;
	}
	
	if(has_possession || (ball_y <= player_y + BOX_SIZE && ball_y >= player_y && ball_x <= player_x + BOX_SIZE && ball_x >= player_x))
	{
		ball_x = player_x + BOX_SIZE/4;
		ball_y = player_y + BOX_SIZE/4;
		ball_dx = ball_dy = ball_ddx = ball_ddy = 0.0f;
		has_possession = true;
	}
}


void recomputeValues() {
	recomputePlayerValues();
	recomputeBallValues();
}

void drawScene() {
	
	recomputeValues();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//glTranslatef(0.0f, 0.0f, -20.0f);
	
	glBegin(GL_QUADS);
	
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-BOX_SIZE * screen_size, -BOX_SIZE  * screen_size, BOX_SIZE / 2 + dist);
	glVertex3f(-BOX_SIZE * screen_size, BOX_SIZE  * screen_size, BOX_SIZE / 2 + dist);
	glVertex3f(BOX_SIZE * screen_size, BOX_SIZE  * screen_size, BOX_SIZE / 2 + dist);
	glVertex3f(BOX_SIZE * screen_size, -BOX_SIZE  * screen_size, BOX_SIZE / 2 + dist);
	
	glColor3f(1.0f,1.0f,0.0f);
	glVertex3f(player_x, player_y, BOX_SIZE);
	glVertex3f(player_x, BOX_SIZE+player_y, BOX_SIZE);
	glVertex3f(BOX_SIZE+player_x, BOX_SIZE+player_y, BOX_SIZE);
	glVertex3f(BOX_SIZE+player_x, player_y, BOX_SIZE);
	
	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(ball_x, ball_y, BOX_SIZE*2);
	glVertex3f(ball_x, BOX_SIZE/2+ball_y, BOX_SIZE*2);
	glVertex3f(BOX_SIZE/2+ball_x, BOX_SIZE/2+ball_y, BOX_SIZE*2);
	glVertex3f(BOX_SIZE/2+ball_x, ball_y, BOX_SIZE*2);
	
	
	glEnd();
	
	glutSwapBuffers();
}

//Called every 25 milliseconds
void update(int value) {
	glutPostRedisplay();
	glutTimerFunc(25,update,0);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	
	glutCreateWindow("Putting It All Together - videotutorialsrock.com");
	initRendering();
	
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutKeyboardUpFunc(handleKeyrelease);
	glutMouseFunc(handleMouse);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);
	
	glutMainLoop();
	return 0;
}








