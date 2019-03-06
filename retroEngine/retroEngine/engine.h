#pragma once
#include "raylib.h"
#include "glow.h"

class Engine {

public:

	int screenWidth  = 480;
	int screenHeight = 272;
	int screenScale  = 2;

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

	RenderTexture2D* mainRender;
	Glow* glow;

	Rectangle sourceRec = { 0.0f, 0.0f, (float)screenWidth, -(float)screenHeight };
	Rectangle scaledRec = { 0.0f, 0.0f, (float)screenWidth * (float)screenScale, (float)screenHeight * (float)screenScale };

};