#include "SkillSetTable.h"
#include "../3rd/rapidcsv.h"

SkillSetTable::SkillSetTable()
	:DataTable(Types::SkillSet)
{
	fileName = "tables/skillset.csv";
}

SkillSetTable::~SkillSetTable()
{
}

const list<string>& SkillSetTable::Get(const string& setName)
{
	auto find = table.find(setName);
	if (find == table.end())
		throw "Wrong SkillSetName";
	return find->second;
}

void SkillSetTable::Release()
{
	table.clear();
}

bool SkillSetTable::Load()
{
	Release();
	rapidcsv::Document doc(fileName, rapidcsv::LabelParams(0, -1));
	vector<string> setName = doc.GetColumn<string>(0);
	keys = setName;
	auto columnCount = doc.GetColumnCount();
	auto rowCount = doc.GetRowCount();
	for (int j = 0; j < rowCount; ++j)
	{
		if (table.find(setName[j]) != table.end())
		{
			cout << "duplicate values exist" << endl;
			return false;
		}
		string str;
		for(int i = 1; i < columnCount;++i)
		{
			str = doc.GetCell<string>(i, j);
			if (str.empty())
				break;
			table[setName[j]].push_back(str);
		}
	}
	return true;
}
