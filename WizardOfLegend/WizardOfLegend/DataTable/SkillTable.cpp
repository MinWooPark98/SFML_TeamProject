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
	vector<string> skillName = doc.GetColumn<string>(0);
	vector<int> element = doc.GetColumn<int>(1);
	vector<int> attackType = doc.GetColumn<int>(2);
	vector<int> attackCntLim = doc.GetColumn<int>(3);
	vector<float> attackInterval = doc.GetColumn<float>(4);
	vector<float> distance = doc.GetColumn<float>(5);
	vector<int> attackShape = doc.GetColumn<int>(6);
	vector<float> amplitude = doc.GetColumn<float>(7);
	vector<float> frequency = doc.GetColumn<float>(8);
	vector<int> moveType = doc.GetColumn<int>(9);
	vector<int> playerAction = doc.GetColumn<int>(10);
	vector<float> skillDelay = doc.GetColumn<float>(11);
	vector<float> skillCoolDown = doc.GetColumn<float>(12);
	vector<float> dmgRatio = doc.GetColumn<float>(13);
	vector<int> dmgType = doc.GetColumn<int>(14);
	vector<float> dmgDelay = doc.GetColumn<float>(15);
	vector<float> duration = doc.GetColumn<float>(16);
	vector<float> speed = doc.GetColumn<float>(17);
	vector<string> animClipName_1 = doc.GetColumn<string>(18);
	vector<string> animClipName_2 = doc.GetColumn<string>(19);

	for (int j = 0; j < doc.GetRowCount(); ++j)
	{
		if (table.find(skillName[j]) != table.end())
		{
			cout << "duplicate values exist" << endl;
			return false;
		}
		table.insert({ skillName[j], { skillName[j], (Skill::Element)element[j], (Skill::AttackType)attackType[j], attackCntLim[j], attackInterval[j], distance[j],  (Projectile::AttackShape)attackShape[j], amplitude[j], frequency[j], (Projectile::MoveType)moveType[j], (Player::SkillAction)playerAction[j], skillDelay[j], skillCoolDown[j], dmgRatio[j], (Projectile::DamageType)dmgType[j], dmgDelay[j], duration[j], speed[j], vector<string>({animClipName_1[j], animClipName_2[j]})}});
	}
	return true;
}
