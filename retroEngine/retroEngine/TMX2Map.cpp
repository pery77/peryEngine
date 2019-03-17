#include "TMX2Map.h"

pery::TMX2Map::TMX2Map(std::string TMXName)
{
	//Setup variables
	MapLoaded.name = TMXName;
	rapidxml::xml_document<> tmxDoc;

	//Load file
	std::string st = "../Assets/Maps/" + TMXName + ".tmx";
	const char *fname = st.c_str();
	std::ifstream tmxFile(fname);

	//File to buffer
	std::stringstream buffer;
	buffer << tmxFile.rdbuf();

	//Close file
	tmxFile.close();

	//Buffer to string
	std::string tmxFileContent(buffer.str());

	// String to char array, safe-to-modify copy of tmxFileContent
	std::vector<char> tmxFileContentCopy(tmxFileContent.begin(), tmxFileContent.end());
	tmxFileContentCopy.push_back('\0');

	//XML Parse
	tmxDoc.parse<rapidxml::parse_no_data_nodes>(&tmxFileContentCopy[0]);

	//--------------------------------------
	//Main node (Map node)
	rapidxml::xml_node<char> * mapNode;
	mapNode = tmxDoc.first_node("map");

	//Store attributes in CurrentMap struct.
	MapLoaded.version = mapNode ->first_attribute("version")->value();
	MapLoaded.orientation = mapNode->first_attribute("orientation")->value();

	//atoi string to interger
	MapLoaded.width = atoi(mapNode->first_attribute("width")->value());
	MapLoaded.height = atoi(mapNode->first_attribute("height")->value());

	MapLoaded.tileWidth = atoi(mapNode->first_attribute("tilewidth")->value());
	MapLoaded.tileHeight = atoi(mapNode->first_attribute("tileheight")->value());
	
	//Load backgroundcolor only if node exist
	if(mapNode->first_attribute("backgroundcolor")!=0)
		MapLoaded.backgroundColor = mapNode->first_attribute("backgroundcolor")->value();

	//--------------------------------------
	//Tileset Attributtes
	rapidxml::xml_node<char> * currentTileSetNode;
	currentTileSetNode = mapNode->first_node("tileset");

	//Loop tilesets while not null.
	while (currentTileSetNode != NULL)
	{
		//Create a tileset and load attributes.
		MapTileset t;

		t.firstgid = atoi(currentTileSetNode->first_attribute("firstgid")->value());
		t.source   = currentTileSetNode->first_attribute("source")->value();

		//Add tileset to CurrentMap tilesets array.
		MapLoaded.tilesets.push_back(t);

		//Next tile node
		currentTileSetNode = currentTileSetNode->next_sibling("tileset");
	}

	//--------------------------------------
	//Layers
	rapidxml::xml_node<char> * currentLayerNode;
	currentLayerNode = mapNode->first_node("layer");
	while (currentLayerNode != NULL)
	{
		//Create a Layer and load attributes.
		MapLayer l;

		l.name   = currentLayerNode->first_attribute("name")->value();

		l.id     = atoi(currentLayerNode->first_attribute("id")->value());
		l.width  = atoi(currentLayerNode->first_attribute("width")->value());
		l.height = atoi(currentLayerNode->first_attribute("height")->value());

		//Data
		rapidxml::xml_node<char> * dataNode;
		dataNode = currentLayerNode->first_node("data");

		Data d;
		if (dataNode->first_attribute("encoding") != 0)
			d.encoding    = dataNode->first_attribute("encoding")->value();
		if (dataNode->first_attribute("compression") != 0)
			d.compression = dataNode->first_attribute("compression")->value();

		d.content = dataNode->value();

		//Put data inside layer
		l.data = d;
		
		//Add tileset to CurrentMap tilesets array.
		MapLoaded.layers.push_back(l);

		//Next tile node
		currentLayerNode = currentLayerNode->next_sibling("layer");
	}


	//--------------------------------------
	//Objects

	//Decompress all layer data.
	for (int i = 0; i < MapLoaded.layers.size(); i++)
	{
		DecompressLayerData(i);
	}

	//Load TXS (tilesets files)
	for (int ts = 0; ts < MapLoaded.tilesets.size(); ts++)
	{
		rapidxml::xml_document<> tsxDoc;

		//Load file
		std::string st = "../Assets/Tilesets/" + MapLoaded.tilesets[ts].source;
		const char *fname = st.c_str();
		std::ifstream tsxFile(fname);

		//File to buffer
		std::stringstream buffer;
		buffer << tsxFile.rdbuf();

		//Close file
		tsxFile.close();

		//Buffer to string
		std::string tsxFileContent(buffer.str());

		// String to char array, safe-to-modify copy of tmxFileContent
		std::vector<char> tsxFileContentCopy(tsxFileContent.begin(), tsxFileContent.end());
		tsxFileContentCopy.push_back('\0');

		//XML Parse
		tsxDoc.parse<rapidxml::parse_no_data_nodes>(&tsxFileContentCopy[0]);
		
		//Main node (Map node)
		rapidxml::xml_node<char> * tilesetNode;
		tilesetNode = tsxDoc.first_node("tileset");

		std::cout << buffer.str();
		std::cout << tilesetNode->value();
		
	}

}

