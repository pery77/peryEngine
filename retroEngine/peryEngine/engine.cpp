#include "engine.h"

Vector2 ballPosition = { 0, 0 };
Vector2 MouseWorldPosition;


pery::Engine::Engine()
{

}

pery::Engine::~Engine()
{
	printf("Unload Engine");
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

	//Load first level.
	LoadLevel("monsterboy");
	
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
		RenderFrame();
		Update();
	}		

	CloseWindow();					// Close window and OpenGL context
}

void pery::Engine::ProcessInput()
{

	if (IsKeyDown(KEY_D)) ballPosition.x += 4;
	if (IsKeyDown(KEY_A)) ballPosition.x -= 4;
	if (IsKeyDown(KEY_W)) ballPosition.y -= 4;
	if (IsKeyDown(KEY_S)) ballPosition.y += 4;


	camera->GoTo(ballPosition.x, ballPosition.y);

	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
	{
		int MouseX = (int)MouseWorldPosition.x;
		int MouseY = (int)MouseWorldPosition.y;
		level->CreateBox( MouseX, MouseY);
	}

	if (IsKeyPressed(KEY_SPACE))
	{
		camera->GoTo(0, 0);
	}
	if (IsKeyPressed(KEY_ONE)) //Reload level
	{
		LoadLevel("test1");
	}
	if (IsKeyPressed(KEY_TWO)) //Reload level
	{
		LoadLevel("monsterboy");
	}
}

void pery::Engine::Update()
{
	camera->Update();
	level->World->Step(1 / 60.f, 8, 3); //Physics update (60fps)
}

void pery::Engine::LoadLevel(std::string name)
{
	if (level != NULL) level->~Level();
	
	level = new Level(name);

	if (camera != NULL) delete camera;
	camera = new CameraView(ScreenWidth, ScreenHeight, 1, level->GetWidth(), level->GetHeight(), level->GetTileSize());

}

//Draw loop.
void pery::Engine::RenderFrame()
{
	int camOffsetX, camOffsetY;
	Rectangle bounds = camera->GetTileBounds();
	camOffsetX = camera->GetOffset().x;
	camOffsetY = camera->GetOffset().y;

	float MouseX = (GetMousePosition().x * 1 / ScreenScale) + camera->GetPosition().x;
	float MouseY = (GetMousePosition().y * 1 / ScreenScale) + camera->GetPosition().y;

	MouseWorldPosition = { MouseX, MouseY };

	BeginDrawing();
	ClearBackground(BLANK);
	//Draw game to texture.
	BeginTextureMode(mainRender);
	//DrawBackground solid color.
	if (level->CurrentMap->MapLoaded.backgroundColor != "null")
	{
		DrawRectangle(0, 0, ScreenWidth, ScreenHeight, level->CurrentMap->MapLoaded.bgColor);
	}
	//Draw level
	for (int i = 0; i < level->GetLayers(); i++)
	{

		if (level->CurrentMap->MapLoaded.renderQueue[i].imageLayer.id != -1)
		{
			MapImageLayer * il = &level->CurrentMap->MapLoaded.renderQueue[i].imageLayer;

			if (il->visible)
			{
				DrawTexture(il->image.texture,
					il->offsetx - camera->GetPosition().x * il->speedX,
					il->offsety - camera->GetPosition().y * il->speedY, WHITE);
			}
		}
		if (level->CurrentMap->MapLoaded.renderQueue[i].layer.id != -1)
		{
			MapLayer * l = &level->CurrentMap->MapLoaded.renderQueue[i].layer;

			if (l->isImage && l->visible)
			{
				DrawTexture(l->targetTexture.texture,
					l->offsetx - camera->GetPosition().x * l->speedX,
					l->offsety - camera->GetPosition().y * l->speedY, WHITE);
			}
			else
			{
				if (l->visible)
				{

					//Draw tiles.
					for (int y = 0, tileY = bounds.y; y < bounds.height; y++, tileY++)
					{
						for (int x = 0, tileX = bounds.x; x < bounds.width; x++, tileX++)
						{

							Tile * t = level->GetTile(i, tileX, tileY);
							if (t == NULL) continue;
							t->Draw((x * t->tileWidth) - camOffsetX,
								(y * t->tileHeight) - camOffsetY);
							//DrawText(FormatText("%i", x*16-camOffsetX), x*16, y*16, 8, RED);

						}
					}
				}
			}
		}
	}

	bool c = false;
	Vector2 cursor = { ballPosition.x - camera->GetPosition().x + ScreenWidth * 0.3, ballPosition.y - camera->GetPosition().y + ScreenHeight * 0.5 };

	Color col = GREEN;
	if (c) col = RED;

	//DrawText(FormatText("%i,%i",(int)camera->GetPosition().x,(int)camera->GetPosition().y), cursor.x-10, cursor.y+10, 2, RED);
	DrawText(FormatText("%i,%i", (int)cursor.x, (int)cursor.y), cursor.x - 10, cursor.y + 10, 2, RED);
	DrawCircleV(cursor, 4, col);

	DrawText(FormatText("%i,%i", (int)MouseX, (int)MouseY),MouseX - camera->GetPosition().x,
		MouseY - camera->GetPosition().y, 2, RED);

	level->ProcessEntities(camera->GetPosition().x, camera->GetPosition().y);


	//End draw game in main texture.
	EndTextureMode();

	ClearBackground(BLACK);


	//Blend texture for postprocess effect.
	BeginBlendMode(1);
	//Draw main texture scaled to screen.
	DrawTexturePro(mainRender.texture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);
	//Draw glow frist pass (big glow effect)
	glow->BigGlow(mainRender.texture);
	DrawTexturePro(glow->BlurTexture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);
	//Draw glow second pass
	//glow->SetValues(.5, 1.1, 1.5);
	glow->SetValues(.99, 0.1, 0.1);
	glow->BlurTexture = glow->DrawGlow(mainRender.texture);
	DrawTexturePro(glow->BlurTexture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);
	//End draw main + postprocess 
	EndBlendMode();

	EndDrawing();
}