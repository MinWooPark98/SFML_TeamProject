#pragma once
#include "DataTable.h"
#include <map>
#include <list>
#include "../GameObject/SkillSet.h"

class SkillSetTable : public DataTable
{
public:
	struct SetInfo
	{
		float newCoolDown;
		string iconDir;
		list<string> skillNames;
	};
protected:
	map<Skill::Element, map<string, SetInfo>> table;

public:
	SkillSetTable();
	virtual ~SkillSetTable();

	const SetInfo& Get(const string& setName);
	const map<string, SetInfo> Get(Skill::Element elem);
	const Skill::Element GetElement(const string& name);
	const map<Skill::Element, map<string, SetInfo>>& GetTable() { return table; }

	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};

