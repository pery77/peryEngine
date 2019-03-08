#pragma once
#include "raylib.h"
#include "glow.h"
#include "tilesetManager.h"

class Engine {

public:

	const int ScreenWidth  = 480;
	const int ScreenHeight = 272;
	const int ScreenScale  = 2;

	Engine();
	~Engine();

	//Start the engine
	void Go();

private:

	void Init();
	void MainLoop();
	void RenderFrame();
	void ProcessInput();
	void Update();

	RenderTexture2D mainRender;
	Glow* glow;

	Rectangle sourceRec = { 0.0f, 0.0f, (float)ScreenWidth, -(float)ScreenHeight };
	Rectangle scaledRec = { 0.0f, 0.0f, (float)ScreenWidth * (float)ScreenScale, (float)ScreenHeight * (float)ScreenScale };

	void LoadImages();

	TilesetManager tilesetManager;

};