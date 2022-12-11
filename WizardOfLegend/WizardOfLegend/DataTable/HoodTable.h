#pragma once
#include "DataTable.h"
#include <map>
#include "../GameObject/Item/Hood.h"

class HoodTable : public DataTable
{
public:
	enum class Locked
	{
		Locked,
		Unlocked,
	};
protected:
	map<Locked, map<int, Hood::HoodInfo>> table;

public:
	HoodTable();
	virtual ~HoodTable();

	const Hood::HoodInfo& Get(int id);
	const map<int, Hood::HoodInfo>& GetInfoList(Locked locked);
	const map<Locked, map<int, Hood::HoodInfo>>& GetTable() { return table; }

	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};

