#include "StatTable.h"
#include "../3rd/rapidcsv.h"

StatTable::StatTable()
	:DataTable(DataTable::Types::Stat)
{
	fileName = "tables/statTable.csv";
}

StatTable::~StatTable()
{
}

const StatTable::Stat& StatTable::Get(const string& objType)
{
	auto find = table.find(objType);
	if (find == table.end())
		throw "Wrong skillName";
	return find->second;
}

void StatTable::Release()
{
	table.clear();
}

bool StatTable::Load()
{
	Release();

	rapidcsv::Document doc(fileName, rapidcsv::LabelParams(0, -1));
	auto columnCount = doc.GetColumnCount();
	auto rowCount = doc.GetRowCount();
	vector<string> objType = doc.GetColumn<string>(0);
	keys = objType;
	vector<int> attackDmg = doc.GetColumn<int>(1);
	vector<int> maxHp = doc.GetColumn<int>(2);
	vector<float> speed = doc.GetColumn<float>(3);
	for (int j = 0; j < rowCount; ++j)
	{
		if (table.find(objType[j]) != table.end())
		{
			cout << "duplicate values exist" << endl;
			return false;
		}
		table.insert({ objType[j], {attackDmg[j], maxHp[j], speed[j]} });
	}
	return true;
}
