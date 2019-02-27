#include "water.h"

Water::Water(int screenWidth, int screenHeight)
{

}

Water::~Water()
{
	Unload();
}

void Water::Unload()
{
	//UnloadRenderTexture();
}

Texture2D Water::DrawWater(Texture2D texture)
{
	DrawTextureRec(texture, {0,0,480,277}, { 0,0 }, WHITE);
	return texture;
}