#include "engine.h"

Engine::Engine()
{
}

Engine::~Engine()
{
	UnloadRenderTexture(mainRender);
}

void Engine::Init()
{
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(ScreenWidth * ScreenScale, ScreenHeight* ScreenScale, 
		"Pery Engine test");
	SetTargetFPS(60);
	
	mainRender = LoadRenderTexture(ScreenWidth, ScreenHeight);
	SetTextureFilter(mainRender.texture, 0);

	glow = new Glow(ScreenWidth, ScreenHeight);
	glow->SetFilter(1);

	LoadImages();

	//Texture2D blurTexture;

	

}

void Engine::Go()
{
	Init();
	MainLoop();
}

void Engine::MainLoop()
{
	
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		ProcessInput();
		Update();
		RenderFrame();
	}

	CloseWindow();        // Close window and OpenGL context
}

void Engine::ProcessInput()
{
}

void Engine::Update()
{
}

void Engine::RenderFrame()
{
	BeginDrawing();
	ClearBackground(GRAY);
	BeginTextureMode(mainRender);
	Texture2D ts = Texture2D(tilesetManager.GetTileset(0));

	Tile t(ts, 2, 1);

	tilesetManager.AddTile(t);
	for (int x=0; x < 30; x++)
	{
		for (int y = 0; y < 17; y++) 
		{
			tilesetManager.GetTile(0).Draw(x * 16, y * 16);
			//DrawPixel(x * 16, y * 16, WHITE);
		}
	}
	DrawTextureRec(tilesetManager.GetTileset(0), { 0, 0, 100, 100 },
		{ 10, 10 }, WHITE);  // Draw part of the texture
	EndTextureMode();

	DrawTexturePro(mainRender.texture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);

	EndDrawing();
}

void Engine::LoadImages()
{
	Texture2D tileset1 = LoadTexture("../Assets/blocks.png");
	SetTextureFilter(tileset1, 0);

	tilesetManager.AddTileset(tileset1);



}
