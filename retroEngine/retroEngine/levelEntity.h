#pragma once
#include "raylib.h"
#include "cameraView.h"

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

		void CreateCollider(bool isStatic);
		void CreateSprite();
		void DrawCollider(int x, int y);

		int x, y, width, height;



	private:

	};
}