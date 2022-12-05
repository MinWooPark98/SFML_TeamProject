#include "MapNameTable.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <iostream>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

MapNameTable::MapNameTable()
	:DataTable(DataTable::Types::MapName)
{
}

MapNameTable::~MapNameTable()
{
}

void MapNameTable::Release()
{
}

bool MapNameTable::Load()
{
	for (auto& file : std::experimental::filesystem::directory_iterator("config/data/map/"))
	{
		string name = file.path().string();
		name = name.substr(16, name.size() - 17 + 1);
		name = name.substr(0, name.size() - 5);
		keys.push_back(name);
	}
	return true;
}
