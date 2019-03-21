#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <map>


#include "raylib.h"

#include "tools.h"
#include "rapidxml\rapidxml.hpp"
#include "miniz\miniz.h"

namespace pery {

	struct TSXTile
	{
		int id;
		std::map<std::string, std::string> properties;
	};

	//Struct image
	struct Image
	{
		std::string source;
		int width;
		int height;
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
		Texture2D texture;
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

		struct Objects {
			int id;
			std::string name;
			std::string type;
			
			int x;
			int y;
			int widht;
			int height;

			bool visible;

		};

		std::vector<Objects> objects;
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
	struct Group
	{
		std::string name;
		int id;

		bool visible;

	};

	struct MapLayer
	{
		//Attributes
		int id;
		std::string name;
		int width;
		int height;

		bool visible;

		//Data child
		Data data;

		//Store tile IDs
		std::vector<std::uint32_t> IDs;

		//properties dictionary
		std::map<std::string, std::string> properties;

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
		std::vector<MapTileset> tilesets;
		std::vector<MapLayer>   layers;

		std::vector<MapObjectGroup> objectgroup;
	};





	class TMX2Map {

	public:

		TMX2Map(std::string TMXName);
		~TMX2Map();

		Map MapLoaded;

		//Decompress zlib string.
		void DecompressLayerData(int layer)
		{
			//Get data
			std::string data = MapLoaded.layers[layer].data.content.c_str();

			//using a string stream we can remove whitespace such as tabs
			std::stringstream ss;
			ss << data;
			ss >> data;

			//Decode data
			data = base64_decode(data);

			//Get total layer tiles
			int tileCount = MapLoaded.layers[layer].width * MapLoaded.layers[layer].height;

			std::size_t expectedSize = tileCount * 4; //4 bytes per tile
			std::vector<unsigned char> byteData;
			byteData.reserve(expectedSize);

			std::size_t dataSize = data.length() * sizeof(unsigned char);
			decompress(data.c_str(), byteData, dataSize, expectedSize);

			byteData.insert(byteData.end(), data.begin(), data.end());

			MapLoaded.layers[layer].IDs.reserve(tileCount);

			for (auto i = 0u; i < expectedSize - 3u; i += 4u)
			{
				std::uint32_t id = byteData[i] | byteData[i + 1] << 8 | byteData[i + 2] << 16 | byteData[i + 3] << 24;
				MapLoaded.layers[layer].IDs.push_back(id);
			}
		}

		void ShowMapInfo();

		const char * getValue(rapidxml::xml_node<char> * node, const char * name);

	private:

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
				LOG("Input string is empty, decompression failed.");
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
				LOG("inflate init failed");
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
					LOG("If using gzip compression try using zlib instead");
				case Z_MEM_ERROR:
					inflateEnd(&stream);
					LOG(std::to_string(result));
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
				LOG("stream.avail_in is 0");
				LOG("zlib decompression failed.");
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

		void processMap(rapidxml::xml_node<char> * parentNode);
		void processTilesets(rapidxml::xml_node<char> * parentNode);
		void processLayers(rapidxml::xml_node<char> * parentNode, Group * parentGroup);
		void processGroup(rapidxml::xml_node<char> * parentNode, Group * parentGroup);
		void processObjectGroup(rapidxml::xml_node<char> * parentNode, Group * parentGroup);
};
}