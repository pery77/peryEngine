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
	std::cout << tilesList.size();
}

Tile* TilesetManager::GetTile(int index)
{
	return tilesList[index];
}
