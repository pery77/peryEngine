#pragma once
#include "raylib.h"
#include <vector>
#include "tile.h"

class TilesetManager {

public:

	TilesetManager();
	~TilesetManager();

	void AddTilesetTexture(Texture2D& texture);
	Texture2D& GetTilesetTexture(int index);

	void AddTile(Tile & tile);
	Tile& GetTile(int index);

private:

	std::vector <Texture2D> textureList;
	std::vector<Tile> tilesList;
};