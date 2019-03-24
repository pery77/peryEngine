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

	parseTMX(mapNode);

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
	//return;
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
/*
	//Show layers info
	std::cout << MapLoaded.layers.size() << " layer(s)" << std::endl;
	std::cout << std::endl;
	for (int i = 0; i < MapLoaded.layers.size(); i++)
	{
		std::cout << "    Layer: " << i << " index: "<< MapLoaded.layers[i].index << std::endl;
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
*/
	//Show objects info
	std::cout << MapLoaded.objects.size() << " objects(s)" << std::endl;
	std::cout << std::endl;
	for (int i = 0; i < MapLoaded.objects.size(); i++)
	{
		std::cout << "    Object: " << i << std::endl;
		std::cout << "    ";
		std::cout << "Name: " << MapLoaded.objects[i].name <<
			" | ID: " << MapLoaded.objects[i].id << std::endl;

		std::cout << std::endl;

	}

	std::cout << std::endl;
/*
	//Show imagelayers info
	std::cout << MapLoaded.imageLayers.size() << " imageLayer(s)" << std::endl;
	std::cout << std::endl;
	for (int i = 0; i < MapLoaded.imageLayers.size(); i++)
	{
		std::cout << "    ImageLayer: " << i << " index: " << MapLoaded.layers[i].index << std::endl;
		std::cout << "    ";
		std::cout << "Name: " << MapLoaded.imageLayers[i].name <<
			" | ID: " << MapLoaded.imageLayers[i].id << std::endl;
	}
*/
	std::cout << std::endl;

}

//Get attribute value (name) from node
const char * pery::TMX2Map::getValue(rapidxml::xml_node<char> * node, const char * name)
{
	if (node->first_attribute(name) == NULL) return "null";
	return node->first_attribute(name)->value();
}

