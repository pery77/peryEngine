#pragma once

#include "sprite.h"
#include "Box2D\Box2D.h"


namespace pery
{
	class LevelEntity
	{
	public:

		LevelEntity(b2World* &World, int x, int y, int width, int height);
		~LevelEntity();

		b2World* World;
		b2Body*  Body;

		Sprite* sprite;

		void CreateCollider(bool isStatic);
		void CreateSprite(TSXTileSet & tileset, int id);
		void DrawCollider(int x, int y);
		void DrawSprite(int x, int y);

		int x, y, width, height;



	private:

	};
}