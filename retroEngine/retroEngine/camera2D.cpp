#include "camera2D.h"

Cam::Cam(int w, int h, float speed)
{

}

Cam::~Cam()
{
}

void Cam::Move(int x, int y)
{
}

void Cam::MoveCenter(int x, int y)
{
}

void Cam::GoTo(int x, int y)
{
}

void Cam::GoToCenter(int x, int y)
{
}

void Cam::Update()
{
}

Rectangle Cam::GetTileBounds()
{
	int tileSize = 16;
	int x = (int)(position.x / tileSize);
	int y = (int)(position.y / tileSize);

	//+1 in case camera size isn't divisible by tileSize
	//And +1 again because these values start at 0, and
	//we want them to start at one
	int w = (int)(size.x / tileSize + 2);
	int h = (int)(size.y / tileSize + 2);

	//And +1 again if we're offset from the tile
	if (x % tileSize != 0)
		w++;
	if (y % tileSize != 0)
		h++;

	return { (float)x, (float)y, (float)w, (float)h };
}

