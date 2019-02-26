#include "raylib.h"
#include "glow.h"

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

	Glow glow(screenWidth, screenHeight, screenScale);
	glow.SetFilter(1);

	Texture2D bgImg = LoadTexture("../Assets/img1.png");
	SetTextureFilter(bgImg, 0);
	Rectangle sourceRec = { 0.0f, 0.0f, (float)screenWidth, -(float)screenHeight };
	Rectangle water = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
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
					sourceRec.x = -GetMouseX();
					DrawTextureRec(bgImg, sourceRec, { 0,0 }, WHITE);
				EndTextureMode();
			//end main draw


			//Blend bloom and main draw.
				BeginBlendMode(1);

				if (!IsKeyDown(KEY_A)) 
				{
					DrawTextureEx(mainRender.texture, { 0,0 }, 0, screenScale, WHITE);
				}
				if (!IsKeyDown(KEY_S))
				{
					//1 pass
	
					glow.SetValues(.65, 0.5, 1.2, 1);
					Texture2D t = glow.DrawGlow(mainRender.texture, 1);
					DrawTextureEx(t, { 0,0 }, 0, screenScale, WHITE);
				

					//2 pass
					glow.SetValues(.6, 4, 1.5, 4);
					t = glow.DrawGlow(mainRender.texture, 1);
					glow.SetValues(0.5, 1, 1, 1);
					t = glow.Blur(t);
					DrawTextureEx(t, { 0,0 }, 0, screenScale, WHITE);
				}

				EndBlendMode();	

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// Unloading
	UnloadTexture(bgImg);       
	UnloadRenderTexture(mainRender);
	glow.Unload();


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