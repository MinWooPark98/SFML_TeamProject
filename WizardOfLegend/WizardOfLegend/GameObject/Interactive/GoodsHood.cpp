#include "GoodsHood.h"
#include "../../Framework/ResourceMgr.h"
#include "../../DataTable/DataTableMGR.h"
#include "../../DataTable/HoodTable.h"

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
	mannequin.SetScale({ 3.f, 3.f });
	mannequin.SetTexture(*RESOURCE_MGR->GetTexture("graphics/MannequinDisplay.png"));

	hoodDisplay.Init();
	hoodDisplay.SetSpriteShader();
	hoodDisplay.SetSpritePalette(64, "graphics/WizardPalette.png");
	hoodDisplay.SetScale({ 3.f, 3.f });
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
	hoodDisplay.SetTexture(*RESOURCE_MGR->GetTexture("graphics/WizardOutfitMannequin.png"));
	hoodDisplay.SetSpriteColor(info.paletteIdx);
	ForSale(true);
}

void GoodsHood::Saled()
{
	switch (payment)
	{
	case Goods::Payment::Gold:
		break;
	case Goods::Payment::Platinum:
		DATATABLE_MGR->Get<HoodTable>(DataTable::Types::Hood)->Unlock(info.id);
		SetActive(false);
		break;
	default:
		break;
	}
}
