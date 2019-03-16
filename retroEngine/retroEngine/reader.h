#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "tile.h"
#include "tilesetManager.h"
#include "minijson_reader.hpp"
#include "tools.h"

namespace pery
{
	class Reader
	{
	public:

		Reader();
		~Reader();

		void LoadFile(std::string fileName);

	private:

	};
}
