#include "level.h"
using namespace std;

#include <iostream>

pery::Level::Level(std::string levelName)
{

	CurrentMap = new TMX2Map (levelName);
	CurrentMap->ShowMapInfo();

	this->levelName = levelName;

	tilesetManager = new TilesetManager();	

	for (int ts = 0; ts < CurrentMap->MapLoaded.tilesets.size(); ts ++)
	{
		tilesetManager->AddTileset(CurrentMap->MapLoaded.tilesets[ts].tileset);
	}

	width    = CurrentMap->MapLoaded.width;
	height   = CurrentMap->MapLoaded.height;
	layers   = CurrentMap->MapLoaded.renderQueue.size();
	tileSize = CurrentMap->MapLoaded.tileHeight;

	SetDimensions(CurrentMap->MapLoaded.renderQueue.size(), width, height);

	LoadLevel();

}

pery::Level::~Level()
{
	LOG("Unload level");
	delete tilesetManager;
	delete CurrentMap;

	map.clear();

}

void pery::Level::AddTile(int x, int y, Tile * tile, int layer)
{
	if (map[layer][x][y] != NULL)
	{
		delete map[layer][x][y];
		map[layer][x][y] = NULL;
	}

	map[layer][x][y] = tile;
}

pery::Tile * pery::Level::GetTile(int layer, int x, int y)
{

	if ( map.size() == 0) return nullptr;

 	//Clamp array.
	if (x < 0)			x = 0;
	if (x > width - 1)  x = width - 1;
	if (y < 0)			y = 0;
	if (y > height - 1) y = height - 1;

	return map[layer][x][y];
}

void pery::Level::LoadLevel()
{

	for (int i = 0; i < CurrentMap->MapLoaded.renderQueue.size(); i++)
	{
		if (CurrentMap->MapLoaded.renderQueue[i].imageLayer.id != -1)
		{
			const char * imageSource = CurrentMap->MapLoaded.renderQueue[i].imageLayer.image.source.c_str();
			CurrentMap->MapLoaded.renderQueue[i].imageLayer.image.texture = LoadTexture(imageSource);
		}
		if (CurrentMap->MapLoaded.renderQueue[i].layer.id != -1)
		{
			int co = 0;
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					int t = CurrentMap->MapLoaded.renderQueue[i].layer.IDs[co] - 1;
					co++;
					if (t >= 0) {
						AddTile(x, y, tilesetManager->GetTile(t), i);
					}
				}
			}
		}
	}
}

void pery::Level::SetDimensions(int layers, int width, int height)
{

	map.resize(layers);
	for (int i = 0; i < layers; i++)
	{
		map[i].resize(width);
		for (int j = 0; j < width; j++)
		{
			map[i][j].resize(height);
		}
	}

}
