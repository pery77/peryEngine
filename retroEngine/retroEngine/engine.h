#pragma once
#include "raylib.h"
#include "glow.h"
#include "level.h"
#include "cameraView.h"


namespace pery {

	class Engine {

	public:
		//Screen constant values.
		const int ScreenWidth = 480;
		const int ScreenHeight = 272;
		const int ScreenScale = 3;

		Engine();
		~Engine();

		//Start the engine
		void Go();


	private:

		void Init();		 //Initialize engine.
		void MainLoop();	 //Main loop
		void RenderFrame();  //Draw loop
		void ProcessInput(); //Keys events loop.
		void Update();		 //Update loop.

		void LoadLevel(std::string name);

		//Main texture for render
		RenderTexture2D mainRender;
		Glow* glow; //Effect, post process.

		//Rectangles for render main texture in screen or window.
		Rectangle sourceRec = { 0.0f, 0.0f, (float)ScreenWidth, -(float)ScreenHeight };
		Rectangle scaledRec = { 0.0f, 0.0f, (float)ScreenWidth * (float)ScreenScale, (float)ScreenHeight * (float)ScreenScale };

		//Current level
		Level* level;
		//Current camera
		CameraView* camera;
	};
}