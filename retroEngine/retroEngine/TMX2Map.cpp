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

	//Process map attributes
	processMap(mapNode);

	//Tileset Attributtes
	processTilesets(mapNode);

	//Layers in root
	processLayers(mapNode, NULL);

	//Groups in root (Recursive, read child groups and layers).
	processGroup(mapNode, NULL);
	
	//Objects
	processObjectGroup(mapNode, NULL);


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
		
		MapLoaded.tilesets[ts].tileset.name    = tilesetNode->first_attribute("name")   ->value();
		MapLoaded.tilesets[ts].tileset.version = tilesetNode->first_attribute("version")->value();

		MapLoaded.tilesets[ts].tileset.tileWidth  = atoi(tilesetNode->first_attribute("tilewidth") ->value());
		MapLoaded.tilesets[ts].tileset.tileHeight = atoi(tilesetNode->first_attribute("tileheight")->value());

		MapLoaded.tilesets[ts].tileset.tileCount = atoi(tilesetNode->first_attribute("tilecount")->value());
		MapLoaded.tilesets[ts].tileset.columns   = atoi(tilesetNode->first_attribute("columns")->value());

		MapLoaded.tilesets[ts].tileset.rows = MapLoaded.tilesets[ts].tileset.tileCount / MapLoaded.tilesets[ts].tileset.columns;

		//Image
		rapidxml::xml_node<char> * imageNode;
		imageNode = tilesetNode->first_node("image");

		MapLoaded.tilesets[ts].tileset.image.source = imageNode->first_attribute("source")->value();

		MapLoaded.tilesets[ts].tileset.image.width  = atoi(imageNode->first_attribute("width") ->value());
		MapLoaded.tilesets[ts].tileset.image.height = atoi(imageNode->first_attribute("height")->value());
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

		//Tsx file:
		std::cout << "    Name: " << MapLoaded.tilesets[i].tileset.name <<
			", Version: " << MapLoaded.tilesets[i].tileset.version <<
			std::endl;

		std::cout << "    Tile size: [w:" << MapLoaded.tilesets[i].tileset.tileWidth << ",h:" << MapLoaded.tilesets[i].tileset.tileHeight << "]" << std::endl;
		std::cout << "    Tileset size: [rows:" << MapLoaded.tilesets[i].tileset.rows << ",cols:" << MapLoaded.tilesets[i].tileset.columns << 
			"] (" << MapLoaded.tilesets[i].tileset.tileCount << " tiles.)"<< std::endl;

		//Tileset image
		std::cout << "    Image source: " << MapLoaded.tilesets[i].tileset.image.source << std::endl;
		std::cout << "    Image size: [w:" << MapLoaded.tilesets[i].tileset.image.width << 
			",h:" << MapLoaded.tilesets[i].tileset.image.height << "]" << std::endl;

		std::cout << std::endl;
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
		std::cout << "    visible: " << MapLoaded.layers[i].visible << std::endl;
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

//Get attribute value (name) from node
const char * pery::TMX2Map::getValue(rapidxml::xml_node<char> * node, const char * name)
{
	if (node->first_attribute(name) == NULL) return "null";
	return node->first_attribute(name)->value();
}

void pery::TMX2Map::processMap(rapidxml::xml_node<char>* node)
{
	//Store attributes in CurrentMap struct.
	MapLoaded.version = getValue(node, "version");
	MapLoaded.orientation = getValue(node, "orientation");

	//atoi string to interger
	MapLoaded.width = atoi(getValue(node, "width"));
	MapLoaded.height = atoi(getValue(node, "height"));

	MapLoaded.tileWidth = atoi(getValue(node, "tilewidth"));
	MapLoaded.tileHeight = atoi(getValue(node, "tileheight"));

	//Load backgroundcolor only if node exist
	MapLoaded.backgroundColor = getValue(node, "backgroundcolor");
}

void pery::TMX2Map::processTilesets(rapidxml::xml_node<char>* parentNode)
{
	rapidxml::xml_node<char> * node;
	node = parentNode->first_node("tileset");

	//Loop tilesets while not null.
	while (node != NULL)
	{
		//Create a tileset and load attributes.
		MapTileset t;

		t.firstgid = atoi(getValue(node, "firstgid"));
		t.source = getValue(node, "source");

		//Add tileset to CurrentMap tilesets array.
		MapLoaded.tilesets.push_back(t);

		//Next tile node
		node = node->next_sibling("tileset");
	}
}

void pery::TMX2Map::processLayers(rapidxml::xml_node<char>* parentNode, Group * parentGroup)
{
	rapidxml::xml_node<char> * node;
	node = parentNode->first_node("layer");

	while (node != NULL)
	{
		//Create a Layer and load attributes.
		MapLayer l;

		l.name = getValue(node, "name");

		l.id = atoi(getValue(node, "id"));
		l.width = atoi(getValue(node, "width"));
		l.height = atoi(getValue(node, "height"));

		l.visible = getValue(node, "visible") == "null" ? true : false;
		//Inherit parent visibility
		if (parentGroup != NULL && !parentGroup->visible) l.visible = false; 

		//Data
		rapidxml::xml_node<char> * dataNode;
		dataNode = node->first_node("data");

		Data d;
		if (dataNode->first_attribute("encoding") != 0)
			d.encoding = dataNode->first_attribute("encoding")->value();
		if (dataNode->first_attribute("compression") != 0)
			d.compression = dataNode->first_attribute("compression")->value();

		d.content = dataNode->value();

		//Put data inside layer
		l.data = d;

		//Add layer to CurrentMap layers array.
		MapLoaded.layers.push_back(l);

		//Next layer node
		node = node->next_sibling("layer");
	}
}

void pery::TMX2Map::processGroup(rapidxml::xml_node<char>* parentNode, Group * parentGroup)
{
	rapidxml::xml_node<char> * node;
	node = parentNode->first_node("group");

	//Loop groups while not null.
	while (node != NULL)
	{
		//Create group structure, not used, only visible state for childs.
		Group g;
		g.name = getValue(node, "name");
		g.id   = atoi(getValue(node, "id"));

		g.visible = getValue(node, "visible") == "null" ? true : false;

		//Inherit parent visibility
		if (parentGroup != NULL && !parentGroup->visible) g.visible = false;
	
		//Recursive Groups ( try to find chidren groups )
		processGroup(node, &g);

		//Process layers in current group
		processLayers(node, &g);

		//Process ObjectGroup in current group
		processObjectGroup(node, &g);

		//Next group node in same branch.
		node = node->next_sibling("group");
	}
}

void pery::TMX2Map::processObjectGroup(rapidxml::xml_node<char>* parentNode, Group * parentGroup)
{
	rapidxml::xml_node<char> * node;
	node = parentNode->first_node("objectgroup");

	//Loop objectgroup while not null.
	while (node != NULL)
	{
		MapObjectGroup og;
		og.name = getValue(node, "name");
		og.id = atoi(getValue(node, "id"));

		og.visible = getValue(node, "visible") == "null" ? true : false;

		//Inherit parent visibility
		if (parentGroup != NULL && !parentGroup->visible) og.visible = false;


		//processObjects(node); //TODO

		std::cout<<og.name<<" v: "<< og.visible<<std::endl;
		//Next layer node
		node = node->next_sibling("objectgroup");
	}
}
