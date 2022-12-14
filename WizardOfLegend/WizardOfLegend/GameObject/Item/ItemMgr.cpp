#include "ItemMgr.h"
#include "../../DataTable/DataTableMGR.h"
#include "../../DataTable/RelicTable.h"
#include "../../DataTable/HoodTable.h"
#include "../../Scene/SceneMgr.h"
#include "../Player.h"
#include "../../DataTable/StatTable.h"
#include "Relic.h"
#include "Hood.h"
#include "../../Ui/UiMgr.h"

ItemMgr::ItemMgr()
	:hood(nullptr), player(nullptr)
{
}

ItemMgr::~ItemMgr()
{
}

void ItemMgr::SetHood(int id)
{
	if (hood == nullptr)
		hood = new Hood();
	auto table = DATATABLE_MGR->Get<HoodTable>(DataTable::Types::Hood);
	const auto& hoodInfo = table->Get(id);
	hood->SetHoodInfo(hoodInfo);
	player->SetPaletteColor(hoodInfo.paletteIdx);
	((SpriteObj*)SCENE_MGR->GetCurrentScene()->GetUiMgr()->FindUiObj("PLAYERSTATUSPORTRAIT"))->SetPaletteColor(hoodInfo.paletteIdx);
	

	totalValues.Reset();
	totalValues += hood->GetValues();

	for (auto relic : relicList)
	{
		if (relic->GetApplyValues())
			totalValues += relic->GetValues();
	}
	Apply();
	player->SetCurHp(player->GetMaxHp());
}

void ItemMgr::AddRelic(int id)
{
	if (id == -1)
		return;
	auto table = DATATABLE_MGR->Get<RelicTable>(DataTable::Types::Relic);
	const auto& relicInfo = table->Get(id);
	Relic* newItem = new Relic();
	newItem->SetRelicInfo(relicInfo);
	newItem->SetPlayer(player);
	relicList.push_back(newItem);
}

void ItemMgr::ChangeRelic(int id, int idx)
{
	auto table = DATATABLE_MGR->Get<RelicTable>(DataTable::Types::Relic);
	const auto& relicInfo = table->Get(id);
	relicList[idx]->SetRelicInfo(relicInfo);
}

void ItemMgr::Update(float dt)
{
	totalValues.Reset();
	if (hood != nullptr)
		totalValues += hood->GetValues();

	for (auto relic : relicList)
	{
		relic->Update(dt);
		if (relic->GetApplyValues())
			totalValues += relic->GetValues();

	}
	Apply();
}

void ItemMgr::Apply()
{
	auto statTable = DATATABLE_MGR->Get<StatTable>(DataTable::Types::Stat);
	auto& stat = statTable->Get("Player");
	player->SetSpeed(stat.speed * (1 + totalValues.speed));
	player->SetAtkDmg(stat.attackDmg * (1 + totalValues.atkDmg));
	player->SetDamageTake(stat.damageTake + totalValues.dmg);
	player->SetMaxHp(stat.maxHp * (1 + totalValues.maxHp));
	player->SetEvasionRate(stat.evasionRate + totalValues.evasionRate);
	player->SetCriticalRate(stat.criticalRate + totalValues.criticalRate);
	player->SetCriticalRatio(stat.criticalRatio + totalValues.criticalRatio);
}

void ItemMgr::Clear()
{
	if (hood != nullptr)
	{
		delete hood;
		hood = nullptr;
	}
	for (auto item : relicList)
	{
		delete item;
	}
	relicList.clear();
	totalValues.Reset();
}

void ItemMgr::Load()
{
	Clear();
	// 로드 부분 추가
}
