#pragma once
#include "raylib.h"

class Glow
{

public:

	Glow(int screenWidth, int screenHeight);
	~Glow();
	
	Texture2D DrawGlow(Texture2D texture);
	Texture2D Blur(Texture2D texture);

	void SetFilter(int filter);
	void SetValues(float threshold, float brightPower, float blurPower);
	void SetSpread(float spread);
	void Unload();

private:

	RenderTexture2D brightPass;
	RenderTexture2D blurH;
	RenderTexture2D blurV;

	Shader bright = LoadShader(0, "../Assets/brightness.fs");
	Shader blur   = LoadShader(0, "../Assets/blur.fs");

	int blurDirLoc = GetShaderLocation(blur, "horizontal");
	int blurPowerLoc = GetShaderLocation(blur, "power");
	int blurSpreadLoc = GetShaderLocation(blur, "spread");

	int thresholdLoc = GetShaderLocation(bright, "threshold");
	int powerLoc = GetShaderLocation(bright, "power");

	float directionPass[1] = { 0 };
};