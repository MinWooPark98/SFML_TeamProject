#pragma once
#include "DataTable.h"
#include <map>
#include "../GameObject/Item/Relic.h"

class RelicTable : public DataTable
{
public:
	enum class Locked
	{
		Locked,
		Unlocked,
	};
protected:
	map<Locked, map<int, Relic::RelicInfo>> table;

public:
	RelicTable();
	virtual ~RelicTable();

	const Relic::RelicInfo& Get(int id);
	const map<int, Relic::RelicInfo>& GetInfoList(Locked locked);
	const map<Locked, map<int, Relic::RelicInfo>>& GetTable() { return table; }

	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};

