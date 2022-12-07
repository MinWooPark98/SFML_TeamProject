#include "SkillBookUi.h"
#include "../GameObject/SpriteObj.h"
#include "SkillBookButton.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/FrameWork.h"
#include "../GameObject/TextObj.h"
#include "../Framework/InputMgr.h"

SkillBookUi::SkillBookUi()
	:collection(nullptr), skillVecIdx(0), isMoving(true), moveSpeed(400.f)
{
}

SkillBookUi::~SkillBookUi()
{
}

void SkillBookUi::Init()
{
	for (int i = 0; i < 2; ++i)
	{
		SpriteObj* newPanel = new SpriteObj();
		newPanel->Init();
		newPanel->SetTexture(*RESOURCE_MGR->GetTexture("graphics/ItemInfoPanel.png"));
		newPanel->SetColor(Color(50, 50, 50, 220));
		newPanel->SetScale({ 6.f, 3.f * (2 - i)});
		panels.push_back(newPanel);
	}

	for (int i = 0; i < 4; ++i)
	{
		SkillBookButton* button = new SkillBookButton();
		button->Init();
		button->SetOption("graphics/ArcanaLargeFront.png");
		button->SetHighLight("graphics/ArcanaLargeHighlight.png");
		options.push_back(button);

	}
	options[0]->SetName("기초");
	options[1]->SetName("돌진");
	options[2]->SetName("표준");
	options[3]->SetName("시그니처");

	collection = new SkillBookButton();
	collection->Init();
	collection->SetOption("graphics/ArcanaAllSkillsFocused.png");
	collection->HighLightOnFunc = bind(&SkillBookButton::SetOption, collection, "graphics/SpellBookOpen7.png");
	collection->HighLightOffFunc = bind(&SkillBookButton::SetOption, collection, "graphics/SpellBookOpen2_2.png");

	for (int i = 0; i < 2; ++i)
	{
		SpriteObj* icon = new SpriteObj();
		icon->Init();
		icon->SetScale({ 4.f, 4.f });
		TextObj* text = new TextObj();
		text->Init();
		text->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
		text->SetSize(25);
		text->SetFillColor(Color::White);
		infos.push_back({ icon, text });
	}
	infos[0].first->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Space.png"));
	infos[0].first->SetOrigin(Origins::MC);
	infos[0].second->SetString("선택");
	infos[0].second->AsciiToUnicode();
	infos[0].second->SetOrigin(Origins::ML);
	infos[1].first->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Escape.png"));
	infos[1].first->SetOrigin(Origins::MC);
	infos[1].second->SetString("나가기");
	infos[1].second->AsciiToUnicode();
	infos[1].second->SetOrigin(Origins::ML);
}
	

void SkillBookUi::Reset()
{
	Object::Reset();
	isMoving = true;
	auto& windowSize = FRAMEWORK->GetWindowSize();
	if (!panels.empty())
		SetPos(Vector2f(windowSize.x * 0.025f, windowSize.y));
}

void SkillBookUi::Release()
{
}

void SkillBookUi::Update(float dt)
{
	Object::Update(dt);
	if (isMoving)
	{
		Translate(Vector2f(0.f, -1.f) * moveSpeed * dt);
		auto& windowSize = FRAMEWORK->GetWindowSize();
		if (position.y < windowSize.y * 0.3f)
			isMoving = false;
		return;
	}
	auto inputVer = InputMgr::GetAxisRaw(Axis::Vertical);
	if (inputVer > 0.f && !collection->GetHighLightOn())
	{
		collection->HighLightOn();
		options[skillVecIdx]->HighLightOff();
	}
	else if (inputVer < 0.f && !options[skillVecIdx]->GetHighLightOn() )
	{
		collection->HighLightOff();
		options[skillVecIdx]->HighLightOn();
	}
}

void SkillBookUi::Draw(RenderWindow& window)
{
	Object::Draw(window);
	for (auto panel : panels)
	{
		panel->Draw(window);
	}
	for (auto option : options)
	{
		option->Draw(window);
	}
	collection->Draw(window);
	for (auto& pair : infos)
	{
		pair.first->Draw(window);
		pair.second->Draw(window);
	}
}

void SkillBookUi::Reposition()
{
	auto skillPanelBnd = panels[0]->GetGlobalBounds();
	panels[0]->SetPos(position);
	auto collectionPanelBnd = panels[1]->GetGlobalBounds();
	panels[1]->SetPos(position + Vector2f(0.f, skillPanelBnd.height + 50.f));

	for (int i = 0; i < options.size(); ++i)
	{
		options[i]->SetPos({ skillPanelBnd.left + skillPanelBnd.width * (0.125f + 0.25f * i), skillPanelBnd.top + skillPanelBnd.height * 0.45f });
	}
	collection->SetPos({ collectionPanelBnd.left + collectionPanelBnd.width * 0.2f, collectionPanelBnd.top + collectionPanelBnd.height * 0.5f });
	for (int i = 0; i < infos.size(); ++i)
	{
		infos[i].first->SetPos({ collectionPanelBnd.left + collectionPanelBnd.width * 0.7f, collectionPanelBnd.top + collectionPanelBnd.height * (0.25f + 0.5f * i) });
		auto infoIconBnd = infos[i].first->GetGlobalBounds();
		infos[i].second->SetPos({ infoIconBnd.left + infoIconBnd.width + 40.f, infos[i].first->GetPos().y });
	}
}

void SkillBookUi::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	Reposition();
}

void SkillBookUi::Translate(const Vector2f& delta)
{
	Object::Translate(delta);
	Reposition();
}

void SkillBookUi::SetActive(bool active)
{
	Object::SetActive(active);
	if (active)
		Reset();
}
