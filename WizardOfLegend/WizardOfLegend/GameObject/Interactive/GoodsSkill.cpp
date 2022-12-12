#include "GoodsSkill.h"
#include "../../Framework/ResourceMgr.h"
#include "../../DataTable/DataTableMGR.h"
#include "../../DataTable/SkillSetTable.h"
#include "../../DataTable/SkillSetIntroTable.h"
#include "../../Scene/SceneMgr.h"
#include "../Player.h"

GoodsSkill::GoodsSkill(Payment payment)
	:Goods(payment, Types::Relic)
{
}

GoodsSkill::~GoodsSkill()
{
}

void GoodsSkill::Init()
{
	Goods::Init();
	card.Init();
	card.SetTexture(*RESOURCE_MGR->GetTexture("graphics/ArcanaPickupFront.png"));
	card.SetScale({ 3.f, 3.f });

	skillIcon.Init();
	skillIcon.SetScale({ 3.f, 3.f });
}

void GoodsSkill::Draw(RenderWindow& window)
{
	card.Draw(window);
	skillIcon.Draw(window);
}

void GoodsSkill::SetInfo(const string& name)
{
	auto table = DATATABLE_MGR->Get<SkillSetTable>(DataTable::Types::SkillSet);
	skillIcon.SetTexture(*RESOURCE_MGR->GetTexture(table->Get(name).iconDir));
	auto introTable = DATATABLE_MGR->Get<SkillSetIntroTable>(DataTable::Types::SkillSetIntro);
	skillName = name;
	skillIntro = introTable->Get(name).skillIntro_Kor;
	ForSale(true);
}

void GoodsSkill::Saled()
{
	switch (payment)
	{
	case Goods::Payment::Gold:
		{
			auto player = ((Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER"));
			auto& skillSets = player->GetSkillSets();
			for (auto skillSet : skillSets)
			{
				if (skillSet->GetSkillSetName().empty())
				{
					skillSet->Set(skillName);
					return;
				}
			}
			player->SetExtraSkillSet(skillName);
		}
		break;
	case Goods::Payment::Platinum:
		DATATABLE_MGR->Get<SkillSetTable>(DataTable::Types::SkillSet)->Unlock(skillName);
		SetActive(false);
		break;
	default:
		break;
	}
}
