#include "NpcTalkTable.h"
#include "../3rd/rapidcsv.h"

NpcTalkTable::NpcTalkTable()
	: DataTable(DataTable::Types::NpcTalk)
{
}

NpcTalkTable::~NpcTalkTable()
{
	fileName = "tables/npcTalkTable.csv";
}

const NpcTalkTable::NpcType& NpcTalkTable::Get(const string& objType)
{
	auto find = table.find(objType);
	if (find == table.end())
		throw "Wrong skillName";
	return find->second;
}

void NpcTalkTable::Release()
{
	table.clear();
}

bool NpcTalkTable::Load()
{
	Release();

	rapidcsv::Document doc(fileName, rapidcsv::LabelParams(0, -1));
	auto columnCount = doc.GetColumnCount();
	auto rowCount = doc.GetRowCount();
	vector<string> objType = doc.GetColumn<string>(0);
	keys = objType;

	vector<vector<string>> talks;
	for (int i = 0; i < columnCount; i++)
	{
		vector<string> talk = doc.GetColumn<string>(i);
		talks.push_back(talk);
	}

	for (int j = 0; j < rowCount; ++j)
	{
		if (table.find(objType[j]) != table.end())
		{
			cout << "duplicate values exist" << endl;
			return false;
		}

		for (int i = 0; i < talks.size(); i++)
			table.insert({ objType[j], { talks[j][i] } });
	}

	return true;
}
