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
	vector<int> moveType = doc.GetColumn<int>(8);
	vector<int> playerAction = doc.GetColumn<int>(9);
	vector<float> skillDelay = doc.GetColumn<float>(10);
	vector<float> dmgRatio = doc.GetColumn<float>(11);
	vector<int> dmgType = doc.GetColumn<int>(12);
	vector<float> dmgDelay = doc.GetColumn<float>(13);
	vector<float> duration = doc.GetColumn<float>(14);
	vector<float> speed = doc.GetColumn<float>(15);
	vector<string> animClipName = doc.GetColumn<string>(16);

	for (int j = 0; j < doc.GetRowCount(); ++j)
	{
		if (table.find(skillName[j]) != table.end())
		{
			cout << "duplicate values exist" << endl;
			return false;
		} 
		table.insert({ skillName[j], { skillName[j], (Skill::Element)element[j], (Skill::AttackType)attackType[j], attackCntLim[j], attackInterval[j], distance[j],  (Projectile::AttackShape)attackShape[j], amplitude[j], (Projectile::MoveType)moveType[j], (Player::SkillAction)playerAction[j], skillDelay[j], dmgRatio[j], (Projectile::DamageType)dmgType[j], dmgDelay[j], duration[j], speed[j], animClipName[j] } });
	}
	return true;
}
