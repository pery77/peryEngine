#include "engine.h"
#include "TMX2Map.h"
#include <iostream>
#include "miniz\miniz.h"

int main(int argc, char* argv[])
{
	//TMXMap map("../Assets/test.tmx");
	//LOG(map.getLayer(0).getData().getEncodedData());

	pery::TMX2Map map("test");

	LOG(map.CurrentMap.name);
	LOG(map.CurrentMap.version);
	LOG(map.CurrentMap.orientation);
	LOG(map.CurrentMap.height);
	LOG(map.CurrentMap.width);
	LOG(map.CurrentMap.tileHeight);
	LOG(map.CurrentMap.tileWidth);
	LOG(map.CurrentMap.backgroundColor);
	LOG("-----------");
	for (int i = 0; i < map.CurrentMap.tilesets.size(); i++)
	{
		LOG("Tileset->");
		LOG(map.CurrentMap.tilesets[i].firstgid);
		LOG(map.CurrentMap.tilesets[i].source);
	}
	LOG("-----------");
	LOG("-----------");
	for (int i = 0; i < map.CurrentMap.layers.size(); i++)
	{
		LOG("Layer->");
		LOG(map.CurrentMap.layers[i].name);
		LOG(map.CurrentMap.layers[i].id);
		LOG(map.CurrentMap.layers[i].width);
		LOG(map.CurrentMap.layers[i].height);

		LOG("Layer Data->");
		LOG(map.CurrentMap.layers[i].data.encoding);
		LOG(map.CurrentMap.layers[i].data.compression);
		mz_streamp st;
		std::cout << map.CurrentMap.layers[i].data.content << std::endl;
		LOG("_");
	}


	//Create engine and start.
	pery::Engine* engine = new pery::Engine();
//	engine->Go();

	delete engine;  //Deallocate engine from memory

#ifdef _DEBUG
	LOG("Press ENTER to exit.");
	std::cin.ignore(1); //Wait for enter key in debug mode.
#endif
	
	return 0;		//Exit program
}