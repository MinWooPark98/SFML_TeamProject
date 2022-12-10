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
	vector<Item*> GetList() { return itemList; }

	void Update(float dt);

	void SetPlayer(Player* player) { this->player = player; }
	void Apply();

	void Clear();
	void Load();
};

