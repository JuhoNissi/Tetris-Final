const int NumPoses = 4;		// N, S, E, W
const int NumBrickTypes = 7;	// 2x2, 1x4, "T", ...

class Shape {
public:
	Shape();
	Shape(int);
	void setColor(float, float, float);
	void rotate(int);
	void setLocation(int,int);
	const int* transforms();
	int posX;
	int posY;
	float red;
	float green;
	float blue;
private:
	int type;
	int rotation;
};

// The following is an array of the transformations of 
// different bricks that are available. It assumes top
// left is 0,0 and that the brick is anchored there.
const int brickTransforms[NumBrickTypes][NumPoses][8] =
{
	{
		{0, 0, 1, 0, 0, 1, 1, 1},// XX
		{0, 0, 1, 0, 0, 1, 1, 1},// XX
		{0, 0, 1, 0, 0, 1, 1, 1},
		{0, 0, 1, 0, 0, 1, 1, 1},
	},
	{
		{0, 0, 0, 1, 0, 2, 0, 3},// XXXX
		{0, 0, 1, 0, 2, 0, 3, 0},
		{0, 0, 0, 1, 0, 2, 0, 3},
		{0, 0, 1, 0, 2, 0, 3, 0},
	},
	{
		{0, 0, 0, 1, 1, 1, 0, 2},//  X
		{1, 0, 0, 1, 1, 1, 2, 1},// XXX
		{0, 1, 1, 0, 1, 1, 1, 2},
		{0, 0, 1, 0, 2, 0, 1, 1}
	},
	{	{0, 0, 1, 0, 0, 1, 0, 2},// X
		{0, 0, 0, 1, 1, 1, 2, 1},// XXX
		{1, 0, 1, 1, 0, 2, 1, 2},
		{0, 0, 1, 0, 2, 0, 2, 1}
	},
	{	{0, 0, 1, 0, 1, 1, 1, 2},// XXX
		{0, 0, 1, 0, 2, 0, 0, 1},// X
		{0, 0, 0, 1, 0, 2, 1, 2},
		{2, 0, 0, 1, 1, 1, 2, 1}
	},
	{	{0, 0, 0, 1, 1, 1, 1, 2},//  XX
		{1, 0, 2, 0, 0, 1, 1, 1},// XX
		{0, 0, 0, 1, 1, 1, 1, 2},
		{1, 0, 2, 0, 0, 1, 1, 1}
	},
	{	{1, 0, 0, 1, 1, 1, 0, 2},// XX
		{0, 0, 1, 0, 1, 1, 2, 1},//  XX
		{1, 0, 0, 1, 1, 1, 0, 2},
		{0, 0, 1, 0, 1, 1, 2, 1}
	}
};

