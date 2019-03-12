#include "glow.h"

Glow::Glow(int screenWidth, int screenHeight)
{

	brightPass = LoadRenderTexture(screenWidth, screenHeight);
	blurH = LoadRenderTexture(screenWidth, screenHeight);
	blurV = LoadRenderTexture(screenWidth, screenHeight);
	
	SetFilter(0);
	SetSpread(1);

	Glow::SetValues(0.5, 1, 1);

}

Glow::~Glow()
{
	std::cout << "Unload glow"<<std::endl;
	Unload();
}

void Glow::Unload()
{
	UnloadTexture(BlurTexture);

	UnloadRenderTexture(brightPass);
	UnloadRenderTexture(blurH);
	UnloadRenderTexture(blurV);

	UnloadShader(bright);
	UnloadShader(blur);
}

Texture2D Glow::DrawGlow(Texture2D texture)
{

	//Extract bright pass.
	BeginTextureMode(brightPass);
		BeginShaderMode(bright);
			DrawTextureEx(texture, { 0,0 }, 0, 1, WHITE);
		EndShaderMode();
	EndTextureMode();
	
	return Blur(brightPass.texture);

}

Texture2D Glow::Blur(Texture2D texture)
{
	//Horizontal blur pass
	directionPass[0] = 0;
	SetShaderValue(blur, blurDirLoc, directionPass, 1);
	BeginTextureMode(blurH);
		BeginShaderMode(blur);
			DrawTextureEx(texture, { 0,0 }, 0, 1, WHITE);
		EndShaderMode();
	EndTextureMode();
	directionPass[0] = 1;
	//Vertical blur pass
	SetShaderValue(blur, blurDirLoc, directionPass, 1);
	BeginTextureMode(blurV);
		BeginShaderMode(blur);
			DrawTextureEx(blurH.texture, { 0,0 }, 0, 1, WHITE);
		EndShaderMode();
	EndTextureMode();
	
	return blurV.texture;
}

void Glow::BigGlow(Texture2D texture)
{
	SetValues(.6, 1, 1.1);
	SetSpread(4);
	BlurTexture = DrawGlow(texture);
	SetSpread(2);
	BlurTexture = Blur(BlurTexture);
	SetSpread(1);
	BlurTexture = Blur(BlurTexture);
}

void Glow::SetValues(float threshold, float brightPower, float blurPower)
{
	float brightThreshold[1] = { threshold };
	float brightP[1] = { brightPower };
	float blurP[1] = { blurPower };

	SetShaderValue(bright, thresholdLoc, brightThreshold, 1);
	SetShaderValue(bright, powerLoc, brightP, 1);

	SetShaderValue(blur, blurPowerLoc, blurP, 1);

}

void Glow::SetSpread(float spread)
{
	float blurS[1] = { spread };
	SetShaderValue(blur, blurSpreadLoc, blurS, 1);
}

void Glow::SetFilter(int filter)
{
	
	SetTextureFilter(brightPass.texture, filter);
	SetTextureFilter(blurH.texture, filter);
	SetTextureFilter(blurV.texture, filter);
}


