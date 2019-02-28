#include "water.h"

Water::Water(int screenWidth, int screenHeight)
{
	Water::screenWidth = screenWidth;
	Water::screenHeight = screenHeight;
	result = LoadRenderTexture(screenWidth, screenHeight);
}

Water::~Water()
{
	Unload();
}

void Water::Unload()
{
	UnloadRenderTexture(result);

}

void Water::DrawWater(Texture2D texture, float height)
{
	BeginTextureMode(result);

		//Original
		DrawTexturePro(texture, 
			{ 0, 0, (float)screenWidth, (float)screenHeight - height }, 
			{ 0, 0, (float)screenWidth, (float)screenHeight - height }, 
			{ 0, 0 }, 0, WHITE);
		//Reflection
		DrawTexturePro(texture, { 0, (float)screenHeight - ( height * 2), (float)screenWidth, - height }, 
			{ 0, (float)screenHeight - height, (float)screenWidth, height }, 
			{ 0, 0 }, 0, WHITE);

	EndTextureMode();
	
}