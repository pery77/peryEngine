#pragma once
#include "raylib.h"
class Water
{
public:

	Water(int screenWidth, int screenHeight);
	~Water();

	Texture2D DrawWater(Texture2D mainTexture);
	void Unload();

private:
};