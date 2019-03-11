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
	if (this == NULL) return;
	Vector2 pos = { (float)x, (float)y };
	DrawTextureRec(tilesetTexture, {(float)col, (float)row , (float)size, (float)size}, pos, WHITE);  // Draw part of the texture
	//DrawText("123", pos.x, pos.y,0, RED);

}                                                                                                                                                                                                                                                                                                                                                                                                                         