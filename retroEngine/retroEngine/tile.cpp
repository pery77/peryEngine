#include "tile.h"
#include <iostream>

Tile::Tile(Texture2D & tileset, int tileCol, int tileRow)
	:tilesetTexture (tileset)
{
	col = tileCol * size;
	row = tileRow * size;
}

Tile::~Tile()
{
}

void Tile::Draw(int x, int y)
{
	DrawTextureRec(tilesetTexture, {(float)col, (float)row , (float)size, (float)size}, 
		{ (float)x * size, (float)y  * size }, WHITE);  // Draw part of the texture
}                                                                                                                                                                                                                                                                                                                                                                                                                         