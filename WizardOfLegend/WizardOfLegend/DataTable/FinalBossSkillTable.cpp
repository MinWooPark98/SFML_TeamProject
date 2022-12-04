#include "FinalBossSkillTable.h"
#include "../3rd/rapidcsv.h"

FinalBossSkillTable::FinalBossSkillTable()
	:DataTable(Types::FinalBossSkill)
{
	fileName = "tables/finalbossSkillTable.csv";
}

FinalBossSkillTable::~FinalBossSkillTable()
{
}

const list<string>& FinalBossSkillTable::Get(const string& setName)
{
	auto find = table.find(setName);
	if (find == table.end())
		throw "Wrong SkillSetName";
	return find->second;
}

void FinalBossSkillTable::Release()
{
	table.clear();
}

bool FinalBossSkillTable::Load()
{
	Release();
	rapidcsv::Document doc(fileName, rapidcsv::LabelParams(0, -1));
	vector<string> listElem = doc.GetColumn<string>(0);
	keys = listElem;
	auto columnCount = doc.GetColumnCount();
	auto rowCount = doc.GetRowCount();
	for (int j = 0; j < rowCount; ++j)
	{
		if (table.find(listElem[j]) != table.end())
		{
			cout << "duplicate values exist" << endl;
			return false;
		}
		list<string> skillNames;
		string str;
		for (int i = 1; i < columnCount; ++i)
		{
			str = doc.GetCell<string>(i, j);
			if (str.empty())
				break;
			skillNames.push_back(str);
		}
		table[listElem[j]] = skillNames;
	}
	return true;
}
