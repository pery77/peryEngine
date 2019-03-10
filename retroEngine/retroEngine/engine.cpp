#include "engine.h"

Engine::Engine()
{
}

Engine::~Engine()
{
	UnloadRenderTexture(mainRender);
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

	//Create tile manager
	tilesetManager = new TilesetManager();

	LoadImages();
	LoadLevel();
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

	BeginDrawing();
		ClearBackground(BLACK);
			//Draw game to texture.
			BeginTextureMode(mainRender);

				for (int x=0; x < level->GetWidth(); x++)
				{
					for (int y = 0; y < level->GetHeight(); y++) 
					{
						level->GetTile(x, y)->Draw(x, y);
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

void Engine::LoadImages()
{
	Texture2D tileset1 = LoadTexture("../Assets/tileset1.png");
	SetTextureFilter(tileset1, 0);


	tilesetManager->AddTileset(tileset1);
	tilesetManager->MakeTiles();

}

void Engine::LoadLevel()
{
	//Temporary level for testing
	level = new Level(30, 17);

	Tile* tile;
	for (int y = 0; y < level->GetHeight(); y++)
	{
		for (int x = 0; x < level->GetWidth(); x++)
		{
			
			if (y % 4 == 0)
				tile = tilesetManager->GetTile(33);
			else
				tile = tilesetManager->GetTile(18);

			if (x == 0 || y == 0 || x == level->GetWidth()-1 || y == level->GetHeight()-1)
				tile = tilesetManager->GetTile(5);
				

			level->AddTile(x, y, tile);
		}
	}
}
