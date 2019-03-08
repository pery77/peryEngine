#include "tilesetManager.h"

TilesetManager::TilesetManager()
{
}

TilesetManager::~TilesetManager()
{

}

void TilesetManager::AddTilesetTexture(Texture2D & texture)
{
	textureList.push_back(texture);
}

Texture2D & TilesetManager::GetTilesetTexture(int index)
{
	return textureList[index];
}

void TilesetManager::AddTile(Tile & tile)
{
	tilesList.push_back(tile);
}

Tile & TilesetManager::GetTile(int index)
{
	return tilesList[index];
}
