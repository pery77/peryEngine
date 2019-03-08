#pragma once
#include "raylib.h"

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
	const int size = 16;
};
