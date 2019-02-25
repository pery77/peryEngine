#include "glow.h"

Glow::Glow(int screenWidth, int screenHeight, int screenScale)
{

	brightPass = LoadRenderTexture(screenWidth, screenHeight);
	blurH = LoadRenderTexture(screenWidth, screenHeight);
	blurV = LoadRenderTexture(screenWidth, screenHeight);
	
	SetFilter(0);

	Glow::screenScale = screenScale;

	float brightThreshold[1] = { 0.5 };
	float brightPower[1] = { 3.0 };
	float blurPower[1] = {1.05 };

	SetShaderValue(bright, thresholdLoc, brightThreshold, 1);
	SetShaderValue(bright, powerLoc, brightPower, 1);

	SetShaderValue(blur, blurpowerLoc, blurPower, 1);
}

Glow::~Glow()
{
	Unload();
}

void Glow::DrawGlow(Texture2D texture)
{

	//Extract bright pass.
	BeginTextureMode(brightPass);
		BeginShaderMode(bright);
			DrawTextureEx(texture, { 0,0 }, 0, 1, WHITE);
		EndShaderMode();
	EndTextureMode();

	//Blend bloom and main draw.
	BeginBlendMode(1);

	if (!IsKeyDown(KEY_A))
		DrawTextureEx(texture, { 0,0 }, 0, screenScale, WHITE);
		
	if (!IsKeyDown(KEY_S))
	{
		Texture2D t = Blur(brightPass.texture);
		DrawTextureEx(t, { 0,0 }, 0, screenScale, WHITE);
	}
	EndBlendMode();
}
Texture2D Glow::Blur(Texture2D texture)
{
	//Horizontal blur pass
	SetShaderValue(blur, blurDirLoc, h, 1);
	BeginTextureMode(blurH);
		BeginShaderMode(blur);
			DrawTextureEx(texture, { 0,0 }, 0, 1, WHITE);
		EndShaderMode();
	EndTextureMode();

	//Vertical blur pass
	SetShaderValue(blur, blurDirLoc, v, 1);
	BeginTextureMode(blurV);
		BeginShaderMode(blur);
			DrawTextureEx(blurH.texture, { 0,0 }, 0, 1, WHITE);
		EndShaderMode();
	EndTextureMode();
	
	return blurV.texture;
}

void Glow::SetFilter(int filter)
{
	
	SetTextureFilter(brightPass.texture, filter);
	SetTextureFilter(blurH.texture, filter);
	SetTextureFilter(blurV.texture, filter);
}

void Glow::Unload()
{
	UnloadRenderTexture(brightPass);
	UnloadRenderTexture(blurH);
	UnloadRenderTexture(blurV);

	UnloadShader(bright);
	UnloadShader(blur);
}
