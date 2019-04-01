#pragma once
#include "raylib.h"
#include "tilesetManager.h"

namespace pery {
	class Sprite
	{
	public:
		//Sprite(Texture2D & tileset, int tileCol, int tileRow, int tileWidth, int tileHeight);
		Sprite(TSXTileSet & tileset, int tile);
		~Sprite();

		Texture2D & tilesetTexture;
		TSXTileSet & tileset;
		Rectangle source;
		Vector2 origin;


	private:

		int tileWidth;
		int tileHeight;
		int col;
		int row;
	};
}