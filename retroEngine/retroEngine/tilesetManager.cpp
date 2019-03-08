#include "tilesetManager.h"

TilesetManager::TilesetManager()
{
}

TilesetManager::~TilesetManager()
{

}

void TilesetManager::AddTileset(Texture2D  texture)
{
	tilesetsList.push_back(texture);
}

Texture2D  TilesetManager::GetTileset(int index)
{
	return tilesetsList[index];
}

void TilesetManager::AddTile(Tile tile)
{
	tilesList.push_back(tile);
}

Tile TilesetManager::GetTile(int index)
{
	return tilesList[index];
}
