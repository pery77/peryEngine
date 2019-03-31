#include "sprite.h"

pery::Sprite::Sprite(Texture2D & tileset, int tileCol, int tileRow, int tileWidth, int tileHeight)
	: tilesetTexture(tileset), tileWidth(tileWidth), tileHeight(tileHeight)
{
	col = tileCol * tileHeight;
	row = tileRow * tileWidth;
	source = { (float)col, (float)row , (float)tileWidth, (float)tileHeight };
	origin = { (float)tileWidth * 0.5f,(float)tileHeight *0.5f };
}

pery::Sprite::~Sprite()
{
}


