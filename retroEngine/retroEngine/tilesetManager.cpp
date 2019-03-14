#include "tilesetManager.h"

pery::TilesetManager::TilesetManager()
{
	LOG("TileManager Created");
}

pery::TilesetManager::~TilesetManager()
{
	LOG("Unload TilesetManager");
	for (int i = 0; i < tilesList.size(); i++)
	{
		delete tilesList[i];
	}
	tilesList.clear();

	for (int i = 0; i < tilesetsList.size(); i++)
	{
		UnloadTexture(tilesetsList[i]);
	}
	tilesetsList.clear();
}

void pery::TilesetManager::AddTileset(Texture2D & texture)
{
	tilesetsList.push_back(texture);
}

Texture2D & pery::TilesetManager::GetTileset(int index)
{
	return tilesetsList[index];
}

void pery::TilesetManager::AddTile(Tile * tile)
{
	tilesList.push_back(tile);
}

pery::Tile* pery::TilesetManager::GetTile(int index)
{
	return tilesList[index];
}

void pery::TilesetManager::MakeTiles()
{
	Tile* tile;
	for (int y = 0; y < TILESETSIZE; y++)
	{
		for (int x = 0; x < TILESETSIZE; x++)
		{
			tile = new Tile(GetTileset(0), x, y);
			AddTile(tile);
		}
	}
}
