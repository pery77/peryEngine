#include "tile.h"

Tile::Tile(Texture2D & texture)
{
	baseSprite = texture;
}

Tile::~Tile()
{
	UnloadTexture(baseSprite);
}

void Tile::Draw(int x, int y)
{
	DrawTextureRec(baseSprite, {0,0,64,64}, { (float)x, (float)y }, WHITE);  // Draw part of the texture
}
