#include "sprite.h"

pery::Sprite::Sprite(TSXTileSet & tileset, int id)
	:tileset(tileset), tilesetTexture(tileset.image.texture)
{
	tileWidth = tileset.tileWidth;
	tileHeight = tileset.tileHeight;

	col = (id % tileset.columns) * tileWidth;
	row = (id / tileset.columns) * tileHeight;

	source = { (float)col, (float)row , (float)tileWidth, (float)tileHeight };
	origin = { (float)tileWidth * 0.5f,(float)tileHeight *0.5f };
}

pery::Sprite::~Sprite()
{
}


