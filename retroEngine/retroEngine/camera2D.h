#pragma once
#include "raylib.h"

class Cam
{

public:
	Cam(int w, int h, float speed);
	~Cam();

	//Moves camera immediately to coordinates
	void Move(int x, int y);
	void MoveCenter(int x, int y);

	//Sets camera target
	void GoTo(int x, int y);
	void GoToCenter(int x, int y);

	//Updates camera position
	void Update();

	Vector2 GetPosition() { return { (float)position.x, (float)position.y }; }

	//Helper function for retreiving camera's offset from
	//nearest tile
	Vector2 GetOffset() { return { (float)((int)position.x % 16), (float)((int)position.y % 16) }; }

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
};
