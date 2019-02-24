/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

	int screenWidth = 480;
	int screenHeight = 272;
	int screenScale = 3;

	int pixelGlow = 1;

	void drawGrid();

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------


	InitWindow(screenWidth * screenScale, screenHeight* screenScale, "raylib [core] example - basic window");
	
	RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
	SetTextureFilter(target.texture, 0);

	RenderTexture2D brightPass = LoadRenderTexture(screenWidth, screenHeight);
	RenderTexture2D blurH = LoadRenderTexture(screenWidth, screenHeight);
	RenderTexture2D blurV = LoadRenderTexture(screenWidth, screenHeight);

	SetTextureFilter(brightPass.texture, pixelGlow);
	SetTextureFilter(blurH.texture, pixelGlow);
	SetTextureFilter(blurV.texture, pixelGlow);

	Texture2D bgImg = LoadTexture("../Assets/img4.png");
	SetTextureFilter(bgImg, 0);
	
	Rectangle sourceRec = { 0.0f, 0.0f, (float)screenWidth, -(float)screenHeight };
	Rectangle destRec = { 0.0f, 0.0f, (float)screenWidth*screenScale, (float)screenHeight*screenScale };

	Shader bright = LoadShader(0, "../Assets/brightness.fs");
	Shader blur = LoadShader(0,"../Assets/blur.fs");

	int blurDirLoc = GetShaderLocation(blur, "horizontal");

	int thresholdLoc = GetShaderLocation(bright, "threshold");
	int powerLoc = GetShaderLocation(bright, "power");

	float h[1] = { 0 };
	float v[1] = { 1 };

	float brightThreshold[1] = { 0.2 };
	float brightPower[1]     = { 2.0 };



	float fz[1] = { 1 };

	SetShaderValue(bright, thresholdLoc, brightThreshold, 1);
	SetShaderValue(bright, powerLoc,     brightPower, 1);

	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------
	float a = 0;
	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		a++;
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------
		if (IsKeyPressed(KEY_F11))
		{
			ToggleFullscreen();
			SetWindowPosition(100, 100);
		}
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

			ClearBackground(BLACK);

//Main draw
				BeginTextureMode(target);
		
					DrawTextureEx(bgImg, { (float)GetMouseX(),(float)GetMouseY()-300 }, 0,1, WHITE);
					//drawGrid();	

				EndTextureMode();

//end main draw

				//Extract bright pass.
				BeginTextureMode(brightPass);
					BeginShaderMode(bright);
						DrawTextureEx(target.texture, { 0,0 }, 0, 1, WHITE);
					EndShaderMode();
				EndTextureMode();

				//Horizontal blur pass
				SetShaderValue(blur, blurDirLoc, h, 1);
				BeginTextureMode(blurH);			
					BeginShaderMode(blur);
						DrawTextureEx(brightPass.texture, { 0,0 }, 0, 1, WHITE);
					EndShaderMode();
				EndTextureMode();

				//Vertical blur pass
				SetShaderValue(blur, blurDirLoc, v, 1);
				BeginTextureMode(blurV);
					BeginShaderMode(blur);
						DrawTextureEx(blurH.texture, { 0,0 }, 0, 1, WHITE);
					EndShaderMode();
				EndTextureMode();
				
				//Blend bloom and main draw.
				BeginBlendMode(1);
				
				    if (!IsKeyDown(KEY_A))
						DrawTexturePro(target.texture, sourceRec, destRec, { 0,0 }, 0.0f, WHITE);
					if (!IsKeyDown(KEY_SPACE)) 
						DrawTextureEx(blurV.texture, { 0,0 }, 0, screenScale, WHITE);
	
				EndBlendMode();

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	UnloadTexture(bgImg);       // Texture unloading
	UnloadRenderTexture(target);
	UnloadRenderTexture(brightPass);

	UnloadShader(blur);

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

void drawGrid()
{

	for (int y = 0; y < screenHeight; y = y + 16)
	{
		DrawLine(0, y, screenWidth, y, RED);
	}

	for (int x = 0; x < screenWidth; x = x + 16)
	{
		DrawLine(x, 0, x, screenHeight, RED);

	}

	DrawRectangleLines(0, 0, screenWidth, screenHeight, GREEN);
	
}