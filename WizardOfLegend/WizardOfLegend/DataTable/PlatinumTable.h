#pragma once
#include "DataTable.h"
#include <map>

class PlatinumTable : public DataTable
{
protected:
	struct Platinum
	{
		int platinum;
	};
	map<string, Platinum> table;

public:
	PlatinumTable();
	virtual ~PlatinumTable();

	const Platinum& Get(const string& objType);

	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};

