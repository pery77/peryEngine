#pragma once
#include "raylib.h"
#include <math.h> //For sqrt

namespace pery
{

	class CameraView
	{
		
	public:
		CameraView(int w, int h, float speed, int levelWidth, int levelHeight, int tileSize);
		~CameraView();

		//Moves camera immediately to x, y
		void Move(int x, int y);
		//void MoveCenter(int x, int y);

		//Sets camera target
		void GoTo(int x, int y);

		//Updates camera position
		void Update();

		int TileSize = 16;
		Vector2 GetPosition() { return { (float)position.x, (float)position.y }; }

		//Helper function for retreiving camera's offset from
		//nearest tile
		Vector2 GetOffset() { return { (float)((int)position.x % TileSize), (float)((int)position.y % TileSize) }; }

		//Helper function for retreiving a rectangle defining
		//which tiles are visible through camera
		Rectangle GetTileBounds();

	private:
		//Absolute position of camera (number of
		//pixels from origin of level map)
		Vector2 position;

		//Target position camera is moving towards
		Vector2 target;

		//Size of camera
		Vector2 size;

		//Speed of camera, a value between 0.0 and 1.0
		float speed;

		//Prevent camera go out of map.
		int stopX;
		int stopY;
	};
}