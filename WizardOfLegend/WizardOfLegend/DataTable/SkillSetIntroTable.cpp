#include "SkillSetIntroTable.h"
#include "../3rd/rapidcsv.h"

SkillSetIntroTable::SkillSetIntroTable()
	:DataTable(Types::SkillSetIntro)
{
	fileName = "tables/skillsetIntro.csv";
}

SkillSetIntroTable::~SkillSetIntroTable()
{
}

const SkillSetIntroTable::Intro& SkillSetIntroTable::Get(const string& skillName)
{
	auto find = table.find(skillName);
	if (find == table.end())
		throw "Wrong skillName";
	return find->second;
}

void SkillSetIntroTable::Release()
{
	table.clear();
}

bool SkillSetIntroTable::Load()
{
	Release();

	rapidcsv::Document doc(fileName, rapidcsv::LabelParams(0, -1));
	auto columnCount = doc.GetColumnCount();
	auto rowCount = doc.GetRowCount();
	vector<string> skillSetName = doc.GetColumn<string>(0);
	vector<string> skillSetName_kor = doc.GetColumn<string>(1);
	vector<string> skillSetIntro_kor = doc.GetColumn<string>(2);
	keys = skillSetName;
	for (int j = 0; j < rowCount; ++j)
	{
		if (table.find(skillSetName[j]) != table.end())
		{
			cout << "duplicate values exist" << endl;
			return false;
		}
		table.insert({ skillSetName[j], {skillSetName_kor[j], skillSetIntro_kor[j]} });
	}
	return true;
}
