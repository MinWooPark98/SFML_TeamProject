#include "GoodsSkill.h"
#include "../../Framework/ResourceMgr.h"
#include "../../DataTable/DataTableMGR.h"
#include "../../DataTable/SkillSetTable.h"
#include "../../DataTable/SkillSetIntroTable.h"
#include "../../Scene/SceneMgr.h"
#include "../Player.h"
#include "../TextObj.h"

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
	skillIcon.SetTexture(*RESOURCE_MGR->GetTexture(data.iconDir));
	skillIcon.SetOrigin(Origins::MC);
	auto introTable = DATATABLE_MGR->Get<SkillSetIntroTable>(DataTable::Types::SkillSetIntro);
	SetGoodsName(name);
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
					skillSet->Set(goodsName);
					SetActive(false);
					return;
				}
			}
			player->SetExtraSkillSet(goodsName);
		}
		break;
	case Goods::Payment::Platinum:
		auto table = DATATABLE_MGR->Get<SkillSetTable>(DataTable::Types::SkillSet);
		table->Unlock(goodsName);
		table->Load();
		break;
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
