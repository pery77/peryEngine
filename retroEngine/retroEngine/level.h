#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "tile.h"
#include "tilesetManager.h"
#include "minijson_reader.hpp"
#include "tools.h"

namespace pery
{
	class Level
	{
	public:
		Level(std::string levelName);
		~Level();

		void AddTile(int x, int y, Tile* tile);
		Tile* GetTile(int x, int y);

		void LoadLevel();
		int GetWidth();
		int GetHeight();

	private:
		std::vector<std::vector<Tile*>> map;

		int width, height;
		void SetDimensions(int width, int height);
		std::string levelName;

		//Tileset manager
		TilesetManager* tilesetManager;

	};
}