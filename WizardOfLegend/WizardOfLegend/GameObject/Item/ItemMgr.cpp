#include "ItemMgr.h"
#include "../../DataTable/DataTableMGR.h"
#include "../../DataTable/ItemTable.h"
#include "../../Scene/SceneMgr.h"
#include "../Player.h"

ItemMgr::ItemMgr()
{
}

ItemMgr::~ItemMgr()
{
}

void ItemMgr::AddItem(int id)
{
	auto table = DATATABLE_MGR->Get<ItemTable>(DataTable::Types::Item);
	auto& itemInfo = table->Get(id);
	Item* newItem = new Item();
	newItem->SetInfo(itemInfo);
	newItem->SetPlayer((Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER"));
	itemList.push_back(newItem);
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
