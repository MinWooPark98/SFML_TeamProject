#include "GoodsRelic.h"
#include "../../Framework/ResourceMgr.h"
#include "../../DataTable/DataTableMGR.h"
#include "../../DataTable/RelicTable.h"
#include "../../Scene/SceneMgr.h"
#include "../Player.h"
#include "../Item/ItemMgr.h"

GoodsRelic::GoodsRelic(Payment payment)
	:Goods(payment, Types::Relic)
{
}

GoodsRelic::~GoodsRelic()
{
}

void GoodsRelic::Init()
{
	Goods::Init();
	relicDisplay.Init();
	relicDisplay.SetScale({ 3.f, 3.f });
}

void GoodsRelic::Draw(RenderWindow& window)
{
	Goods::Draw(window);
	relicDisplay.Draw(window);
}

void GoodsRelic::SetInfo(const Relic::RelicInfo& info)
{
	this->info = info;
	relicDisplay.SetTexture(*RESOURCE_MGR->GetTexture(info.iconDir));
	ForSale(true);
}

void GoodsRelic::Saled()
{
	switch (payment)
	{
	case Goods::Payment::Gold:
		((Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER"))->GetItemMgr()->AddRelic(info.id);
		break;
	case Goods::Payment::Platinum:
		DATATABLE_MGR->Get<RelicTable>(DataTable::Types::Relic)->Unlock(info.id);
		SetActive(false);
		break;
	default:
		break;
	}
}
