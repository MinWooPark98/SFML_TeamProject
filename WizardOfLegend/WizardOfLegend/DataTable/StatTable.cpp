#include "StatTable.h"
#include "../3rd/rapidcsv.h"

StatTable::StatTable()
	:DataTable(DataTable::Types::Stat)
{
	fileName = "tables/StatTable.csv";
}

StatTable::~StatTable()
{
}

const StatTable::Stat& StatTable::Get(const string& objType)
{
	auto find = table.find(objType);
	if (find == table.end())
		throw invalid_argument("wrong value");
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
	vector<float> speed = doc.GetColumn<float>(1);
	vector<int> attackDmg = doc.GetColumn<int>(2);
	vector<float> damageTake = doc.GetColumn<float>(3);
	vector<int> maxHp = doc.GetColumn<int>(4);
	vector<float> evasionRate = doc.GetColumn<float>(5);
	vector<float> criticalRate = doc.GetColumn<float>(6);
	vector<float> criticalRatio = doc.GetColumn<float>(7);
	for (int j = 0; j < rowCount; ++j)
	{
		if (table.find(objType[j]) != table.end())
		{
			cout << "duplicate values exist" << endl;
			return false;
		}
		table.insert({ objType[j], { speed[j], attackDmg[j], damageTake[j], maxHp[j], evasionRate[j], criticalRate[j], criticalRatio[j] }});
	}
	return true;
}
