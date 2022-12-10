#pragma once
#include "Item.h"
#include <vector>

class ItemMgr
{
private:
	vector<Item*> itemList;

	Item::Values totalValues;

public:
	ItemMgr();
	~ItemMgr();

	void AddItem(int id);
	vector<Item*> GetList() { return itemList; }

	void Update(float dt);

	void Clear();
	void Load();
};

