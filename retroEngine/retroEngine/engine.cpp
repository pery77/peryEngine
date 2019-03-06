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
	InitWindow(screenWidth * screenScale, screenHeight* screenScale, 
		"Pery Engine test");
	SetTargetFPS(60);
	
	mainRender = &LoadRenderTexture(screenWidth, screenHeight);
	SetTextureFilter(mainRender->texture, 0);

	glow = new Glow(screenWidth, screenHeight);
	glow->SetFilter(1);

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

	EndDrawing();
}
