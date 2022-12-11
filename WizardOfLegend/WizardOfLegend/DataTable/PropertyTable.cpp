#include "PropertyTable.h"
#include "../3rd/rapidcsv.h"

PropertyTable::PropertyTable()
	: DataTable(DataTable::Types::MonsterProperty)
{
	fileName = "tables/monsterPropertyTable.csv";
}

PropertyTable::~PropertyTable()
{
}

const PropertyTable::Property& PropertyTable::Get(const string& objType)
{
	auto find = table.find(objType);
	if (find == table.end())
		throw "Wrong skillName";
	return find->second;
}

void PropertyTable::Release()
{
	table.clear();
}

bool PropertyTable::Load()
{
	Release();

	rapidcsv::Document doc(fileName, rapidcsv::LabelParams(0, -1));
	auto columnCount = doc.GetColumnCount();
	auto rowCount = doc.GetRowCount();
	vector<string> objType = doc.GetColumn<string>(0);
	keys = objType;

	vector<float> goldProbability = doc.GetColumn<float>(1);
	vector<int> goldDropNumber = doc.GetColumn<int>(2);
	vector<int> minGold = doc.GetColumn<int>(3);
	vector<int> maxGold = doc.GetColumn<int>(4);
	vector<float> platinumProbability = doc.GetColumn<float>(5);
	vector<int> platinumDropNumber = doc.GetColumn<int>(6);

	for (int j = 0; j < rowCount; ++j)
	{
		if (table.find(objType[j]) != table.end())
		{
			cout << "duplicate values exist" << endl;
			return false;
		}
		table.insert({ objType[j], 
			{goldProbability[j], goldDropNumber[j], minGold[j], maxGold[j], platinumProbability[j], platinumDropNumber[j]} });
	}

	return true;
}
