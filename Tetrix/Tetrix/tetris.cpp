#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

#include <time.h>
#include <GL/glut.h>
#include "game.h"

const int BLOCKSIZE = 30;
const int VPWIDTH = COLS * BLOCKSIZE;
const int VPHEIGHT = ROWS * BLOCKSIZE;

// Structure to maintain the state of a game

Game game = NULL;		// THE game

void BitmapText(char *str, int wcx, int wcy)
{
	glRasterPos2i(wcx, wcy);
	for (int i=0; str[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	}
}

void dropTimer(int id){
	if(game.MoveY())
		glutTimerFunc(10, dropTimer, 0);
	glutPostRedisplay();
}

// timer - Callback for timers

void timer(int id)
{
	if (game.killed) {	// mode for when the user has been killed
		if (id < 10)
			glutTimerFunc(200, timer, id+1);
		else {
			game.killed = false;
			game.Restart();
			glutTimerFunc(game.timer, timer, 0);
		}
		glutPostRedisplay();
	} else if (!game.paused) {	// normal operation is when game
		game.MoveY();		// is not paused
		if (game.killed) {
			glutTimerFunc(10, timer, 1);
		} else {
			glutPostRedisplay();
			glutTimerFunc(game.timer, timer, 0);
		}
	}
}

// keyboard - Callback for handling user's keyboard actions.

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'p') {
		game.paused = !game.paused;
		if (!game.paused)
			glutTimerFunc(game.timer, timer, 0);
		glutPostRedisplay();
	} else if (!game.paused && !game.killed && key == ' ') {
		game.score += ROWS - game.current.posY;
		glutTimerFunc(10, dropTimer, 0);
	}
}

// keyboard - Callback for handling user's arrow keys.

void special(int key, int x, int y)
{
	if (!game.paused && !game.killed) {
		if (key == GLUT_KEY_LEFT) {
			game.MoveX(-1);
			glutPostRedisplay();
		} else if (key == GLUT_KEY_RIGHT) {
			game.MoveX(1);
			glutPostRedisplay();
		}else if (key == GLUT_KEY_UP) {
			game.rotateShape(1);
			glutPostRedisplay();
		}else if (key == GLUT_KEY_DOWN) {
			game.rotateShape(-1);
			glutPostRedisplay();
		}
	}
}

// display - Callback for displaying the grid and statistics.

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, VPWIDTH, VPHEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, COLS, ROWS, 0);

	if(!game.paused)
		for (int r=0; r<ROWS; r++) {
			for (int c=0; c<COLS; c++) {
				Tile &tile = game.grid[c][r];
				if (tile.filled){
					glColor3f(tile.red, tile.green, tile.blue);
					glRectd(c, r, c+1, r+1);
				}
			}
		}

// draw next shape on right upper screen

	glViewport(VPWIDTH, 0, VPWIDTH , VPHEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, COLS, ROWS, 0);

	Shape &nxt = game.next;
	glColor3d(nxt.red, nxt.green, nxt.blue);
	const int* trans = nxt.transforms();
	if(!game.paused)
		for(int x = 0; x < 8; x += 2)
			glRectd(2 + trans[x], 4 + trans[x + 1] - 1, 2 + trans[x] +1, 4 + trans[x + 1]);

	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(0, VPHEIGHT / 2);
	glEnd();

// Write messages on right hand lower side of screen

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, VPHEIGHT);
	glColor3f(1, 1, 0);
	char msg[100];
	if(game.paused)
		BitmapText("Game Paused", 10, 250);
	if(game.killed)
		BitmapText("Game Over", 10, 250);
	sprintf_s(msg, "Lines: %d", game.lines);
	BitmapText(msg, 10, 200);
	sprintf_s(msg, "Level: %d", game.level);
	BitmapText(msg, 10, 180);
	sprintf_s(msg, "Score: %d", game.score);
	BitmapText(msg, 10, 140);

	glutSwapBuffers();
}

void Sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

#ifndef WIN32
void Sleep(unsigned int mseconds)
{
	clock_t goal = mseconds + clock();
	while (goal > clock());
}
#endif

void reDraw() {
	display();
#ifndef WIN32
	Sleep(50000);
#endif
}

int main(int argc, char *argv[])
{	
	cout << "p to pause" << endl;
	cout << "space to drop" << endl;
	cout << "left and right arrows to move sideways" << endl;
	cout << "up and down arrows to rotate" << endl;
	game = Game(reDraw);

	srand((unsigned)time(0)); 
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(VPWIDTH + 200, VPHEIGHT);

	glutCreateWindow("Tetris MataFakaClone");

	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(game.timer, timer, 0);

	glutMainLoop();

	return 0;
}
