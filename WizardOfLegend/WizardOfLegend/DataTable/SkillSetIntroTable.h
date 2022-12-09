#pragma once
#include "DataTable.h"
#include <map>
#include <vector>

class SkillSetIntroTable : public DataTable
{
public:
	struct Intro
	{
		string skillName_Kor;
		string skillIntro_Kor;
	};
protected:
	map<string, Intro> table;

public:
	SkillSetIntroTable();
	virtual ~SkillSetIntroTable();

	const Intro& Get(const string& skillName);

	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};

