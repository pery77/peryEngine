#pragma once
#include <iostream>
#include <string>

#define LOG pery::Tools::Log

namespace pery {

	static class Tools {	
	public:
		static void Log(const char* text);
		static void Log(int text);
		static void Log(std::string text);
	private:

	};
}