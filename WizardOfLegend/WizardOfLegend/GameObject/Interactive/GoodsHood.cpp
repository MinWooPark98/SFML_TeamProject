#include "GoodsHood.h"
#include "../../Framework/ResourceMgr.h"
#include "../../DataTable/DataTableMGR.h"
#include "../../DataTable/HoodTable.h"
#include "../TextObj.h"
#include "../../Scene/SceneMgr.h"
#include "../Item/ItemMgr.h"
#include "../Player.h"

GoodsHood::GoodsHood(Payment payment)
	:Goods(payment, Types::Hood)
{
}

GoodsHood::~GoodsHood()
{
}

void GoodsHood::Init()
{
	Goods::Init();
	mannequin.Init();
	mannequin.SetTexture(*RESOURCE_MGR->GetTexture("graphics/MannequinDisplay.png"));
	mannequin.SetOrigin(Origins::BC);

	hoodDisplay.Init();
	hoodDisplay.UseShader();
	hoodDisplay.SetSpriteShader();
	hoodDisplay.SetSpritePalette(64, "graphics/WizardPalette.png");
	hoodDisplay.SetOrigin(Origins::BC);
}

void GoodsHood::Draw(RenderWindow& window)
{
	Goods::Draw(window);
	mannequin.Draw(window);
	hoodDisplay.Draw(window);
}

void GoodsHood::SetInfo(const Hood::HoodInfo& info)
{
	this->info = info;
	mannequin.SetTexture(*RESOURCE_MGR->GetTexture("graphics/MannequinDisplayForOutfit.png"));
	mannequin.SetOrigin(Origins::BC);
	mannequin.SetPos({ position.x, position.y - 10.f });
	hoodDisplay.SetTexture(*RESOURCE_MGR->GetTexture("graphics/WizardOutfitMannequin.png"));
	hoodDisplay.SetPaletteColor(info.paletteIdx);
	hoodDisplay.SetOrigin(Origins::BC);
	hoodDisplay.SetPos(mannequin.GetPos());
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
	ForSale(true);
}

void GoodsHood::Saled()
{
	switch (payment)
	{
	case Goods::Payment::Gold:
		((Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER"))->GetItemMgr()->SetHood(info.id);
		break;
	case Goods::Payment::Platinum:
		DATATABLE_MGR->Get<HoodTable>(DataTable::Types::Hood)->Unlock(info.id);
		SetActive(false);
		break;
	default:
		break;
	}
}

void GoodsHood::SetPos(const Vector2f& pos)
{
	Goods::SetPos(pos);
	mannequin.SetPos({ pos.x, pos.y - 10.f });
	hoodDisplay.SetPos(mannequin.GetPos());
}
