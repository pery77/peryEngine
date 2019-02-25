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
	InitWindow(screenWidth * screenScale, screenHeight* screenScale, "raylib [core] example - basic window");
	
	RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
	SetTextureFilter(target.texture, 0);

	Glow glow(screenWidth, screenHeight, screenScale);

	Texture2D bgImg = LoadTexture("../Assets/img1.png");
	SetTextureFilter(bgImg, 0);
	
	Rectangle sourceRec = { 0.0f, 0.0f, (float)screenWidth, -(float)screenHeight };
	
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
				BeginTextureMode(target);
					sourceRec.x = a;
					DrawTextureRec(bgImg, sourceRec, { 0,0 }, WHITE);
					//drawGrid();	
				EndTextureMode();

			//end main draw

				glow.DrawGlow(target.texture);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// Unloading
	UnloadTexture(bgImg);       
	UnloadRenderTexture(target);

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