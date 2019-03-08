#include "tile.h"

Tile::Tile(Texture2D & tileset, int tileCol, int tileRow)
	:tilesetTexture (tileset)
{
	col = tileCol * size;
	row = tileRow * size;
}

Tile::~Tile()
{
	//UnloadTexture(tilesetTexture);
}

void Tile::Draw(int x, int y)
{
	DrawTextureRec(tilesetTexture, {(float)col, (float)row , (float)size, (float)size}, 
		{ (float)x, (float)y }, WHITE);  // Draw part of the texture
	
	DrawPixel(col, row, WHITE);
}                                                                                                                                                                                                                                                                                                                                                                                                                         