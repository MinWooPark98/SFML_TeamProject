#pragma once
#include "DataTable.h"
#include <map>
#include "../GameObject/Item/Item.h"

class ItemTable : public DataTable
{
public:
	enum class Locked
	{
		Locked,
		Unlocked,
	};
protected:
	map<Locked, map<int, Item::Info>> table;

public:
	ItemTable();
	virtual ~ItemTable();

	const Item::Info& Get(int id);
	const map<int, Item::Info> GetInfoList(Locked locked);
	const map<Locked, map<int, Item::Info>> GetTable() { return table; }

	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};

