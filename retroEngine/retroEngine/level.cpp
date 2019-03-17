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

		CurrentMap->MapLoaded.tilesets[ts].source;

		Texture2D currenTileset = LoadTexture("../Assets/tileset1.png");
		SetTextureFilter(currenTileset, 0);

		tilesetManager->AddTileset(currenTileset);


	}

		tilesetManager->MakeTiles();



	LoadLevel();
}

pery::Level::~Level()
{
	LOG("Unload level");
	delete tilesetManager;
	delete CurrentMap;

	map.clear();

}

void pery::Level::AddTile(int x, int y, Tile * tile)
{
	if (map[x][y] != NULL)
	{
		delete map[x][y];
		map[x][y] = NULL;
	}

	map[x][y] = tile;
}

pery::Tile * pery::Level::GetTile(int x, int y)
{

	if ( map.size() == 0) return nullptr;

 	//Clamp array.
	if (x < 0)			x = 0;
	if (x > width - 1)  x = width - 1;
	if (y < 0)			y = 0;
	if (y > height - 1) y = height - 1;

	return map[x][y];
}

void pery::Level::LoadLevel()
{


	

	width  = CurrentMap->MapLoaded.width;
	height = CurrentMap->MapLoaded.height;

	SetDimensions(width, height);

	int co = 0;
	for (int y = 0; y < GetHeight(); y++)
	{
		for (int x = 0; x < GetWidth(); x++)
		{
			int t = CurrentMap->MapLoaded.layers[0].IDs[co]-1;
			co++;
			if (t >= 0) {
				AddTile(x, y, tilesetManager->GetTile(t));
			}
		}
	}
}

int pery::Level::GetWidth()
{
	return width;
}

int pery::Level::GetHeight()
{
	return height;
}

void pery::Level::SetDimensions(int width, int height)
{
	//width are rows
	map.resize(width);

	for (int i = 0; i < width; i++)
	{
		map.at(i).resize(height, 0);
	}
}
