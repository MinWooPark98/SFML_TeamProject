#pragma once
#include "DataTable.h"
#include <map>

class TutorialClearTable : public DataTable
{
protected:
	map<string, vector<string>> table;

public:
	TutorialClearTable();
	virtual ~TutorialClearTable();

	const vector<string>& Get(const string& objType);
	const string& GetNpcName(const string& objType);
	const map<string, vector<string>>& GetTable() { return table; };
	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};