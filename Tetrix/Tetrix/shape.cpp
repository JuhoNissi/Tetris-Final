#include <cstdlib>
#include <cstdio>
using namespace std;

#include "shape.h"

void Shape::setColor(float r, float g, float b){
	red = r;
	green = g;
	blue = b;
}

Shape::Shape(int t)
{
	type = t;
	switch (type) {
		case 0:		setColor(1.0, 0.0, 0.0); break;  //red
		case 1:		setColor(0.0, 0.0, 1.0); break;  //blue
		case 2:		setColor(0.0, 1.0, 0.0); break; //green
		case 3: 	setColor(0.0, 1.0, 1.0); break; //cyan
		case 4:		setColor(1.0, 0.0, 1.0); break; //magenta
		case 5:		setColor(1.0, 1.0, 0.0); break; //yellow
		case 6:		setColor(0.5, 0.5, 0.5); break; //grey
		default:	setColor(0.0, 0.0, 0.0); break; //black
	}
	rotation = 0;
}

Shape::Shape(){
   Shape(1);
}

void Shape::setLocation(int x, int y){
	posX = x;
	posY = y;
}

void Shape::rotate(int dir){
	rotation += dir;
	if(rotation > 3)
		rotation = 0;
	else if(rotation < 0)
		rotation = 3;
}

const int* Shape::transforms(){
	return brickTransforms[type][rotation];
}
