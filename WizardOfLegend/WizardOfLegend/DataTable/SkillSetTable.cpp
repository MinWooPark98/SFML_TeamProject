#include "SkillSetTable.h"
#include "../3rd/rapidcsv.h"
#include <fstream>

SkillSetTable::SkillSetTable()
	:DataTable(Types::SkillSet)
{
	fileName = "tables/SkillSet.csv";
}

SkillSetTable::~SkillSetTable()
{
}

const SkillSetTable::SetInfo& SkillSetTable::Get(const string& setName)
{
	for (auto& outermost : table)
	{
		for (auto& elemTable : outermost.second)
		{
			auto find = elemTable.second.find(setName);
			if (find != elemTable.second.end())
				return find->second;
		}
	}
	throw invalid_argument("wrong value");
}

const map<Skill::Element, map<string, SkillSetTable::SetInfo>> SkillSetTable::GetElementLists(Locked locked)
{
	auto find = table.find(locked);
	if (find == table.end())
		throw invalid_argument("wrong value");
	return find->second;
}

const map<string, SkillSetTable::SetInfo> SkillSetTable::Get(Locked locked, Skill::Element elem)
{
	if (table.find(locked) == table.end())
		throw invalid_argument("wrong value");
	auto find = table[locked].find(elem);
	if (find == table[locked].end())
		throw invalid_argument("wrong value");
	return find->second;
}

const Skill::Element SkillSetTable::GetElement(const string& name)
{
	for (int i = 0; i < (int)Skill::Element::Count; ++i)
	{
		auto find_locked = table[Locked::Locked][(Skill::Element)i].find(name);
		if (find_locked != table[Locked::Locked][(Skill::Element)i].end())
			return (Skill::Element)i;
		auto find_unlocked = table[Locked::Unlocked][(Skill::Element)i].find(name);
		if (find_unlocked != table[Locked::Unlocked][(Skill::Element)i].end())
			return (Skill::Element)i;
	}
	throw invalid_argument("wrong value");
}

void SkillSetTable::Release()
{
	table.clear();
}

bool SkillSetTable::Load()
{
	Release();

	map<string, Locked> lockedTable;
	string lockedFileName = "tables/SkillSetLocked.csv";
	rapidcsv::Document docLocked(lockedFileName, rapidcsv::LabelParams(0, -1));
	{
		auto columnCount = docLocked.GetColumnCount();
		auto rowCount = docLocked.GetRowCount();
		vector<string> key = docLocked.GetColumn<string>(0);
		vector<int> locked = docLocked.GetColumn<int>(1);
		for (int j = 0; j < rowCount; ++j)
		{
			if (lockedTable.find(key[j]) != lockedTable.end())
			{
				cout << "duplicate values exist" << endl;
				return false;
			}
			lockedTable[key[j]] = (Locked)locked[j];
		}
	}

	rapidcsv::Document doc(fileName, rapidcsv::LabelParams(0, -1));
	vector<string> setName = doc.GetColumn<string>(0);
	keys = setName;
	auto columnCount = doc.GetColumnCount();
	auto rowCount = doc.GetRowCount();
	for (int j = 0; j < rowCount; ++j)
	{
		for (auto& outermost : table)
		{
			for (auto& elemTable : outermost.second)
			{
				if (elemTable.second.find(setName[j]) != elemTable.second.end())
				{
					cout << "duplicate values exist" << endl;
					return false;
				}
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
		Locked locked = Locked::Locked;
		if (lockedTable.find(setName[j]) == lockedTable.end())
		{
			auto row = docLocked.GetRowCount();
			docLocked.SetCell(0, row, setName[j]);
			docLocked.SetCell(1, row, (int)Locked::Locked);
			docLocked.Save(lockedFileName);
		}
		else
			locked = lockedTable[setName[j]];
		table[locked][(Skill::Element)element][setName[j]] = {coolDown, iconDir, skillNames};
	}
	return true;
}
