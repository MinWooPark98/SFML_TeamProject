#pragma once
#include "DataTable.h"
#include <map>
#include "../GameObject/Item/Hood.h"
#include "../3rd/rapidcsv.h"

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
	rapidcsv::Document hoodLocked;

public:
	HoodTable();
	virtual ~HoodTable();

	const Hood::HoodInfo& Get(int id);
	const map<int, Hood::HoodInfo>& GetInfoList(Locked locked);
	const map<Locked, map<int, Hood::HoodInfo>>& GetTable() { return table; }
	void Unlock(int id);

	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};

