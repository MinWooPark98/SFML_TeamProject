#include "Relic.h"
#include "../Player.h"
#include "ItemMgr.h"
#include "../../Scene/SceneMgr.h"
#include "../../DataTable/DataTableMGR.h"
#include "../../DataTable/RelicTable.h"

Relic::Relic()
	:Item(Types::Relic), applyValues(true)
{
}

void Relic::Update(float dt)
{
	if (player != nullptr)
		return;
	switch (relicInfo.condition)
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
		break;
	}
}

void Relic::CheckHpLower()
{
	if (player->GetCurHp() <= player->GetMaxHp() * relicInfo.conditionValue)
		applyValues = true;
	else
		applyValues = false;
}

void Relic::CheckHpUpper()
{
	if (player->GetCurHp() >= player->GetMaxHp() * relicInfo.conditionValue)
		applyValues = true;
	else
		applyValues = false;
}

void Relic::CheckNumOfItems()
{
	auto itemMgr = (ItemMgr*)SCENE_MGR->GetCurrentScene()->FindGameObj("ITEM_MGR");
	auto table = DATATABLE_MGR->Get<RelicTable>(DataTable::Types::Relic);
	auto& itemInfo = table->Get(relicInfo.id);
	relicInfo.values = itemInfo.values * itemMgr->GetRelicList().size();
}

void Relic::CheckOnHit()
{
	if (player->GetState() == Player::States::Hit)
		applyValues = true;
	else
		applyValues = false;
}

void Relic::CheckWhileDashing()
{
	if (player->GetState() == Player::States::Dash)
		applyValues = true;
	else
		applyValues = false;
}