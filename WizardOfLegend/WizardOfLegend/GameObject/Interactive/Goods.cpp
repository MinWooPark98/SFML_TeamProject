#include "Goods.h"
#include "../../Framework/ResourceMgr.h"

Goods::Goods(Payment payment, Types type)
	:forSale(false), payment(payment), type(type)
{
}

Goods::~Goods()
{
}

void Goods::Init()
{
	Interactive::Init();
	switch (payment)
	{
	case Payment::Gold:
		sprite.setTexture(*RESOURCE_MGR->GetTexture("graphics/PriceMarkerGold.png"));
		break;
	case Payment::Platinum:
		sprite.setTexture(*RESOURCE_MGR->GetTexture("graphics/PriceMarkerPlat.png"));
		break;
	default:
		break;
	}
	SetOrigin(Origins::BC);

	SetHitBox(FloatRect(0.f, 0.f, 22.f, 10.f));
	SetHitBoxOrigin(Origins::BC);

	interactKey->SetActive(false);
}

void Goods::ForSale(bool sale)
{
	forSale = sale;
	if (sale)
		interactKey->SetActive(true);
}
