#include "tilesetManager.h"
#include <iostream>

TilesetManager::TilesetManager()
{
	std::cout << "TileManager Created" << std::endl;

}

TilesetManager::~TilesetManager()
{
}

void TilesetManager::AddTileset(Texture2D & texture)
{
	tilesetsList.push_back(texture);
}

Texture2D & TilesetManager::GetTileset(int index)
{
	return tilesetsList[index];
}

void TilesetManager::AddTile(Tile * tile)
{
	tilesList.push_back(tile);
}

Tile* TilesetManager::GetTile(int index)
{
	return tilesList[index];
}

void TilesetManager::MakeTiles()
{
	Tile* tile;
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			tile = new Tile(GetTileset(0), x, y);
			AddTile(tile);
		}
	}
}
