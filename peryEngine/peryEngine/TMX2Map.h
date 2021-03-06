#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <map>

#include "raylib.h"


#include "rapidxml\rapidxml.hpp"
#include "miniz\miniz.h"

namespace pery {

	//Objects
	struct MapObject {
		int id;
		std::string name;
		std::string type;

		int x;
		int y;
		int width;
		int height;

		bool visible;

	};

	//Tiles in tsx file.
	struct TSXTile
	{
		int id;

		struct AnimationFrame
		{
			int tileid;
			int duration;
		};

		std::vector<MapObject> objects;
		std::vector<AnimationFrame> frames;
	};

	//Struct image
	struct Image
	{
		std::string source;
		int width;
		int height;

		Texture2D texture;
	};

	//Tsx file tileset
	struct TSXTileSet
	{
		//Tileset attibuttes
		std::string name;
		std::string version;

		int tileWidth;
		int tileHeight;
		int tileCount;
		int columns;
		int rows;

		Image image;

		//properties dictionary
		std::map<std::string, std::string> properties;
		std::vector < TSXTile > tiles;

	};

	//Tileset structure
	struct MapTileset
	{
		int firstgid;
		std::string source;

		TSXTileSet tileset;

	};


	struct MapObjectGroup
	{
		int id;
		std::string name;

		bool visible;
	};

	//Data node
	struct Data
	{
		std::string encoding;
		std::string compression;

		//Data string
		std::string content;
	};

	//Group (layers and objectsgroup conainer )
	struct Group
	{
		std::string name;
		int id;

		bool visible;
	};

	struct MapLayer
	{
		//Attributes
		int id           = -1;
		int width        = -1;
		int height       = -1;

		std::string name = "null";

		bool visible = true;

		//Data child
		Data data;

		//Store tile IDs
		std::vector<std::uint32_t> IDs;

		//properties dictionary
		std::map<std::string, std::string> properties;

		//Fields for image mode
		bool isImage;
		RenderTexture2D targetTexture;
		float speedX = 1;
		float speedY = 1;
		int offsetx = 0;
		int offsety = 0;

	};

	struct MapImageLayer
	{
		int id      = -1;
		int offsetx = 0;
		int offsety = 0;

		std::string	name = "null";

		Image image;
		bool visible = true;

		//properties dictionary
		std::map<std::string, std::string> properties;

		//Velocity relative to camera, for paralax effect.
		float speedX = 1;
		float speedY = 1;

	};

	struct RenderQueue
	{
		MapLayer      layer;
		MapImageLayer imageLayer;
	};

	//Map structure
	struct Map
	{
		//Map attibuttes
		std::string name	        = "null";
		std::string version         = "null";
		std::string orientation     = "null";
		std::string backgroundColor = "null";
		int width      = -1;
		int height     = -1;
		int tileWidth  = -1;
		int tileHeight = -1;

		//Child nodes
		std::vector<MapTileset>    tilesets = {};
		std::vector<MapObject>     objects  = {};

		std::vector<RenderQueue> renderQueue = {};

		//Background color
		Color bgColor;
	};

	class TMX2Map {

	public:

		TMX2Map(std::string TMXName);
		~TMX2Map();

		Map MapLoaded;

		//Decompress zlib string.
		void DecompressLayerData(MapLayer * layer)
		{
			//Get data
			std::string data = layer->data.content.c_str();

			//using a string stream we can remove whitespace such as tabs
			std::stringstream ss;
			ss << data;
			ss >> data;

			//Decode data
			data = base64_decode(data);

			//Get total layer tiles
			int tileCount = layer->width * layer->height;

			std::size_t expectedSize = tileCount * 4; //4 bytes per tile
			std::vector<unsigned char> byteData;
			byteData.reserve(expectedSize);

			std::size_t dataSize = data.length() * sizeof(unsigned char);
			decompress(data.c_str(), byteData, dataSize, expectedSize);

			byteData.insert(byteData.end(), data.begin(), data.end());

			layer->IDs.reserve(tileCount);

			for (auto i = 0u; i < expectedSize - 3u; i += 4u)
			{
				std::uint32_t id = byteData[i] | byteData[i + 1] << 8 | byteData[i + 2] << 16 | byteData[i + 3] << 24;
				layer->IDs.push_back(id);
			}
		}

		void ShowMapInfo();

		const char * getValue(rapidxml::xml_node<char> * node, const char * name);

	private:

		//Find a string value in a map and return it.
		std::string findString(std::map<std::string, std::string> * map, std::string key)
		{
			std::map<std::string, std::string>::iterator it;
			it = map->find(key);
			if (it == map->end()) return "null"; else return it->second;
		}

		//Find a in value in a map and return it.
		int findInt(std::map<std::string, std::string> * map, std::string key)
		{
			std::map<std::string, std::string>::iterator it;
			it = map->find(key);
			if (it == map->end()) return NAN; else return atoi((it->second).c_str());
		}
		//Find a float value in a map and return it.
		float findFloat(std::map<std::string, std::string> * map, std::string key)
		{
			std::map<std::string, std::string>::iterator it;
			it = map->find(key);
			if (it == map->end()) return NAN; else return atof((it->second).c_str());
		}

