#pragma once
#include "raylib.h"

namespace pery {
	class Sprite
	{
	public:
		Sprite(Texture2D & tileset, int tileCol, int tileRow, int tileWidth, int tileHeight);
		~Sprite();

		Texture2D & tilesetTexture;
		Rectangle source;
		Vector2 origin;


	private:

		int tileWidth;
		int tileHeight;
		int col;
		int row;
	};
}