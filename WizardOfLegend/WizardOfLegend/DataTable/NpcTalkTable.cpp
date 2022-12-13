#include "NpcTalkTable.h"
#include "../3rd/rapidcsv.h"

NpcTalkTable::NpcTalkTable()
	: DataTable(DataTable::Types::NpcTalk)
{
	fileName = "tables/tutorialSkillMessage.csv";
}

NpcTalkTable::~NpcTalkTable()
{
}

const vector<string>& NpcTalkTable::Get(const string& objType)
{
	auto find = table.find(objType);
	if (find == table.end())
		throw "empty";
	return find->second;
}

const string& NpcTalkTable::GetNpcName(const string& objType)
{
	auto find = table.find(objType);
	if (find == table.end())
		throw "empty";
	return find->first;
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

	for (int i = 0; i < rowCount; i++)
	{
		if (table.find(objType[i]) != table.end())
		{
			cout << "duplicate values exist" << endl;
			return false;
		}

		vector<string> talk;
		for (int j = 1; j < columnCount; ++j)
		{
			talk.push_back(doc.GetCell<string>(j, i));
			if (talk.empty())
				break;
		}

		table[objType[i]] = talk;
	}

	return true;
}
