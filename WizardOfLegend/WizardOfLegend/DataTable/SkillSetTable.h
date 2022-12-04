#pragma once
#include "DataTable.h"
#include <map>
#include <list>

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
	map<string, SetInfo> table;

public:
	SkillSetTable();
	virtual ~SkillSetTable();

	const SetInfo& Get(const string& setName);

	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};

