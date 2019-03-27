#include "level.h"
using namespace std;

#include <iostream>

//Collision 
void pery::Level::CreateBoxCollider(int x, int y, int w, int h)
{
	w = w * 0.5f;
	h = h * 0.5f;

	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(x + w, y + h);
	BodyDef.type = b2_staticBody;

	b2Body* Body = World->CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox(w, h); // Creates a box shape. Divide your desired width and height by 2.
	
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;  // Sets the density of the body
	FixtureDef.shape = &Shape; // Sets the shape
	
	Body->CreateFixture(&FixtureDef); // Apply the fixture definition
}

void pery::Level::CreateBox(int x, int y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(x, y);
	BodyDef.type = b2_dynamicBody;

	b2Body* Body = World->CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox(8, 8);

	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Shape;

	Body->CreateFixture(&FixtureDef);
}

pery::Level::Level(std::string levelName)
{

	CurrentMap = new TMX2Map (levelName);
	CurrentMap->ShowMapInfo();

	this->levelName = levelName;

	tilesetManager = new TilesetManager();	

	for (int ts = 0; ts < CurrentMap->MapLoaded.tilesets.size(); ts ++)
	{
		tilesetManager->AddTileset(CurrentMap->MapLoaded.tilesets[ts].tileset);
	}

	width    = CurrentMap->MapLoaded.width;
	height   = CurrentMap->MapLoaded.height;
	layers   = CurrentMap->MapLoaded.renderQueue.size();
	tileSize = CurrentMap->MapLoaded.tileHeight;

	SetDimensions(CurrentMap->MapLoaded.renderQueue.size(), width, height);

	//Setup phyisics
	b2Vec2 Gravity(0, 9.8 * pixelsPerMeter);
	World = new b2World(Gravity);

	LoadLevel();

}

pery::Level::~Level()
{
	printf("Unload level");
	for (int i = 0; i < layers; i++)
	{
		UnloadRenderTexture(CurrentMap->MapLoaded.renderQueue[i].layer.targetTexture);
	}
	delete tilesetManager;
	delete CurrentMap;

	map.clear();

	delete World;

}

void pery::Level::AddTile(int x, int y, Tile * tile, int layer)
{
	if (map[layer][x][y] != NULL)
	{
		delete map[layer][x][y];
		map[layer][x][y] = NULL;
	}

	map[layer][x][y] = tile;
}

pery::Tile * pery::Level::GetTile(int layer, int x, int y)
{

	if ( map.size() == 0) return nullptr;

 	//Clamp array.
	if (x < 0)			x = 0;
	if (x > width - 1)  x = width - 1;
	if (y < 0)			y = 0;
	if (y > height - 1) y = height - 1;

	return map[layer][x][y];
}

void pery::Level::LoadLevel()
{

	for (int i = 0; i < CurrentMap->MapLoaded.renderQueue.size(); i++)
	{
		if (CurrentMap->MapLoaded.renderQueue[i].imageLayer.id != -1)
		{
			const char * imageSource = CurrentMap->MapLoaded.renderQueue[i].imageLayer.image.source.c_str();
			CurrentMap->MapLoaded.renderQueue[i].imageLayer.image.texture = LoadTexture(imageSource);
		}
		if (CurrentMap->MapLoaded.renderQueue[i].layer.id != -1)
		{

			int co = 0;
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					int t = CurrentMap->MapLoaded.renderQueue[i].layer.IDs[co] - 1;
					co++;
					if (t >= 0) {
						AddTile(x, y, tilesetManager->GetTile(t), i);
					}
				}
			}
			//Render layer into texture.
			if (CurrentMap->MapLoaded.renderQueue[i].layer.isImage)
			{
				int w = width * tileSize;
				int h = height * tileSize;
				
				RenderTexture2D target = LoadRenderTexture(w, h);
				SetTextureFilter(target.texture, 0);

				CurrentMap->MapLoaded.renderQueue[i].layer.targetTexture = LoadRenderTexture(w, h);
				SetTextureFilter(CurrentMap->MapLoaded.renderQueue[i].layer.targetTexture.texture, 0);

				BeginDrawing();

					BeginTextureMode(target);
						ClearBackground(BLANK);
						//Draw tiles.
						for (int y = 0, tileY = 0; y < height; y++, tileY++)
						{
							for (int x = 0, tileX = 0; x < width; x++, tileX++)
							{		
								Tile * t = GetTile(i, tileX, tileY);
								if (t == NULL) continue;
								t->Draw((x * t->tileWidth),
									(y * t->tileHeight));
							}
						}
					EndTextureMode();

					BeginTextureMode(CurrentMap->MapLoaded.renderQueue[i].layer.targetTexture);
						ClearBackground(BLANK);
						DrawTexture(target.texture, 0, 0, WHITE);
					EndTextureMode();

				EndDrawing();

				UnloadRenderTexture(target);

			}


			
		}
	}

	for (int i = 0; i < CurrentMap->MapLoaded.objects.size(); i++)
	{
		MapObject * o = &CurrentMap->MapLoaded.objects[i];
		if (o->name == "collider")
		{
			Rectangle r = { (float)o->x, (float)o->y, (float)o->width, (float)o->height };
			Colliders.push_back(r);		
			CreateBoxCollider( o->x, o->y, o->width, o->height);
		}

	}

}

void pery::Level::SetDimensions(int layers, int width, int height)
{

	map.resize(layers);
	for (int i = 0; i < layers; i++)
	{
		map[i].resize(width);
		for (int j = 0; j < width; j++)
		{
			map[i][j].resize(height);
		}
	}

}
