#include "GoodsSkill.h"
#include "../../Framework/ResourceMgr.h"
#include "../../DataTable/DataTableMGR.h"
#include "../../DataTable/SkillSetTable.h"
#include "../../DataTable/SkillSetIntroTable.h"
#include "../../Scene/SceneMgr.h"
#include "../Player.h"
#include "../TextObj.h"
#include "../../DataTable/SkillTable.h"

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
	card.SetOrigin(Origins::BC);

	skillIcon.Init();
}

void GoodsSkill::Draw(RenderWindow& window)
{
	Goods::Draw(window);
	card.Draw(window);
	skillIcon.Draw(window);
}

void GoodsSkill::SetInfo(const string& name)
{
	auto table = DATATABLE_MGR->Get<SkillSetTable>(DataTable::Types::SkillSet);
	auto& data = table->Get(name);
	skillKey = name;
	skillIcon.SetTexture(*RESOURCE_MGR->GetTexture(data.iconDir));
	skillIcon.SetOrigin(Origins::MC);
	auto introTable = DATATABLE_MGR->Get<SkillSetIntroTable>(DataTable::Types::SkillSetIntro);
	SetGoodsName(introTable->Get(name).skillName_Kor);
	SetGoodsInfo(introTable->Get(name).skillIntro_Kor);
	ForSale(true);
	switch (payment)
	{
	case Goods::Payment::Gold:
		SetPrice(data.goldPrice);
		break;
	case Goods::Payment::Platinum:
		SetPrice(data.platinumPrice);
		break;
	default:
		break;
	}
}

void GoodsSkill::Saled()
{
	auto table = DATATABLE_MGR->Get<SkillSetTable>(DataTable::Types::SkillSet);
	switch (payment)
	{
	case Goods::Payment::Gold:
		{
			auto player = ((Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER"));
			auto& skillSets = player->GetSkillSets();
			auto skillData = DATATABLE_MGR->Get<SkillTable>(DataTable::Types::Skill);
			bool isDashSkill = table->Get(skillKey).skillNames.size() == 1 && skillData->Get(table->Get(skillKey).skillNames.front()).playerAction == Player::SkillAction::Dash;
			for (int i = 0; i < skillSets.size(); ++i)
			{
				if (isDashSkill && i != 1)
					continue;
				if (!isDashSkill && i == 1)
					continue;
				if (skillSets[i]->GetSkillSetName().empty())
				{
					player->SetSkillSet(i, skillKey, true);
					SetActive(false);
					return;
				}
			}
			player->AddExtraSkillSet(skillKey);
		}
		break;
	case Goods::Payment::Platinum:
		{
			table->Unlock(skillKey);
			table->Load();
			break;
		}
	default:
		break;
	}
	SetActive(false);
}

void GoodsSkill::SetPos(const Vector2f& pos)
{
	Goods::SetPos(pos);
	card.SetPos(pos - Vector2f(0.f, 10.f));
	auto cardBnd = card.GetGlobalBounds();
	skillIcon.SetPos({ cardBnd.left + cardBnd.width * 0.5f, cardBnd.top + cardBnd.height * 0.5f });
}
