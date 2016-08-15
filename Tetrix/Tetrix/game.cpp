#include <cstdlib>
#include <cstdio>
#include <iostream>
using namespace std;

#include "game.h"

// random - return a random number in the interval [0,1]
int randBrick()
{ 
	return rand() % NumBrickTypes;
}

void Game::makeNextBrick(){
	current = next;
	next = Shape(randBrick());
	next.setLocation(COLS / 2, 0);
}

void Game::clearIsMe(){
	const int* trans = current.transforms();
	for(int x = 0; x < 8; x += 2){
		Tile &tile = grid[current.posX + trans[x]][current.posY + trans[x + 1]];
		tile.isMe = false;
	}
}

void Game::clearCurrent(){
	const int* trans = current.transforms();
	for(int x = 0; x < 8; x += 2){
		Tile &tile = grid[current.posX + trans[x]][current.posY + trans[x + 1]];
		tile.filled = false;
		tile.isMe = false;
	}
}

void Game::moveCurrent(){
	const int* trans = current.transforms();
	for(int x = 0; x < 8; x += 2){
		Tile &tile = grid[current.posX + trans[x]][current.posY + trans[x + 1]];
		tile.filled = true;
		tile.isMe = true;
		tile.red = current.red;
		tile.green = current.green;
		tile.blue = current.blue;
	}	
}

bool Game::checkLost(){
	const int* trans = current.transforms();
	for(int x = 0; x < 8; x += 2)
		if(grid[current.posX + trans[x]][current.posY + trans[x + 1]].filled){
			killed = true;
			return true;
		}
	return false;	
}

// Game - Constructor

Game::Game(void (*pt2Func)())
{
	reDraw = pt2Func;
	Restart();
}

// Restart() - clear grid 

void Game::Restart()
{
	timer = 500;
	score = 0;
	lines = 0;
	level = 0;
	ClearGrid();
	killed = false;
	paused = false;
	current = Shape(randBrick());
	current.setLocation(COLS / 2, 0);
	moveCurrent();
	next = Shape(randBrick());
	next.setLocation(COLS / 2, 0);
}

void Game::rotateShape(int dir){
	if(CantRotate(dir))
		return;
	clearCurrent();
	current.rotate(dir);
	moveCurrent();
}

void Game::MoveX(int dir){

	if(CheckCollision(dir))
		return;
	clearCurrent();
	current.posX += dir;
	moveCurrent();
}

void Game::checkForRows(){
	for (int r=0; r<ROWS; r++) {
		bool rowFull = true;
		for (int c=0; c<COLS; c++) {
			if(!grid[c][r].filled){
				rowFull = false;
				break;
			}
		}
		if(rowFull){
			lines++;
			if((lines % 10) == 0)
				level++;
			if(level > 0)
				timer = 500 / level;
			for (int c=0; c<COLS; c++) {
				grid[c][r].filled = false;
				reDraw();
				for (int x=r; x >= 0; x--){
					if(grid[c][x].filled){
						grid[c][x].filled = false;
						grid[c][x+1].filled = true;
						grid[c][x+1].red = grid[c][x].red;
						grid[c][x+1].green = grid[c][x].green;
						grid[c][x+1].blue = grid[c][x].blue;
						reDraw();
					}
				}
			}
		}
	}
}

bool Game::MoveY()
{
	if(CheckCollision(0)){// cant go down anymore, so lock it in place and go to next piece
		checkForRows();
		clearIsMe();
		makeNextBrick();
		if(checkLost())
			return false;
		moveCurrent();
		return false;
	}
	clearCurrent();
	current.posY++;
	moveCurrent();
	return true;
}

// CanRotate - Determines if blocks will collided if the current one is rotated
bool Game::CantRotate(int dir)
{
	current.rotate(dir);
	const int* trans = current.transforms();
	current.rotate(dir * -1);  //undo what we just did now that we have the transforms for rotation
	int newX, newY;
	for(int x = 0; x < 8; x += 2){
		newX = current.posX + trans[x];
		newY = current.posY + trans[x + 1];
		if(newX >= COLS || newX < 0 || newY >= ROWS || (grid[newX][newY].filled && !grid[newX][newY].isMe))  //checks if it would hit walls
			return true;
	}
	return false;
}

// CheckCollision - Determines if blocks will collided, -1 for left, 1 for right, 0 for down
bool Game::CheckCollision(int dir)
{
	const int* trans = current.transforms();
	int newX, newY;
	for(int x = 0; x < 8; x += 2){
		newX = current.posX + trans[x];
		newY = current.posY + trans[x + 1];
		if(dir == 0)
			newY += 1;
		else
			newX += dir;
		if(newX >= COLS || newX < 0 || newY >= ROWS || (grid[newX][newY].filled && !grid[newX][newY].isMe))  //checks if it would hit walls
			return true;
	}
	return false;
}

// ClearGrid - Erases all oncoming blocks from the grid.

void Game::ClearGrid()
{
	for (int r=0; r<ROWS; r++) {
		for (int c=0; c<COLS; c++) {
			grid[c][r].filled  = false;
			grid[c][r].isMe  = false;
		}
	}
}

