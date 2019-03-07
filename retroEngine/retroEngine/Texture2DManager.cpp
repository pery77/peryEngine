#include "Texture2DManager.h"

Texture2DManager::Texture2DManager()
{
}

Texture2DManager::~Texture2DManager()
{
}

void Texture2DManager::AddTexture(Texture2D & texture)
{
	textureList.push_back(texture);
}

Texture2D & Texture2DManager::GetTexture(int index)
{
	return textureList[index];
}
