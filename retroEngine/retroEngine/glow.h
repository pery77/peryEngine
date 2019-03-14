#pragma once
#include "raylib.h"
#include "tools.h"

namespace pery {
	class Glow
	{
	public:

		Glow(int screenWidth, int screenHeight);
		~Glow();

		Texture2D DrawGlow(Texture2D texture);
		Texture2D Blur(Texture2D texture);
		void BigGlow(Texture2D texture);

		Texture2D BlurTexture;

		void SetFilter(int filter);
		void SetValues(float threshold, float brightPower, float blurPower);
		void SetSpread(float spread);
		void Unload();

	private:
		//Render textures 
		RenderTexture2D brightPass;
		RenderTexture2D blurH;
		RenderTexture2D blurV;
		//Shaders
		Shader bright = LoadShader(0, "../Assets/Shaders/brightness.frag");
		Shader blur = LoadShader(0, "../Assets/Shaders/blur.frag");
		//Shaders parameters
		//for blur
		int blurDirLoc = GetShaderLocation(blur, "horizontal");
		int blurPowerLoc = GetShaderLocation(blur, "power");
		int blurSpreadLoc = GetShaderLocation(blur, "spread");
		//for bright
		int thresholdLoc = GetShaderLocation(bright, "threshold");
		int powerLoc = GetShaderLocation(bright, "power");
		//Fake bool for shader blur
		float directionPass[1] = { 0 };

	};
}