		//Find a bool value in a map and return it.
		bool findBool(std::map<std::string, std::string> * map, std::string key)
		{
			std::map<std::string, std::string>::iterator it;
			it = map->find(key);
			if (it == map->end() || (it->second) != "true" ) return false; else return true;
		}

		//Convert hex string to color
		Color hex2Color(std::string str)
		{
			//removes preceding #
			if (str[0] == '#')
				str.erase(0, 1);

			if (str.size() == 6 || str.size() == 8)
			{
				unsigned int value, r, g, b;
				unsigned int a = 255;

				std::stringstream input(str);
				input >> std::hex >> value;

				r = (value >> 16) & 0xff;
				g = (value >> 8) & 0xff;
				b = value & 0xff;

				if (str.size() == 8)
				{
					a = (value >> 24) & 0xff;
				}

				return { (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
			}
		}

		//Decode and Decompress algorithm addapted from:
		//https://github.com/fallahn/tmxlite

		//Decode base64 string
		static inline std::string base64_decode(std::string const& encoded_string)
		{
			static const std::string base64_chars =
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz"
				"0123456789+/";

			//Check if a (unsigned char) is a base64
			std::function<bool(unsigned char)> is_base64 = [](unsigned char c)->bool
			{
				return (isalnum(c) || (c == '+') || (c == '/'));
			};

			//Set variables
			auto in_len = encoded_string.size();
			int i = 0;
			int j = 0;
			int in_ = 0;
			unsigned char char_array_4[4], char_array_3[3];
			std::string ret;

			//Loop string
			while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
			{
				char_array_4[i++] = encoded_string[in_]; in_++;
				if (i == 4)
				{
					for (i = 0; i < 4; i++)
					{
						char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));
					}
					char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
					char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
					char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

					for (i = 0; (i < 3); i++)
					{
						ret += char_array_3[i];
					}
					i = 0;
				}
			}//End Loop

			if (i)
			{
				for (j = i; j < 4; j++)
				{
					char_array_4[j] = 0;
				}

				for (j = 0; j < 4; j++)
				{
					char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));
				}

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (j = 0; (j < i - 1); j++)
				{
					ret += char_array_3[j];
				}
			}

			return ret;
		}

		//Decompress string
		static inline bool decompress(const char* source, std::vector<unsigned char>& dest, std::size_t inSize, std::size_t expectedSize)
		{
			if (!source)
			{
				printf("Input string is empty, decompression failed.");
				return false;
			}

			int currentSize = static_cast<int>(expectedSize);
			std::vector<unsigned char> byteArray(expectedSize / sizeof(unsigned char));
			z_stream stream;
			stream.zalloc = Z_NULL;
			stream.zfree = Z_NULL;
			stream.opaque = Z_NULL;
			stream.next_in = (Bytef*)source;
			stream.avail_in = static_cast<unsigned int>(inSize);
			stream.next_out = (Bytef*)byteArray.data();
			stream.avail_out = static_cast<unsigned int>(expectedSize);

			if (inflateInit(&stream) != Z_OK)
			{
				printf("inflate init failed");
				return false;
			}

			int result = 0;
			do
			{
				result = inflate(&stream, Z_SYNC_FLUSH);

				switch (result)
				{
				case Z_NEED_DICT:
				case Z_STREAM_ERROR:
					result = Z_DATA_ERROR;
				case Z_DATA_ERROR:
					printf("If using gzip compression try using zlib instead");
				case Z_MEM_ERROR:
					inflateEnd(&stream);
					printf(std::to_string(result).c_str());
					return false;
				}

				if (result != Z_STREAM_END)
				{
					int oldSize = currentSize;
					currentSize *= 2;
					std::vector<unsigned char> newArray(currentSize / sizeof(unsigned char));
					std::memcpy(newArray.data(), byteArray.data(), currentSize / 2);
					byteArray = std::move(newArray);

					stream.next_out = (Bytef*)(byteArray.data() + oldSize);
					stream.avail_out = oldSize;

				}
			} while (result != Z_STREAM_END);

			if (stream.avail_in != 0)
			{
				printf("stream.avail_in is 0");
				printf("zlib decompression failed.");
				return false;
			}

			const int outSize = currentSize - stream.avail_out;
			inflateEnd(&stream);

			std::vector<unsigned char> newArray(outSize / sizeof(unsigned char));
			std::memcpy(newArray.data(), byteArray.data(), outSize);
			byteArray = std::move(newArray);

			//copy bytes to vector
			dest.insert(dest.begin(), byteArray.begin(), byteArray.end());

			return true;
		}

		void parseTMX(rapidxml::xml_node<>* node, int indent = 0, Group * parentGroup = NULL);
		void parseTSX(rapidxml::xml_node<>* node, int currentTileset, int indent = 0,   Group * parentGroup = NULL);
		void processProperties(rapidxml::xml_node<>* node, std::map<std::string, std::string> * properties);
	};
}