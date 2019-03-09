#pragma once
#include <vector>
#include "tile.h"

class Level
{
public:
	Level(int width, int height);
	~Level();

	void AddTile(int x, int y, Tile* tile);
	Tile* GetTile(int x, int y);

	void LoadLevel();
	int GetWidth();
	int GetHeight();

private:
	std::vector<std::vector<Tile*>> map;

	int width, height;
	void SetDimensions(int width, int height);

};
