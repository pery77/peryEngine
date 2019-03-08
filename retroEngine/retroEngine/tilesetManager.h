#pragma once
#include "raylib.h"
#include <vector>
#include "tile.h"

class TilesetManager {

public:

	TilesetManager();
	~TilesetManager();

	void AddTileset(Texture2D &texture);
	Texture2D& GetTileset(int index);

	void AddTile(Tile *tile);
	Tile* GetTile(int index);

private:
	std::vector <Texture2D> tilesetsList;
	std::vector <Tile*>     tilesList;
};