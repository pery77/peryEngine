#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "tile.h"
#include "tilesetManager.h"

#include "TMX2Map.h"

namespace pery
{
	class Level
	{
	public:
		Level(std::string levelName);
		~Level();

		void AddTile(int x, int y, Tile* tile, int layer);
		Tile* GetTile(int layer, int x, int y);

		void LoadLevel();

		int GetWidth()    { return width;    }
		int GetHeight()   { return height;   }
		int GetLayers()   { return layers;   }
		int GetTileSize() { return tileSize; }

		TMX2Map* CurrentMap;

	private:

		std::vector<std::vector<std::vector<Tile*>>> map; //[Layer,x,y]

		int width, height, layers, tileSize;
		void SetDimensions(int layers, int width, int height);
		std::string levelName;

		//Tileset manager
		TilesetManager* tilesetManager;

	};
}