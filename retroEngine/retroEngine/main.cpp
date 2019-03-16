#include "engine.h"
#include "TMX2Map.h"
#include <iostream>
#include "miniz\miniz.h"

#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <algorithm>

static inline std::string base64_decode(std::string const& encoded_string)
{
	static const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	std::function<bool(unsigned char)> is_base64 =
		[](unsigned char c)->bool
	{
		return (isalnum(c) || (c == '+') || (c == '/'));
	};

	auto in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

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
	}

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

bool decompress(const char* source, std::vector<unsigned char>& dest, std::size_t inSize, std::size_t expectedSize)
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

	//we'd prefer to use inflateInit2 but it appears 
	//to be incorrect in miniz. This is fine for zlib
	//compressed data, but gzip compressed streams
	//will fail to inflate. IMO still preferable to
	//trying to build/link zlib
	if (inflateInit(&stream/*, 15 + 32*/) != Z_OK)
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

		std::string data = map.CurrentMap.layers[i].data.content.c_str();
		

		
		//using a string stream we can remove whitespace such as tabs
		std::stringstream ss;
		ss << data;
		ss >> data;

		//char *b = &data[0u];
		//std::cout << b << std::endl;

		data = base64_decode(data);
		int m_tileCount = map.CurrentMap.layers[i].width*map.CurrentMap.layers[i].height;
		std::size_t expectedSize =  m_tileCount * 4; //4 bytes per tile
		std::vector<unsigned char> byteData;
		byteData.reserve(expectedSize);

		std::size_t dataSize = data.length() * sizeof(unsigned char);
		decompress(data.c_str(), byteData, dataSize, expectedSize);

		byteData.insert(byteData.end(), data.begin(), data.end());

		std::vector<std::uint32_t> IDs;
		IDs.reserve(m_tileCount);

		for (auto i = 0u; i < expectedSize - 3u; i += 4u)
		{
			std::uint32_t id = byteData[i] | byteData[i + 1] << 8 | byteData[i + 2] << 16 | byteData[i + 3] << 24;
			IDs.push_back(id);
		}

		for (int i = 0; i < IDs.size(); i++)
		{
			std::cout << IDs[i] << ",";
		}

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