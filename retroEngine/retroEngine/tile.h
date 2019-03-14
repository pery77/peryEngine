#pragma once
#include "raylib.h"
#include <iostream> //For NULL
#define TILESIZE 16

namespace pery
{
	class Tile
	{
	public:

		Tile(Texture2D & tileset, int tileCol, int tileRow);
		~Tile();

		void Draw(int x, int y);

	private:

		Texture2D & tilesetTexture;
		int col;
		int row;
	};
}