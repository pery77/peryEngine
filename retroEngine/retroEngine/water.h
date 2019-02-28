#pragma once
#include "raylib.h"
class Water
{
public:

	RenderTexture2D result;

	Water(int screenWidth, int screenHeight);
	~Water();

	void DrawWater(Texture2D mainTexture, float height);
	void Unload();

private:

	int screenWidth;
	int screenHeight;

};