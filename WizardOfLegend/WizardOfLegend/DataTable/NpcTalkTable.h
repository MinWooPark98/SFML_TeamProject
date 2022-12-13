#pragma once
#include "DataTable.h"
#include <map>

class NpcTalkTable : public DataTable
{
protected:
	map<string, vector<string>> table;

public:
	NpcTalkTable();
	virtual ~NpcTalkTable();

	const vector<string>& Get(const string& objType);
	const string& GetNpcName(const string& objType);
	const map<string, vector<string>>& GetTable() { return table; };
	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};

