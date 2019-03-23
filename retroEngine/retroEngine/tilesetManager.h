#pragma once
#include "raylib.h"
#include <vector>
#include "tile.h"
#include "tools.h"
#include "TMX2Map.h"

namespace pery {
	class TilesetManager {

	public:

		TilesetManager();
		~TilesetManager();

		//void AddTileset(Texture2D &texture);
		void AddTileset(TSXTileSet & tsxTileset);
		TSXTileSet& GetTileset(int index);

		void AddTile(Tile *tile);
		Tile* GetTile(int index);

		void MakeTiles(TSXTileSet & tsxTileset);

	private:
		std::vector <TSXTileSet> tilesetsList;
		std::vector <Tile*>     tilesList;
	};
}