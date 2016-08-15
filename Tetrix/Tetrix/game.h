const int ROWS = 22;
const int COLS = 12;

#include "shape.h"

struct Tile {
	bool filled;
	bool isMe;
	float red;
	float green;
	float blue;
};

class Game {
public:
	Game(void (*pt2Func)());
	void ClearGrid();
	void makeNextBrick();
	void Restart();
	void MoveX(int);
	bool MoveY();
	void rotateShape(int);
	bool CantRotate(int);
	bool CheckCollision(int);
	Tile grid[COLS][ROWS];
	bool killed;	// true if the current life has been terminated
	bool paused;	// true if the game is to be paused
	int timer;		// determines how fast game proceeds
	int lines;
	int level;
	int score;
	Shape current;
	Shape next;
private:
	bool checkLost();
	void checkForRows();
	void clearIsMe();
	void clearCurrent();
	void moveCurrent();
	void (*reDraw)(); 
};
