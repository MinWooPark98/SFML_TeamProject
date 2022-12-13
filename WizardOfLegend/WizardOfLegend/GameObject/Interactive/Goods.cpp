#include "Goods.h"
#include "../../Framework/ResourceMgr.h"
#include "../TextObj.h"
#include "../../Scene/SceneMgr.h"
#include "../Player.h"

Goods::Goods(Payment payment, Types type)
	:forSale(false), priceText(nullptr), price(0), payment(payment), type(type), infoPanel(nullptr), goodsNameText(nullptr), goodsInfoText(nullptr)
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
	priceText = new TextObj();
	priceText->Init();
	priceText->SetSize(20.f);
	priceText->GetSFMLText().setScale({ 0.25f, 0.25f });
	priceText->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	priceText->SetFillColor(Color::Black);

	infoPanel = new SpriteObj();
	infoPanel->Init();
	infoPanel->SetTexture(*RESOURCE_MGR->GetTexture("graphics/ItemInfoPanel.png"));
	infoPanel->SetOrigin(Origins::BC);
	infoPanel->SetActive(false);

	goodsNameText = new TextObj();
	goodsNameText->Init();
	goodsNameText->SetSize(40.f);
	goodsNameText->GetSFMLText().setScale({ 0.25f, 0.25f });
	goodsNameText->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	goodsNameText->SetFillColor(Color::White);
	goodsNameText->SetActive(false);

	goodsInfoText = new TextObj();
	goodsInfoText->Init();
	goodsInfoText->SetSize(24.f);
	goodsInfoText->GetSFMLText().setScale({ 0.25f, 0.25f });
	goodsInfoText->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	goodsInfoText->SetFillColor(Color::White);
	goodsInfoText->SetActive(false);

	SetOrigin(Origins::BC);

	SetInteractBox({ 14.f, 12.f });
	SetInteractBoxOrigin(Origins::BC);
	SetInteractKeyPos({ 0.f, -60.f });

	Interact = bind(&Goods::Sale, this);

	SetHitBox(FloatRect(0.f, 0.f, 40.f, 40.f));
	Utils::SetOrigin(hitbox, Origins::BC);
	//interactKey->SetActive(false);
}

void Goods::Draw(RenderWindow& window)
{
	if (infoPanel->GetActive())
		infoPanel->Draw(window);
	if (goodsNameText->GetActive())
		goodsNameText->Draw(window);
	if (goodsInfoText->GetActive())
		goodsInfoText->Draw(window);
	Interactive::Draw(window);
	priceText->Draw(window);
}

void Goods::Approach()
{
	Interactive::Approach();
	infoPanel->SetActive(true);
	goodsNameText->SetActive(true);
	goodsInfoText->SetActive(true);
}

void Goods::ApproachEnd()
{
	Interactive::ApproachEnd();
	infoPanel->SetActive(false);
	goodsNameText->SetActive(false);
	goodsInfoText->SetActive(false);
}

void Goods::ForSale(bool sale)
{
	forSale = sale;
}

void Goods::Sale()
{
	if (!forSale)
		return;
	auto player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
	switch (payment)
	{
	case Goods::Payment::Gold:
		if (player->GetCurGold() > price)
		{
			player->SubGold(price);
			Saled();
		}
		break;
	case Goods::Payment::Platinum:
		if (player->GetCurPlatinum() > price)
		{
			player->SubPlatinum(price);
			Saled();
		}
		break;
	default:
		break;
	}
}

void Goods::SetGoodsName(const string& name)
{
	goodsName = name;
	goodsNameText->SetString(goodsName);
	goodsNameText->AsciiToUnicode();
	goodsNameText->SetOrigin(Origins::MC);
}

void Goods::SetGoodsInfo(const string& info)
{
	goodsInfo = info;
	goodsInfoText->SetString(goodsInfo);
	goodsInfoText->AsciiToUnicode();
	goodsInfoText->SetOrigin(Origins::MC);
}

void Goods::SetPrice(int price)
{
	this->price = price;
	priceText->SetString(to_string(price));
	priceText->SetOrigin(Origins::MC);
}

void Goods::SetPos(const Vector2f& pos)
{
	Interactive::SetPos(pos);
	auto interactKeyBnd = interactKey->GetGlobalBounds();
	auto tagBnd = GetGlobalBounds();
	priceText->SetPos({ tagBnd.left + tagBnd.width * 0.65f, tagBnd.top + tagBnd.height * 0.5f });
	infoPanel->SetPos({ interactKeyBnd.left + interactKeyBnd.width * 0.5f, interactKeyBnd.top + interactKeyBnd.height * 0.5f });
	auto infoPanelBnd = infoPanel->GetGlobalBounds();
	goodsNameText->SetPos({ infoPanelBnd.left + infoPanelBnd.width * 0.5f, infoPanelBnd.top + infoPanelBnd.height * 0.15f });
	auto goodsNameTextBnd = goodsNameText->GetGlobalBounds();
	goodsInfoText->SetPos({ goodsNameTextBnd.left + goodsNameTextBnd.width * 0.5f, goodsNameTextBnd. top + goodsNameTextBnd.height + 7.5f });
}
