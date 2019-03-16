#pragma once
#include <vector>
#include <iostream>
#include <fstream>

#include <sstream>
//#include <cstring>

#include "rapidxml\rapidxml.hpp"

namespace pery {

	//Tileset structure
	struct Tileset
	{
		int firstgid;
		std::string source;
	};

	struct Data
	{
		std::string encoding;
		std::string compression;

		std::string content;
	};

	struct Layer
	{
		//Attributes
		int id;
		std::string name;
		int width;
		int height;

		//Data child
		Data data;
	};

	//Map structure
	struct Map
	{
		//Map attibuttes
		std::string name;
		std::string version;
		std::string orientation;
		std::string backgroundColor;
		int width;
		int height;
		int tileWidth;
		int tileHeight;

		//Child nodes
		std::vector<Tileset> tilesets;
		std::vector<Layer>   layers;
	};



	class TMX2Map {
	public:
		TMX2Map(std::string TMXName);
		~TMX2Map();

		Map CurrentMap;

	private:

	};
}