pery::TMX2Map::~TMX2Map()
{
}

//Show map	
void pery::TMX2Map::ShowMapInfo()
{
	std::cout << "[MAP INFO]" << std::endl;

	//Show map info
	std::cout << "Map Name: " << MapLoaded.name <<
		" | Version: " << MapLoaded.version <<
		" | Orientation: " << MapLoaded.orientation << std::endl;

	std::cout << "Map size: [w:" << MapLoaded.width << ",h:" << MapLoaded.height << "]" << std::endl;
	std::cout << "Tile size: [w:" << MapLoaded.tileWidth << ",h:" << MapLoaded.tileHeight << "]" << std::endl;
	std::cout << "Background color: " << MapLoaded.backgroundColor << std::endl;
	std::cout << std::endl;

	std::cout << MapLoaded.tilesets.size() << " tileset(s)" << std::endl;
	std::cout << std::endl;

	//Show tilesets info
	for (int i = 0; i < MapLoaded.tilesets.size(); i++)
	{
		std::cout << "    ";
		std::cout << "firstgid: " << MapLoaded.tilesets[i].firstgid <<
			" , source: " << MapLoaded.tilesets[i].source << std::endl;

	}
	std::cout << std::endl;

	//Show layers info
	std::cout << MapLoaded.layers.size() << " layer(s)" << std::endl;
	std::cout << std::endl;
	for (int i = 0; i < MapLoaded.layers.size(); i++)
	{
		std::cout << "    Layer: " << i << std::endl;
		std::cout << "    ";
		std::cout << "Name: " << MapLoaded.layers[i].name <<
			" | ID: " << MapLoaded.layers[i].id << std::endl;
		std::cout << "    ";
		std::cout << "Layer size: [w:" << MapLoaded.layers[i].width << ",h:" << MapLoaded.layers[i].height << "]" << std::endl;
		std::cout << "    ";
		std::cout << "Layer Data-> ";
		std::cout << "Encoding: " << MapLoaded.layers[i].data.encoding <<
			" ,Compression: " << MapLoaded.layers[i].data.compression << std::endl;
		std::cout << "    ";
		std::cout << MapLoaded.layers[i].IDs.size() << " tiles in layer. >>> [ ";

		//First 16 Tiles IDs.
		if (MapLoaded.layers[i].IDs.size() > 15)
		{
			for (int id = 0; id < 15; id++) {
				std::cout << MapLoaded.layers[i].IDs[id] << ", ";
			}
			std::cout << "... ]" << std::endl;
			std::cout << std::endl;
		}
	}

}