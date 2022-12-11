#include "Item.h"
#include "../Player.h"
#include "ItemMgr.h"
#include "../../Scene/SceneMgr.h"
#include "../../DataTable/DataTableMGR.h"
#include "../../DataTable/ItemTable.h"

Item::Item()
	:applyValues(true), player(nullptr)
{
}

Item::~Item()
{
}

void Item::Reset()
{
	applyValues = true;
}

void Item::SetInfo(const Info& info)
{
	Reset();
	this->info = info;
}

void Item::Update(float dt)
{
	if (player == nullptr)
		return;

	switch (info.condition)
	{
	case Condition::HpLower:
		CheckHpLower();
		break;
	case Condition::HpUpper:
		CheckHpUpper();
		break;
	case Condition::NumOfItems:
		CheckNumOfItems();
		break;
	case Condition::OnHit:
		CheckOnHit();
		break;
	case Condition::WhileDashing:
		CheckWhileDashing();
		break;
	default:
		return;
	}
}

void Item::CheckHpLower()
{
	if (player->GetCurHp() <= player->GetMaxHp() * info.conditionValue)
		applyValues = true;
	else
		applyValues = false;
}

void Item::CheckHpUpper()
{
	if (player->GetCurHp() >= player->GetMaxHp() * info.conditionValue)
		applyValues = true;
	else
		applyValues = false;
}

void Item::CheckNumOfItems()
{
	auto itemMgr = (ItemMgr*)SCENE_MGR->GetCurrentScene()->FindGameObj("ITEM_MGR");
	auto table = DATATABLE_MGR->Get<ItemTable>(DataTable::Types::Item);
	auto& itemInfo = table->Get(info.id);
	info.values = itemInfo.values * itemMgr->GetList().size();
}

void Item::CheckOnHit()
{
	if (player->GetState() == Player::States::Hit)
		applyValues = true;
	else
		applyValues = false;
}

void Item::CheckWhileDashing()
{
	if (player->GetState() == Player::States::Dash)
		applyValues = true;
	else
		applyValues = false;
}
