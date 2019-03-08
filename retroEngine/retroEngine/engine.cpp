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

	for (int x=0; x < 30; x++)
	{
		for (int y = 0; y < 17; y++) 
		{
			tilesetManager.GetTile[0]->Draw(x * 16, y * 16);
		}
	}
	EndTextureMode();

	DrawTexturePro(mainRender.texture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);

	EndDrawing();
}

void Engine::LoadImages()
{
	Texture2D bgImg = LoadTexture("../Assets/blocks.png");
	SetTextureFilter(bgImg, 0);

	tilesetManager.AddTilesetTexture(bgImg);
	Tile* t = new Tile(tilesetManager.GetTilesetTexture(0), 0, 0);
	Tile& tt = *t;
	tilesetManager.AddTile(tt);

	UnloadTexture(bgImg);
}
