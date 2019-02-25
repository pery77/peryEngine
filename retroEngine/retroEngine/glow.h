#pragma once
#include "raylib.h"

class Glow
{

public:

	int screenScale = 1;

	RenderTexture2D brightPass;
	RenderTexture2D blurH;
	RenderTexture2D blurV;

	Glow(int screenWidth, int screenHeight, int screenScale);
	~Glow();

	Shader bright = LoadShader(0, "../Assets/brightness.fs");
	Shader blur   = LoadShader(0, "../Assets/blur.fs");

	int blurDirLoc = GetShaderLocation(blur, "horizontal");
	int blurpowerLoc = GetShaderLocation(blur, "power");

	int thresholdLoc = GetShaderLocation(bright, "threshold");
	int powerLoc = GetShaderLocation(bright, "power");

	float h[1] = { 0 };
	float v[1] = { 1 };
	
	void DrawGlow(Texture2D texture);
	void SetFilter(int filter);
	Texture2D Blur(Texture2D texture);

	void Unload();
private:

};