#pragma once
#include "DataTable.h"
#include <map>

class StatTable : public DataTable
{
protected:
	struct Stat
	{
		int attackDmg;
		int maxHp;
		float speed;
	};
	map<string, Stat> table;

public:
	StatTable();
	virtual ~StatTable();

	const Stat& Get(const string& objType);

	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};

