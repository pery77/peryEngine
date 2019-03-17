#include "TMX2Map.h"

pery::TMX2Map::TMX2Map(std::string TMXName)
{
	//Setup variables
	CurrentMap.name = TMXName;
	rapidxml::xml_document<> tmxDoc;

	//Load file
	std::string st = "../Assets/" + TMXName + ".tmx";
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
	CurrentMap.version = mapNode ->first_attribute("version")->value();
	CurrentMap.orientation = mapNode->first_attribute("orientation")->value();

	//atoi string to interger
	CurrentMap.width = atoi(mapNode->first_attribute("width")->value());
	CurrentMap.height = atoi(mapNode->first_attribute("height")->value());

	CurrentMap.tileWidth = atoi(mapNode->first_attribute("tilewidth")->value());
	CurrentMap.tileHeight = atoi(mapNode->first_attribute("tileheight")->value());
	
	//Load backgroundcolor only if node exist
	if(mapNode->first_attribute("backgroundcolor")!=0)
		CurrentMap.backgroundColor = mapNode->first_attribute("backgroundcolor")->value();

	//--------------------------------------
	//Tileset Attributtes
	rapidxml::xml_node<char> * currentTileSetNode;
	currentTileSetNode = mapNode->first_node("tileset");

	//Loop tilesets while not null.
	while (currentTileSetNode != NULL)
	{
		//Create a tileset and load attributes.
		Tileset t;

		t.firstgid = atoi(currentTileSetNode->first_attribute("firstgid")->value());
		t.source   = currentTileSetNode->first_attribute("source")->value();

		//Add tileset to CurrentMap tilesets array.
		CurrentMap.tilesets.push_back(t);

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
		Layer l;

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
		CurrentMap.layers.push_back(l);

		//Next tile node
		currentLayerNode = currentLayerNode->next_sibling("layer");
	}


	//--------------------------------------
	//Objects

	//Decompress all layer data.
	for (int i = 0; i < CurrentMap.layers.size(); i++)
	{
		DecompressLayerData(i);
	}
}

pery::TMX2Map::~TMX2Map()
{
}
