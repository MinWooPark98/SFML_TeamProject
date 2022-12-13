#pragma once
#include "Item.h"
#include <vector>

class Player;
class Relic;
class Hood;

class ItemMgr
{
private:
	Hood* hood;
	vector<Relic*> relicList;

	Item::Values totalValues;
	Player* player;

public:
	ItemMgr();
	~ItemMgr();

	void SetHood(int id);
	Hood* GetHood() { return hood; }

	void AddRelic(int id);
	void ChangeRelic(int id, int idx);
	const vector<Relic*>& GetRelicList() { return relicList; }
	void ClearRelicList() { relicList.clear(); }

	void Update(float dt);

	void SetPlayer(Player* player) { this->player = player; }
	void Apply();

	void Clear();
	void Load();
};

