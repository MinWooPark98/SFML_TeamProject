#pragma once
#include <string>
#include <vector>

using namespace std;

class DataTable
{
public:
	enum class Types
	{
		None = -1,
		Platinum,
		Stat,
		Skill,
		SkillSet,
		SkillSetIntro,
		FinalBossSkill,
		NpcTalk,
		MonsterProperty,
		MapName,
		Relic,
		Hood,
		SavedData,
		TutorialClear,
	};
protected:
	Types type;
	vector<string> keys;

	DataTable(const DataTable& ref);
	DataTable& operator = (const DataTable& ref);
public:
	static int TotalTypes;
	DataTable(Types t);

	virtual const vector<string>& GetKeys() { return keys; }
	virtual void Release() = 0;
	virtual bool Load() = 0;
};