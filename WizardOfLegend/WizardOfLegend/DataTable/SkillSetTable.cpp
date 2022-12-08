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
	for (auto& elemTable : table)
	{
		auto find = elemTable.second.find(setName);
		if (find != elemTable.second.end())
			return find->second;
	}
	throw "Wrong SkillSetName";
}

const map<string, SkillSetTable::SetInfo> SkillSetTable::Get(Skill::Element elem)
{
	auto find = table.find(elem);
	if (find == table.end())
		throw "Wrong value";
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
		for (auto& elemTable : table)
		{
			if (elemTable.second.find(setName[j]) != elemTable.second.end())
			{
				cout << "duplicate values exist" << endl;
				return false;
			}
		}
		int element = doc.GetCell<int>(1, j);
		float coolDown = 0.f;
		try
		{
			coolDown = doc.GetCell<float>(2, j);
			if (coolDown < 0.f)
				coolDown = -1.f;
		}
		catch (exception expn)
		{
			coolDown = -1.f;
		}
		string iconDir = doc.GetCell<string>(3, j);
		list<string> skillNames;
		string str;
		for(int i = 4; i < columnCount;++i)
		{
			str = doc.GetCell<string>(i, j);
			if (str.empty())
				break;
			skillNames.push_back(str);
		}
		
		table[(Skill::Element)element][setName[j]] = { coolDown, iconDir, skillNames };
	}
	return true;
}
