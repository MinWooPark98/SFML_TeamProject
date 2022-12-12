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
	relicDisplay.SetOrigin(Origins::BC);
	ForSale(true);
	switch (payment)
	{
	case Goods::Payment::Gold:
		SetPrice(info.goldPrice);
		break;
	case Goods::Payment::Platinum:
		SetPrice(info.platinumPrice);
		break;
	default:
		break;
	}
	SetGoodsName(info.name);
	SetGoodsInfo(info.intro);
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
		break;
	default:
		break;
	}
	SetActive(false);
}

void GoodsRelic::SetPos(const Vector2f& pos)
{
	Goods::SetPos(pos);
	relicDisplay.SetPos(pos - Vector2f(0.f, 10.f));
}
