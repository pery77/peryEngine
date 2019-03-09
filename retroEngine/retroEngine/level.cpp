#include "level.h"

Level::Level(int width, int height)
{
	SetDimensions(width, height);
	this->width  = width;
	this->height = height;
}

Level::~Level()
{
}

void Level::AddTile(int x, int y, Tile * tile)
{
	map[x][y] = tile;
}

Tile * Level::GetTile(int x, int y)
{
	return map[x][y];
}

void Level::LoadLevel()
{
}

int Level::GetWidth()
{
	return width;
}

int Level::GetHeight()
{
	return height;
}

void Level::SetDimensions(int width, int height)
{
	//width are rows
	map.resize(width);

	for (int i = 0; i < width; i++)
	{
		map.at(i).resize(height, 0);
	}
}
