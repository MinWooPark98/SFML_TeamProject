#pragma once
#include "DataTable.h"
#include <map>
#include <list>

class SkillSetTable : public DataTable
{
protected:
	map<string, pair<float, list<string>>> table;

public:
	SkillSetTable();
	virtual ~SkillSetTable();

	const pair<float, list<string>>& Get(const string& setName);

	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};

