#pragma once
#include "Item.h"
#include <vector>

class Player;

class ItemMgr
{
private:
	vector<Item*> itemList;

	Item::Values totalValues;
	Player* player;

public:
	ItemMgr();
	~ItemMgr();

	void AddItem(int id);
	void AddItem(const Item::Info& info);
	void ChangeItem(int id, int idx);
	void ChangeItem(const Item::Info& info, int idx);
	vector<Item*> GetList() { return itemList; }

	void Update(float dt);

	void SetPlayer(Player* player) { this->player = player; }
	void Apply();

	void Clear();
	void Load();
};

