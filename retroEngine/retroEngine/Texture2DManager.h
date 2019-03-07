#pragma once
#include "raylib.h"
#include <vector>
#include "tile.h"

class Texture2DManager {

public:

	Texture2DManager();
	~Texture2DManager();

	void AddTexture(Texture2D& texture);
	Texture2D& GetTexture(int index);


private:

	std::vector <Texture2D> textureList;
};