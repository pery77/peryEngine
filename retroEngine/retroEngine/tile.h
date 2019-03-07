#pragma once
#include "raylib.h"

class Tile
{
public:

	Tile(Texture2D& texture);
	~Tile();

	void Draw(int x, int y);

private:

	Texture2D baseSprite;
};
