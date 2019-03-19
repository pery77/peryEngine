#pragma once
#include "raylib.h"
#include <iostream> //For NULL
#define TILESIZE 16

namespace pery
{
	class Tile
	{
	public:

		Tile(Texture2D & tileset, int tileCol, int tileRow, int tileWidth, int tileHeight);
		~Tile();

		void Draw(int x, int y);
		int tileWidth;
		int tileHeight;

	private:

		Texture2D & tilesetTexture;
		int col;
		int row;



	};
}