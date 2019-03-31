#include "tile.h"




pery::Tile::Tile(Texture2D & tileset, int tileCol, int tileRow, int tileWidth, int tileHeight)
	: tilesetTexture(tileset), tileWidth(tileWidth) , tileHeight(tileHeight)
{
	col = tileCol * tileHeight;
	row = tileRow * tileWidth;

}

	pery::Tile::~Tile()
{
}

void pery::Tile::Draw(int x, int y)
{
	if (this == NULL) return;

	Vector2 pos = { (float)x, (float)y };
	DrawTextureRec(tilesetTexture, {(float)col, (float)row , 
		(float)tileWidth, (float)tileHeight }, pos, WHITE);  // Draw part of the texture

}                                                                                                                                                                                                                                                                                                                                                                                                                         