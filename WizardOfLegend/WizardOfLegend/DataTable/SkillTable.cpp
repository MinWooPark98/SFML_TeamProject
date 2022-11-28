#include "SkillTable.h"
#include "../3rd/rapidcsv.h"

SkillTable::SkillTable()
	:DataTable(Types::Skill)
{
	fileName = "tables/skilltable.csv";
}

SkillTable::~SkillTable()
{
}

const Skill::Set& SkillTable::Get(const string& skillName)
{
	auto find = table.find(skillName);
	if (find == table.end())
		throw "Wrong skillName";
	return find->second;
}

void SkillTable::Release()
{
	table.clear();
}

bool SkillTable::Load()
{
	Release();

	rapidcsv::Document doc(fileName, rapidcsv::LabelParams(0, -1));
	auto columnCount = doc.GetColumnCount();
	auto rowCount = doc.GetRowCount();
	vector<string> skillName = doc.GetColumn<string>(0);
	vector<int> element = doc.GetColumn<int>(1);
	vector<int> attackType = doc.GetColumn<int>(2);
	vector<int> attackCntLim = doc.GetColumn<int>(3);
	vector<float> attackInterval = doc.GetColumn<float>(4);
	vector<float> distance = doc.GetColumn<float>(5);
	vector<int> attackShape = doc.GetColumn<int>(6);
	vector<float> amplitude = doc.GetColumn<float>(7);
	vector<float> frequency = doc.GetColumn<float>(8);
	vector<int> waveType = doc.GetColumn<int>(9);
	vector<float> fallingHeight = doc.GetColumn<float>(10);
	vector<int> rangeType = doc.GetColumn<int>(11);
	vector<int> playerAction = doc.GetColumn<int>(12);
	vector<float> skillDelay = doc.GetColumn<float>(13);
	vector<float> skillCoolDown = doc.GetColumn<float>(14);
	vector<int> dmgType = doc.GetColumn<int>(15);
	vector<float> dmgRatio = doc.GetColumn<float>(16);
	vector<float> dmgDelay = doc.GetColumn<float>(17);
	vector<float> duration = doc.GetColumn<float>(18);
	vector<float> speed = doc.GetColumn<float>(19);
	vector<string> animClipName_1 = doc.GetColumn<string>(20);
	vector<string> animClipName_2 = doc.GetColumn<string>(21);
	vector<string> soundName_1;
	vector<string> soundName_2;
	keys = skillName;
	for (int j = 0; j < rowCount; ++j)
	{
		string soundName1;
		soundName1 = doc.GetCell<string>(22, j);
		soundName_1.push_back(soundName1.empty() ? "" : soundName1);
		string soundName2 = doc.GetCell<string>(23, j);
		soundName_2.push_back(soundName2.empty() ? "" : soundName2);
	}

	for (int j = 0; j < rowCount; ++j)
	{
		if (table.find(skillName[j]) != table.end())
		{
			cout << "duplicate values exist" << endl;
			return false;
		}
		table.insert({ skillName[j], { skillName[j], (Skill::Element)element[j], (Skill::AttackType)attackType[j], attackCntLim[j], attackInterval[j], distance[j],  (Skill::AttackShape)attackShape[j], amplitude[j], frequency[j], (Skill::WaveType)waveType[j], fallingHeight[j], (Skill::RangeType)rangeType[j], (Player::SkillAction)playerAction[j], skillDelay[j], skillCoolDown[j], (Skill::DamageType)dmgType[j], dmgRatio[j], dmgDelay[j], duration[j], speed[j], vector<string>({ animClipName_1[j], animClipName_2[j] }), vector<string>({ soundName_1[j], soundName_2[j]})}});
	}
	return true;
}
