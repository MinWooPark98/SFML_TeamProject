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

const pair<float, list<string>>& SkillSetTable::Get(const string& setName)
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
		float coolDown = 0.f;
		list<string> skillNames;
		string str;
		try
		{
			coolDown = doc.GetCell<float>(1, j);
			if (coolDown < 0.f)
				coolDown = -1.f;
		}
		catch (exception expn)
		{
			coolDown = -1.f;
		}
		for(int i = 2; i < columnCount;++i)
		{
			str = doc.GetCell<string>(i, j);
			if (str.empty())
				break;
			skillNames.push_back(str);
		}
		table[setName[j]] = { coolDown, skillNames };
	}
	return true;
}
