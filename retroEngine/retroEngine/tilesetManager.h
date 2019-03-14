#pragma once
#include "raylib.h"
#include <vector>
#include "tile.h"
#include "tools.h"

//How many tiles in texture
#define TILESETSIZE 16

namespace pery {
	class TilesetManager {

	public:

		TilesetManager();
		~TilesetManager();

		void AddTileset(Texture2D &texture);
		Texture2D& GetTileset(int index);

		void AddTile(Tile *tile);
		Tile* GetTile(int index);

		void MakeTiles();

	private:
		std::vector <Texture2D> tilesetsList;
		std::vector <Tile*>     tilesList;
	};
}