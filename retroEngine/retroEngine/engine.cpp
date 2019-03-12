#include "engine.h"

Engine::Engine()
{
}

Engine::~Engine()
{
	std::cout << "Unload Engine" << std::endl;
	UnloadRenderTexture(mainRender);
	delete glow;
	delete camera;
	delete level;
}

//Initialize engine.
void Engine::Init()
{
	//Set Vsync
	SetConfigFlags(FLAG_VSYNC_HINT);
	//Create window
	InitWindow(ScreenWidth * ScreenScale, ScreenHeight* ScreenScale, "PeryEngine");
	SetTargetFPS(60);
	//ToggleFullscreen();
	//Create main texture and disable texture filter.
	mainRender = LoadRenderTexture(ScreenWidth, ScreenHeight);
	SetTextureFilter(mainRender.texture, 0);

	//Create glow effect.
	glow = new Glow(ScreenWidth, ScreenHeight);
	glow->SetFilter(1);

	level = new Level("test");
	
	camera = new Cam(30, 17, 0.5);
}

//Start engine.
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

	CloseWindow();					// Close window and OpenGL context
}

void Engine::ProcessInput()
{
}

void Engine::Update()
{
}

//Draw loop.
void Engine::RenderFrame()
{
	int camOffsetX, camOffsetY;
	Rectangle bounds = { 10,10,29,16 };// camera->GetTileBounds();
	camOffsetX = GetMouseX();// camera->GetOffset().x;
	camOffsetY = camera->GetOffset().y;
	//std::cout<< bounds.height << std::endl;
	BeginDrawing();
		ClearBackground(BLACK);
			//Draw game to texture.
			BeginTextureMode(mainRender);
			
			for (int y = 0, tileY = bounds.y; y < bounds.height; y++, tileY++)
			{
				for (int x = 0, tileX = bounds.x; x < bounds.width; x++, tileX++)
				{
					level->GetTile(tileX, tileY)->Draw((x * 16) - camOffsetX, (y * 16) - camOffsetY);
				}
			}

			//End draw game in main texture.
			EndTextureMode();

			//Blend texture for postprocess effect.
			BeginBlendMode(1);
				//Draw main texture scaled to screen.
				DrawTexturePro(mainRender.texture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);
				//Draw glow frist pass (big glow effect)
				glow->BigGlow(mainRender.texture);
				DrawTexturePro(glow->BlurTexture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);
				//Draw glow second pass
				glow->SetValues(.4, 1.1, 1.5);
				glow->BlurTexture = glow->DrawGlow(mainRender.texture);
				DrawTexturePro(glow->BlurTexture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);
			//End draw main + postprocess 
			EndBlendMode();

	EndDrawing();

}