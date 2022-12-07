#include "PlatinumTable.h"
#include "../3rd/rapidcsv.h"

PlatinumTable::PlatinumTable()
	:DataTable(DataTable::Types::Platinum)
{
	fileName = "tables/platinumTable.csv";
}

PlatinumTable::~PlatinumTable()
{
}

const PlatinumTable::Platinum& PlatinumTable::Get(const string& objType)
{
	auto find = table.find(objType);
	if (find == table.end())
		throw "None";
	return find->second;
}

void PlatinumTable::Release()
{
	table.clear();
}

bool PlatinumTable::Load()
{
	Release();

	rapidcsv::Document doc(fileName, rapidcsv::LabelParams(0, -1));
	auto columnCount = doc.GetColumnCount();
	auto rowCount = doc.GetRowCount();
	vector<string> objType = doc.GetColumn<string>(0);
	keys = objType;
	vector<int> platinum = doc.GetColumn<int>(1);

	for (int j = 0; j < rowCount; ++j)
	{
		if (table.find(objType[j]) != table.end())
		{
			cout << "duplicate values exist" << endl;
			return false;
		}
		table.insert({ objType[j], {platinum[j]}});
	}
	return true;
}
