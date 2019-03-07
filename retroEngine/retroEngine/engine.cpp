#include "engine.h"

Engine::Engine()
{
}

Engine::~Engine()
{

}

void Engine::Init()
{
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(ScreenWidth * ScreenScale, ScreenHeight* ScreenScale, 
		"Pery Engine test");
	SetTargetFPS(60);
	
	mainRender = &LoadRenderTexture(ScreenWidth, ScreenHeight);
	SetTextureFilter(mainRender->texture, 0);

	glow = new Glow(ScreenWidth, ScreenHeight);
	glow->SetFilter(1);

	LoadImages();
	Texture2D bgImg = LoadTexture("../Assets/img1.png");
	SetTextureFilter(bgImg, 0);

	Texture2D blurTexture;

	

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
		ClearBackground(RAYWHITE);
		testTile->Draw(0,0);
	EndDrawing();
}

void Engine::LoadImages()
{
	Texture2D bgImg = LoadTexture("../Assets/img1.png");
	SetTextureFilter(bgImg, 0);

	textureManager.AddTexture(bgImg);

	testTile = new Tile(textureManager.GetTexture(0));
}
