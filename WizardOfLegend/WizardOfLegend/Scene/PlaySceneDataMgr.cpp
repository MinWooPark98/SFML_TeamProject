#include "PlaySceneDataMgr.h"
#include "SceneMgr.h"
#include "../GameObject/Player.h"
#include "../GameObject/Item/ItemMgr.h"
#include "../GameObject/Item/Hood.h"
#include "../GameObject/Item/Relic.h"
#include "../GameObject/SkillSet.h"
#include "PlayScene.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/SavedDataTable.h"

PlaySceneDataMgr::PlaySceneDataMgr()
{
	data.skillSetNames.assign(6, "");
}

PlaySceneDataMgr::~PlaySceneDataMgr()
{
}

void PlaySceneDataMgr::Save()
{
	auto player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
	SaveHoodId(player->GetItemMgr()->GetHood()->GetHoodInfo().id);
	auto& relicList = player->GetItemMgr()->GetRelicList();
	data.relicIds.clear();
	for (auto relic : relicList)
	{
		data.relicIds.push_back(relic->GetRelicInfo().id);
	}
	const vector<SkillSet*>& skillSets = player->GetSkillSets();
	for (int i = 0; i < skillSets.size(); ++i)
	{
		data.skillSetNames[i] = skillSets[i]->GetSkillSetName();
	}
	data.currHp = player->GetCurHp();
	data.gold = player->GetCurGold();
}

void PlaySceneDataMgr::Load()
{
	auto currScene = (PlayScene*)SCENE_MGR->GetCurrentScene();
	if (currScene->GetMapName() == "TUTORIALMAP")
	{
		data.hoodId = 200;
		data.relicIds.clear();
		for (auto& skillSetName : data.skillSetNames)
		{
			skillSetName.clear();
		}
		data.currHp = 525;
		data.gold = 0;
	}
	else if (currScene->GetMapName() == "TEST")	// ±¤Àå¸ÊÀ¸·Î ÀÌ¸§ ¹Ù²Ù¼À
	{
		auto& savedData = DATATABLE_MGR->Get<SavedDataTable>(DataTable::Types::SavedData)->Get();
		data.hoodId = savedData.hoodId;
		data.relicIds.clear();
		data.relicIds.push_back(savedData.relicId);
		for (auto& skillSetName : data.skillSetNames)
		{
			skillSetName.clear();
		}
		for (int i = 0; i < savedData.skillIds.size(); ++i)
		{
			data.skillSetNames[i < 2 ? i : i + 2] = savedData.skillIds[i];
		}
	}
	auto player = (Player*)currScene->FindGameObj("PLAYER");
	auto itemMgr = player->GetItemMgr();
	itemMgr->SetHood(data.hoodId);
	itemMgr->ClearRelicList();
	for (auto relicId : data.relicIds)
	{
		itemMgr->AddRelic(relicId);
	}
	auto& skillSets = player->GetSkillSets();
	for (int i = 0; i < data.skillSetNames.size(); ++i)
	{
		if (data.skillSetNames[i].empty())
			skillSets[i]->ResetSkills();
		else
			player->SetSkillSet(i, data.skillSetNames[i], true);
	}
	if (currScene->GetMapName() == "TEST")
	{
		itemMgr->Apply();
		player->SetCurHp(player->GetMaxHp());
	}
	else
		player->SetCurHp(data.currHp);
	player->SetCurGold(data.gold);
}
