#include "RelicUi.h"
#include "../GameObject/Button2.h"
#include "../GameObject/SpriteObj.h"
#include "../GameObject/TextObj.h"
#include "../Framework/ResourceMgr.h"

RelicUi::RelicUi()
	:infoPanel(nullptr), relicName(nullptr), relicInfo(nullptr)
{
}

RelicUi::~RelicUi()
{
}

void RelicUi::Init()
{
	Object::Init();

	infoPanel = new SpriteObj();
	infoPanel->Init();
	infoPanel->SetTexture(*RESOURCE_MGR->GetTexture("graphics/ItemInfoPanel.png"));
	infoPanel->SetScale({ 4.f, 3.f });
	infoPanel->SetOrigin(Origins::BR);
	infoPanel->SetActive(false);

	relicName = new TextObj();
	relicName->Init();
	relicName->SetSize(100.f);
	relicName->GetSFMLText().setScale({ 0.25f, 0.25f });
	relicName->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	relicName->SetFillColor(Color::White);
	relicName->SetActive(false);

	relicInfo = new TextObj();
	relicInfo->Init();
	relicInfo->SetSize(75.f);
	relicInfo->GetSFMLText().setScale({ 0.25f, 0.25f });
	relicInfo->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	relicInfo->SetFillColor(Color::White);
	relicInfo->SetActive(false);
}

void RelicUi::Reset()
{
	Object::Reset();
}

void RelicUi::Release()
{
	Object::Release();
}

void RelicUi::Update(float dt)
{
	Object::Update(dt);
	for (auto& relic : display)
	{
		relic.second->Update(dt);
	}
}

void RelicUi::Draw(RenderWindow& window)
{
	Object::Draw(window);
	for (auto& relic : display)
	{
		relic.second->Draw(window);
	}
	if (infoPanel->GetActive())
		infoPanel->Draw(window);
	if (relicName->GetActive())
		relicName->Draw(window);
	if (relicInfo->GetActive())
		relicInfo->Draw(window);
}

void RelicUi::AddRelicUi(const Relic::RelicInfo& info)
{
	Relic::RelicInfo newInfo = info;
	Button2* newButton = new Button2();
	newButton->Init();
	newButton->SetOrigin(Origins::BR);
	newButton->UseSprite();
	newButton->SetSprite(info.iconDir);
	newButton->GetSprite()->SetSize({ 48.f, 48.f });
	newButton->GetSprite()->SetOrigin(Origins::MC);
	newButton->SetHitBox(newButton->GetSprite()->GetGlobalBounds(), Color(0, 0, 0, 155));
	newButton->SetHitBoxOrigin(Origins::MC);
	newButton->SetUI(true);
	newButton->SetDevMode(true);
	newButton->MousePointerOn = bind(&RelicUi::ShowInfo, this, display.size());
	newButton->MousePointerOff = bind(&RelicUi::HideInfo, this);
	display.push_back({ newInfo, newButton });

	Reposition();
}

void RelicUi::ChangeRelicUi(const Relic::RelicInfo& info, int idx)
{
	display[idx].first = info;
	display[idx].second->SetSprite(info.iconDir);
	display[idx].second->GetSprite()->SetOrigin(Origins::MC);
	display[idx].second->SetHitBoxOrigin(Origins::MC);
}

void RelicUi::Reposition()
{
	for (int i = 0; i < display.size(); ++i)
	{
		display[i].second->SetPos({ position.x - (display.size() - i - 1) * 52.f, position.y});
	}
	infoPanel->SetPos(position + Vector2f(24.f, -50.f));
	auto panelBnd = infoPanel->GetGlobalBounds();
	relicName->SetPos({ panelBnd.left + panelBnd.width * 0.5f, panelBnd.top + panelBnd.height * 0.1f });
	relicInfo->SetPos({ panelBnd.left + panelBnd.width * 0.1f, panelBnd.top + panelBnd.height * 0.4f });
}

void RelicUi::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	Reposition();
}

void RelicUi::Translate(const Vector2f& delta)
{
	Object::Translate(delta);
	Reposition();
}

void RelicUi::ShowInfo(int idx)
{
	auto& info = display[idx].first;
	relicName->SetString(info.name);
	relicName->AsciiToUnicode();
	relicName->SetOrigin(Origins::TC);
	relicInfo->SetString(info.intro);
	relicInfo->AsciiToUnicode();
	relicInfo->SetOrigin(Origins::TL);
	infoPanel->SetActive(true);
	relicName->SetActive(true);
	relicInfo->SetActive(true);
}

void RelicUi::HideInfo()
{
	infoPanel->SetActive(false);
	relicName->SetActive(false);
	relicInfo->SetActive(false);
}
