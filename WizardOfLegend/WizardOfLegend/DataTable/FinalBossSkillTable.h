#pragma once
#include "DataTable.h"
#include <map>
#include <list>

class FinalBossSkillTable : public DataTable
{
protected:
	map<string, list<string>> table;

public:
	FinalBossSkillTable();
	virtual ~FinalBossSkillTable();

	const list<string>& Get(const string& setName);

	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};