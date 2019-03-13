#include "camera2D.h"


Cam::Cam(int w, int h, float speed, int levelWidth, int levelHeight)
{
	size.x = w;
	size.y = h;

	if (speed < 0.0)
		speed = 0.0;
	if (speed > 1.0)
		speed = 1.0;

	this->speed = speed;

	Move(0, 0);

	stopX = ((levelWidth  * TILESIZE) - w);
	stopY = ((levelHeight * TILESIZE) - h);
}

Cam::~Cam()
{
}

void Cam::Move(int x, int y)
{
	position.x = (float)x;
	position.y = (float)y;
	target.x = (float)x;
	target.y = (float)y;
}

void Cam::MoveCenter(int x, int y)
{
}

void Cam::GoTo(int x, int y)
{
	target.x = (float)x;
	target.y = (float)y;
}

void Cam::GoToCenter(int x, int y)
{
	x = x - (size.x / 2);
	y = y - (size.y / 2);

	target.x = (float)x;
	target.y = (float)y;
}

void Cam::Update()
{
	//X distance to target, Y distance to target, and Euclidean distance
	float x, y, d;

	//Velocity magnitudes
	float vx, vy, v;

	//Find x and y
	x = (float)(target.x - position.x);
	y = (float)(target.y - position.y);

	//If we're within 1 pixel of the target already, just snap
	//to target and stay there. Otherwise, continue
	if ((x*x + y * y) <= 1)
	{
		position.x = target.x;
		position.y = target.y;
	}
	else
	{
		//Distance formula
		d = sqrt((x*x + y * y));

		//We set our velocity to move 1/60th of the distance to
		//the target. 60 is arbitrary, I picked it because I intend
		//to run this function once every 60th of a second. We also
		//allow the user to change the camera speed via the speed member
		v = (d * speed) / 60;

		//Keep v above 1 pixel per update, otherwise it may never get to
		//the target. v is an absolute value thanks to the squaring of x
		//and y earlier
		if (v < 1.0f)
			v = 1.0f;

		//Similar triangles to get vx and vy
		vx = x * (v / d);
		vy = y * (v / d);

		//Then update camera's position and we're done
		position.x += vx;
		position.y += vy;

		if (position.x < 0) position.x = 0;
		if (position.x > stopX) position.x = stopX;
		if (position.y < 0) position.y = 0;
		if (position.y > stopY) position.y = stopY;
	}
}

Rectangle Cam::GetTileBounds()
{

	int x = (int)(position.x / TILESIZE);
	int y = (int)(position.y / TILESIZE);

	//+1 in case camera size isn't divisible by tileSize
	//And +1 again because these values start at 0, and
	//we want them to start at one
	int w = (int)(size.x / TILESIZE + 2);
	int h = (int)(size.y / TILESIZE + 2);

	//And +1 again if we're offset from the tile
	if (x % TILESIZE != 0)
		w++;
	if (y % TILESIZE != 0)
		h++;

	return { (float)x, (float)y, (float)w, (float)h };
}

