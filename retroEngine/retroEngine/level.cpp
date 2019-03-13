#include "level.h"
using namespace std;
using namespace minijson;

#include <iostream>

Level::Level(std::string levelName)
{
	tilesetManager = new TilesetManager();

	Texture2D tileset1 = LoadTexture("../Assets/tileset1.png");
	SetTextureFilter(tileset1, 0);

	this->levelName = levelName;
	tilesetManager->AddTileset(tileset1);
	tilesetManager->MakeTiles();

	LoadLevel();
}

Level::~Level()
{
	std::cout << "Unload level" << endl;
	delete tilesetManager;

	map.clear();

}

void Level::AddTile(int x, int y, Tile * tile)
{
	if (map[x][y] != NULL)
	{
		delete map[x][y];
		map[x][y] = NULL;
	}

	map[x][y] = tile;
}

Tile * Level::GetTile(int x, int y)
{
	//Clamp array.
	if (x < 0)			x = 0;
	if (x > width - 1)  x = width - 1;
	if (y < 0)			y = 0;
	if (y > height - 1) y = height - 1;

	return map[x][y];
}

void Level::LoadLevel()
{
	string c = "../Assets/" + this->levelName + ".json";

	//Load file
	const char *fname = c.c_str();
	std::ifstream in(fname);
	minijson::istream_context ctx(in);

	int h = 0;
	int w = 0;
	std::vector<int> data;
	//Main parse
	parse_object(ctx, [&](const char *name, value v) {
		dispatch(name)
			<< "height" >> [&] { h = v.as_double(); }
			<< "width" >>  [&] { w = v.as_double(); }
			//Loop layers
			<< "layers" >> [&] {
			//Get layer.
			minijson::parse_array(ctx, [&](minijson::value va)
			{
				std::cout << "layer-> ";
				parse_object(ctx, [&](const char *name, value va) {
					dispatch(name)
						<< "id" >> [&] { std::cout << "ID: " << va.as_string() << endl; }
						//Get tiles.
						<< "data" >> [&] {
						parse_array(ctx, [&](value va) {
							data.push_back(va.as_double());							
							minijson::ignore(ctx);
						});
					};
					minijson::ignore(ctx);
				});
			});
			std::cout << endl;
			minijson::ignore(ctx);
		}

		<< any >> [&] { minijson::ignore(ctx); };
		
	});

	width = w;
	height = h;
	SetDimensions(width, height);

	int co = 0;
	for (int y = 0; y < GetHeight(); y++)
	{
		for (int x = 0; x < GetWidth(); x++)
		{
			int t = data[co]-1;
			co++;

			if (t >= 0) {
				AddTile(x, y, tilesetManager->GetTile(t));
			}
		}
	}
}

int Level::GetWidth()
{
	return width;
}

int Level::GetHeight()
{
	return height;
}

void Level::SetDimensions(int width, int height)
{
	//width are rows
	map.resize(width);

	for (int i = 0; i < width; i++)
	{
		map.at(i).resize(height, 0);
	}
}
