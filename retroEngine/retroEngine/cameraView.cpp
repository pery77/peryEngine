#include "cameraView.h"

pery::CameraView::CameraView(int w, int h, float speed, int levelWidth, int levelHeight, int tileSize)
{
	//Set size
	size.x = w;
	size.y = h;
	/*
	//Clamp speed
	if (speed < 0.0)
		speed = 0.0;
	if (speed > 1.0)
		speed = 1.0;
	*/
	this->speed = speed;

	//Move camera to 0,0
	Move(0, 0);

	TileSize = tileSize;

	//Store variables for stop camera on borders.
	stopX = ((levelWidth  * TileSize) - w);
	stopY = ((levelHeight * TileSize) - h);
}

pery::CameraView::~CameraView()
{
}
//Move camera to x, and y
void pery::CameraView::Move(int x, int y)
{
	position.x = (float)x;
	position.y = (float)y;
	target.x = (float)x;
	target.y = (float)y;
}
//Move camera with speed
void pery::CameraView::GoTo(int x, int y)
{
	target.x = (float)x;
	target.y = (float)y;
}
//Update camera
void pery::CameraView::Update()
{
	//X distance to target, Y distance to target, and Euclidean distance
	float x, y, d;

	//Velocity magnitudes
	float vx, vy, v;

	//Find x and y
	x = (float)(target.x  - position.x);
	y = (float)(target.y  - position.y);

	//If we're within 1 pixel of the target already, just snap
	//to target and stay there. Otherwise, continue
	if ((x*x + y*y) <= 1)
	{
		position.x = target.x;
		position.y = target.y;
	}
	else
	{
		//Distance formula
		d = sqrt((x*x + y*y));

		//Calculate speed (60 FPS)
		v = (d * speed) / 60;

		//Keep v above 1 pixel per update, otherwise it may never get to
		//the target. v is an absolute value thanks to the squaring of x
		//and y earlier
		if (v < 1.0f) v = 1.0f;

		//Similar triangles to get vx and vy
		vx = x * (v / d);
		vy = y * (v / d);

		//Then update camera's position and we're done
		position.x += vx;
		position.y += vy;

		//Stop camera in borders
		if (position.x < 0) position.x = 0;
		if (position.x > stopX) position.x = stopX;
		if (position.y < 0) position.y = 0;
		if (position.y > stopY) position.y = stopY;
	}
}

Rectangle pery::CameraView::GetTileBounds()
{

	int x = (int)(position.x / TileSize);
	int y = (int)(position.y / TileSize);

	//+1 in case camera size isn't divisible by tileSize
	//And +1 again because these values start at 0, and
	//we want them to start at one
	int w = (int)(size.x / TileSize + 2);
	int h = (int)(size.y / TileSize + 2);

	//And +1 again if we're offset from the tile
	if (x % TileSize != 0)
		w++;
	if (y % TileSize != 0)
		h++;

	return { (float)x, (float)y, (float)w, (float)h };
}