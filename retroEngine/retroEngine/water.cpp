#include "water.h"


Water::Water(int screenWidth, int screenHeight)
{
	Water::screenWidth = screenWidth;
	Water::screenHeight = screenHeight;
	result = LoadRenderTexture(screenWidth, screenHeight);
	SetTextureFilter(result.texture, 0);


	SetShaderValue(waterShader, timeLoc, time, 1);
	
}

Water::~Water()
{
	Unload();
}

void Water::Unload()
{
	UnloadRenderTexture(result);
	UnloadShader(waterShader);
}

void Water::DrawWater(Texture2D texture, float height)
{
	BeginTextureMode(result);
		//Original
		//DrawTexturePro(texture,	{ 0, 0, (float)screenWidth, (float)screenHeight - height }, { 0, 0, (float)screenWidth, (float)screenHeight - height }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(texture,
			{ 0, 0, (float)screenWidth, (float)screenHeight },
			{ 0, 0, (float)screenWidth, (float)screenHeight },
			{ 0, 0 }, 0, WHITE);
		//Reflection
		BeginShaderMode(waterShader);
		time[0]++;
		SetShaderValue(waterShader, timeLoc, time, 1);
		DrawTexturePro(texture, { 0, (float)screenHeight - (height * 2), (float)screenWidth, -height },
			{ 0, (float)screenHeight - height, (float)screenWidth, height },
			{ 0, 0 }, 0, WHITE);
		EndShaderMode();
	EndTextureMode();
	
}