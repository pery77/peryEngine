#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "tile.h"
#include "tilesetManager.h"

#include "TMX2Map.h"
#include "Box2D\Box2D.h"
#include "levelEntity.h"

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

		//Collision
		std::vector<Rectangle> Colliders;
		
		void CreateBoxCollider(int x, int y, int w, int h);
		void CreateBox( int x, int y);

		const float pixelsPerMeter = 16.0f;
		b2World * World;

		std::vector<LevelEntity *> entities;

	private:

		std::vector<std::vector<std::vector<Tile*>>> map; //[layer,x,y]

		int width, height, layers, tileSize;
		void SetDimensions(int layers, int width, int height);
		std::string levelName;

		//Tileset manager
		TilesetManager* tilesetManager;

	};
}