void pery::TMX2Map::parseTMX( rapidxml::xml_node<>* node, int indent, Group * parentGroup)
{
	const auto ind = std::string(indent * 4, ' ');
	printf("%s", ind.c_str());

	const rapidxml::node_type t = node->type();
	
	if (t == rapidxml::node_element)
	{
		char * currentNode = node->name();
		
		printf("[%s]\n", currentNode);

		if (std::strcmp(currentNode, "map") == 0)
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

			if(MapLoaded.backgroundColor != "null")
				MapLoaded.bgColor = hex2Color(MapLoaded.backgroundColor);
		}

		if (std::strcmp(currentNode, "tileset") == 0)
		{
			//Create a tileset and load attributes.
			MapTileset t;

			t.firstgid = atoi(getValue(node, "firstgid"));
			t.source = getValue(node, "source");

			//Add tileset to CurrentMap tilesets array.
			MapLoaded.tilesets.push_back(t);
		}

		if (std::strcmp(currentNode, "layer") == 0)
		{
			//Create a Layer and load attributes.
			MapLayer l;

			l.name = getValue(node, "name");

			l.id = atoi(getValue(node, "id"));
			l.width = atoi(getValue(node, "width"));
			l.height = atoi(getValue(node, "height"));

			l.offsetx = atoi(getValue(node, "offsetx"));
			l.offsety = atoi(getValue(node, "offsety"));

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

			//Decompress zlib data string and store in currentlayer IDs array
			DecompressLayerData(&l);

			processProperties(node, &l.properties);

			bool isI = findBool(&l.properties, "isImage");
			l.isImage = isI;

			float sX = findFloat(&l.properties, "speedX");
			if (!isnan(sX)) l.speedX = sX;

			float sY = findFloat(&l.properties, "speedY");
			if (!isnan(sY)) l.speedY = sY;

			//Add layer to CurrentMap renderQueue array.
			RenderQueue r;
			r.layer = l;

			MapLoaded.renderQueue.push_back(r);
		}

		if (std::strcmp(currentNode, "objectgroup") == 0)
		{

			MapObjectGroup og;
			og.name = getValue(node, "name");
			og.id = atoi(getValue(node, "id"));

			og.visible = getValue(node, "visible") == "null" ? true : false;

			//Inherit parent visibility
			if (parentGroup != NULL && !parentGroup->visible) og.visible = false;

			//processObjects(node, parentGroup);
			rapidxml::xml_node<char> * objectNode;
			objectNode = node->first_node("object");

			while (objectNode != NULL)
			{
				MapObject o;

				o.name = getValue(objectNode, "name");
				o.id = atoi(getValue(objectNode, "id"));
				o.type = getValue(objectNode, "type");

				o.x = atoi(getValue(objectNode, "x"));
				o.y = atoi(getValue(objectNode, "y"));

				o.width = atoi(getValue(objectNode, "width"));
				o.height = atoi(getValue(objectNode, "height"));

				o.visible = getValue(objectNode, "visible") == "null" ? true : false;
				//Inherit parent visibility
				if (parentGroup != NULL && !parentGroup->visible) o.visible = false;

				//Add current object to CurrentMap objects array.
				MapLoaded.objects.push_back(o);

				objectNode = objectNode->next_sibling("object");
			}

		}

		if (std::strcmp(currentNode, "imagelayer") == 0)
		{
			MapImageLayer il;

			il.name = getValue(node, "name");
			il.id = atoi(getValue(node, "id"));

			il.offsetx = atoi(getValue(node, "offsetx"));
			il.offsety = atoi(getValue(node, "offsety"));

			il.visible = getValue(node, "visible") == "null" ? true : false;
			//Inherit parent visibility
			if (parentGroup != NULL && !parentGroup->visible) il.visible = false;

			//Get a child image node.
			rapidxml::xml_node<char> * iNode;
			iNode = node->first_node("image");

			//If exist
			if (iNode != NULL) {

				Image i;
				i.source = getValue(iNode, "source");
				i.width = atoi(getValue(iNode, "widht"));
				i.height = atoi(getValue(iNode, "heiht"));

				//Add to image layer.
				il.image = i;
			}

			processProperties(node, &il.properties);
			
			float sX = findFloat(&il.properties, "speedX");
			if (!isnan(sX)) il.speedX = sX;

			float sY = findFloat(&il.properties, "speedY");
			if (!isnan(sY)) il.speedY = sY;
			

			printf(">>>>>>speedx: %f, speedY: %f\n",il.speedX, il.speedY);
			//Add image layer to current map
			RenderQueue r;
			r.imageLayer = il;

			MapLoaded.renderQueue.push_back(r);
		}

		if (std::strcmp(currentNode, "group") == 0)
		{
			//Create group structure, not used, only visible state for childs.
			Group g;
			g.name = getValue(node, "name");
			g.id = atoi(getValue(node, "id"));

			g.visible = getValue(node, "visible") == "null" ? true : false;

			//Inherit parent visibility
			if (parentGroup != NULL && !parentGroup->visible) g.visible = false;
			
			parentGroup = &g;
		}

		//Child node.
		for (rapidxml::xml_node<>* n = node->first_node(); n; n = n->next_sibling()) {
			parseTMX(n, indent + 1, parentGroup);
		}

	}

	
}

void pery::TMX2Map::processProperties(rapidxml::xml_node<>* node, std::map<std::string, std::string>* properties)
{
	rapidxml::xml_node<char> * propertiesNode;
	propertiesNode = node->first_node("properties");

	if (propertiesNode != NULL)
	{
		rapidxml::xml_node<char> * propertyNode;
		propertyNode = propertiesNode->first_node("property");

		while (propertyNode != NULL)
		{
			std::string key (getValue(propertyNode, "name"));
			std::string value(getValue(propertyNode, "value"));

			properties->emplace(key,value);

			propertyNode = propertyNode->next_sibling("property");
		}
		
	}
}
