#include "tilesetManager.h"
#include "TMX2Map.h"

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
		UnloadTexture(tilesetsList[i].texture);
	}
	tilesetsList.clear();
}

void pery::TilesetManager::AddTileset(TSXTileSet & tsxTileset)
{
	tilesetsList.push_back(tsxTileset);

	//Load tileset png file int texture 2D
	std::string st = "../Assets/Tilesets/" + tsxTileset.image.source;

	const char * imageSource = st.c_str();
	tsxTileset.texture = LoadTexture(imageSource);

	SetTextureFilter(tsxTileset.texture, 0);

	MakeTiles(tsxTileset);

}

pery::TSXTileSet & pery::TilesetManager::GetTileset(int index)
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

void pery::TilesetManager::MakeTiles(TSXTileSet & tsxTileset)
{
	
	Tile* tile;
	for (int y = 0; y < tsxTileset.rows; y++)
	{
		for (int x = 0; x < tsxTileset.columns; x++)
		{
			tile = new Tile(tsxTileset.texture, x, y);
			AddTile(tile);
		}
	}
}
