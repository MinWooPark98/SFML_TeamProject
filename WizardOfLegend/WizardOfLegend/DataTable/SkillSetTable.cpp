#include "SkillSetTable.h"
#include "../3rd/rapidcsv.h"

SkillSetTable::SkillSetTable()
	:DataTable(Types::SkillSet)
{
	fileName = "tables/skillSet.csv";
}

SkillSetTable::~SkillSetTable()
{
}

const SkillSetTable::SetInfo& SkillSetTable::Get(const string& setName)
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
		string iconDir = doc.GetCell<string>(2, j);
		list<string> skillNames;
		string str;
		for(int i = 3; i < columnCount;++i)
		{
			str = doc.GetCell<string>(i, j);
			if (str.empty())
				break;
			skillNames.push_back(str);
		}
		table[setName[j]] = { coolDown, iconDir, skillNames };
	}
	return true;
}
