#pragma once
#include "DataTable.h"
#include <map>
#include <vector>
#include "../GameObject/Skill.h"

class SkillTable : public DataTable
{
protected:
	map<string, Skill::Set> table;

public:
	SkillTable();
	virtual ~SkillTable();

	const Skill::Set& Get(const string& skillName);

	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};

