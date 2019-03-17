#include "engine.h"

pery::Engine::Engine()
{
}

pery::Engine::~Engine()
{
	pery::Tools::Log("Unload Engine");
	UnloadRenderTexture(mainRender);
	delete glow;
	delete camera;
	delete level;
}

//Initialize engine.
void pery::Engine::Init()
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

	level = new Level("test2");

	camera = new CameraView(ScreenWidth, ScreenHeight, 1, level->GetWidth(), level->GetHeight());
}

//Start engine.
void pery::Engine::Go()
{
	Init();
	MainLoop();
}

void pery::Engine::MainLoop()
{
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		ProcessInput();
		Update();
		RenderFrame();
	}

	CloseWindow();					// Close window and OpenGL context
}

void pery::Engine::ProcessInput()
{
	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
	{
		int x = GetMousePosition().x;
		int y = GetMousePosition().y;

		camera->GoTo(x, y);
	}

	if (IsKeyPressed(KEY_W))
	{
		camera->GoTo(camera->GetPosition().x, camera->GetPosition().y - 16);
	}
	if (IsKeyPressed(KEY_S))
	{
		camera->GoTo(camera->GetPosition().x, camera->GetPosition().y + 16);
	}
	if (IsKeyPressed(KEY_D))
	{
		camera->GoTo(camera->GetPosition().x + 16, camera->GetPosition().y);
	}
	if (IsKeyPressed(KEY_A))
	{
		camera->GoTo(camera->GetPosition().x - 16, camera->GetPosition().y);
	}
	if (IsKeyPressed(KEY_SPACE))
	{
		camera->GoTo(0, 0);
	}
}

void pery::Engine::Update()
{
	camera->Update();
}

//Draw loop.
void pery::Engine::RenderFrame()
{
	int camOffsetX, camOffsetY;
	Rectangle bounds = camera->GetTileBounds();
	camOffsetX = camera->GetOffset().x;
	camOffsetY = camera->GetOffset().y;

	BeginDrawing();
	ClearBackground(BLACK);
	//Draw game to texture.
	BeginTextureMode(mainRender);

	//Draw tiles.
	for (int y = 0, tileY = bounds.y; y < bounds.height; y++, tileY++)
	{
		for (int x = 0, tileX = bounds.x; x < bounds.width; x++, tileX++)
		{
			level->GetTile(tileX, tileY)->Draw((x * TILESIZE) - camOffsetX, (y * TILESIZE) - camOffsetY);
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
	glow->SetValues(.5, 1.1, 1.5);
	glow->BlurTexture = glow->DrawGlow(mainRender.texture);
	DrawTexturePro(glow->BlurTexture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);
	//End draw main + postprocess 
	EndBlendMode();

	EndDrawing();
}