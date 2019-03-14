#include "tile.h"


pery::Tile::Tile(Texture2D & tileset, int tileCol, int tileRow)
	:tilesetTexture (tileset)
{
	col = tileCol * TILESIZE;
	row = tileRow * TILESIZE;
}

pery::Tile::~Tile()
{
}

void pery::Tile::Draw(int x, int y)
{
	if (this == NULL) return;

	Vector2 pos = { (float)x, (float)y };
	DrawTextureRec(tilesetTexture, {(float)col, (float)row , 
		(float)TILESIZE, (float)TILESIZE }, pos, WHITE);  // Draw part of the texture

}                                                                                                                                                                                                                                                                                                                                                                                                                         