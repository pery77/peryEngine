#include "raylib.h"
#include "glow.h"
#include "water.h"

int screenWidth = 480;
int screenHeight = 272;
int screenScale = 3;

void drawGrid();

int main(int argc, char* argv[])
{



	// Initialization
	//--------------------------------------------------------------------------------------

	SetConfigFlags(FLAG_VSYNC_HINT); 
	InitWindow(screenWidth * screenScale, screenHeight* screenScale, "Pery Engine test");
	
	RenderTexture2D mainRender = LoadRenderTexture(screenWidth, screenHeight);
	SetTextureFilter(mainRender.texture, 0);

	Glow glow(screenWidth, screenHeight);
	glow.SetFilter(1);

	Water water(screenWidth, screenHeight);
	
	Texture2D bgImg = LoadTexture("../Assets/img1.png");
	SetTextureFilter(bgImg, 0);

	Texture2D blurTexture;

	Rectangle sourceRec = { 0.0f, 0.0f, (float)screenWidth, -(float)screenHeight };
	Rectangle scaledRec = { 0.0f, 0.0f, (float)screenWidth * (float) screenScale, (float)screenHeight * (float) screenScale };
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
		if (IsKeyPressed(KEY_F1))
		{
			glow.SetFilter(0);
		}
		if (IsKeyPressed(KEY_F2))
		{
			glow.SetFilter(1);
		}
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

			ClearBackground(BLACK);

			//Main draw
				BeginTextureMode(mainRender);
					DrawTextureRec(bgImg, { (float)-GetMouseX(), 0.0f, (float)screenWidth, -(float)screenHeight } , { 0,0 }, WHITE);
				EndTextureMode();
			//end main draw

				water.DrawWater(mainRender.texture, 16 * 5);
				
			//Blend bloom and final draw.
				BeginBlendMode(1);

				if (!IsKeyDown(KEY_A)) 
				{
					DrawTexturePro(water.result.texture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);
				}
				if (!IsKeyDown(KEY_S))
				{
					glow.SetValues(.6, 1, 1);
					glow.SetSpread(4);				
					blurTexture = glow.DrawGlow(water.result.texture);
					glow.SetSpread(2);
					blurTexture = glow.Blur(blurTexture);
					glow.SetSpread(1);
					blurTexture = glow.Blur(blurTexture);

					DrawTexturePro(blurTexture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);
					
					glow.SetValues(.4, .4, 2);
					blurTexture = glow.DrawGlow(water.result.texture);

					DrawTexturePro(blurTexture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);

				}

				EndBlendMode();	
	
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// Unloading
	UnloadTexture(bgImg);
	UnloadTexture(blurTexture);
	UnloadRenderTexture(mainRender);

	glow.Unload();
	water.Unload();

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