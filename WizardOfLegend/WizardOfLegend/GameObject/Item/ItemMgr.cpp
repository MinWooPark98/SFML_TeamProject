#include "ItemMgr.h"
#include "../../DataTable/DataTableMGR.h"
#include "../../DataTable/ItemTable.h"
#include "../../Scene/SceneMgr.h"
#include "../Player.h"
#include "../../DataTable/StatTable.h"

ItemMgr::ItemMgr()
	:player(nullptr)
{
}

ItemMgr::~ItemMgr()
{
}

void ItemMgr::AddItem(int id)
{
	auto table = DATATABLE_MGR->Get<ItemTable>(DataTable::Types::Item);
	const auto& itemInfo = table->Get(id);
	AddItem(itemInfo);
}

void ItemMgr::AddItem(const Item::Info& info)
{
	Item* newItem = new Item();
	newItem->SetInfo(info);
	newItem->SetPlayer((Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER"));
	itemList.push_back(newItem);
}

void ItemMgr::ChangeItem(int id, int idx)
{
	auto table = DATATABLE_MGR->Get<ItemTable>(DataTable::Types::Item);
	const auto& itemInfo = table->Get(id);
	ChangeItem(itemInfo, idx);
}

void ItemMgr::ChangeItem(const Item::Info& info, int idx)
{
	itemList[idx]->SetInfo(info);
}

void ItemMgr::Update(float dt)
{
	totalValues.Reset();
	for (auto item : itemList)
	{
		item->Update(dt);
		if (item->GetApplyValues())
			totalValues += item->GetValues();
	}
	Apply();
}

void ItemMgr::Apply()
{
	auto statTable = DATATABLE_MGR->Get<StatTable>(DataTable::Types::Stat);
	auto& stat = statTable->Get("Player");
	player->SetSpeed(stat.speed * (1 + totalValues.speed));
	player->SetAtkDmg(stat.attackDmg * (1 + totalValues.atkDmg));
	player->SetDamageTake(stat.damageTake * (1 - totalValues.dmg));
	player->SetMaxHp(stat.maxHp * (1 + totalValues.maxHp));
	player->SetEvasionRate(stat.evasionRate * (1 + totalValues.evasionRate));
	player->SetCriticalRate(stat.criticalRate * (1 + totalValues.criticalRate));
	player->SetCriticalRatio(stat.criticalRatio * (1 + totalValues.criticalRatio));
}

void ItemMgr::Clear()
{
	for (auto item : itemList)
	{
		delete item;
	}
	itemList.clear();
	totalValues.Reset();
}

void ItemMgr::Load()
{
	Clear();
	// 로드 부분 추가
}
