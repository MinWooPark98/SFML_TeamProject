#include "SkillBookUi.h"
#include "../GameObject/SpriteObj.h"
#include "SkillBookButton.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/FrameWork.h"
#include "../GameObject/TextObj.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/Player.h"
#include "../Scene/SceneMgr.h"
#include "../GameObject/SkillSet.h"

SkillBookUi::SkillBookUi()
	:collection(nullptr), skillVecIdx(0), isMoving(true), moveSpeed(3600.f), state(States::SkillOption)
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

		SpriteObj* skill = new SpriteObj();
		skill->Init();
		skill->SetScale({ 3.75f, 4.f });

		options.push_back({ button, skill });
	}
	options[0].first->SetButtonName("����");
	options[1].first->SetButtonName("����");
	options[2].first->SetButtonName("ǥ��");
	options[3].first->SetButtonName("�ñ״�ó");

	collection = new SkillBookButton();
	collection->Init();
	collection->SetOption("graphics/SpellBookOpen2_2.png");
	collection->SetButtonName("��� �Ƹ�ī��");
	collection->SetTextPlace(SkillBookButton::TextPlace::Aside);
	collection->HighLightOnFunc = bind(&SkillBookButton::SetOption, collection, "graphics/ArcanaAllSkillsFocused.png");
	collection->HighLightOffFunc = bind(&SkillBookButton::SetOption, collection, "graphics/SpellBookOpen2_2.png");

	for (int i = 0; i < 2; ++i)
	{
		SpriteObj* icon = new SpriteObj();
		icon->Init();
		icon->SetScale({ 3.75f, 3.75f });
		TextObj* text = new TextObj();
		text->Init();
		text->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
		text->SetSize(22);
		text->SetFillColor(Color::White);
		infos.push_back({ icon, text });
	}
	infos[0].first->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Space.png"));
	infos[0].first->SetOrigin(Origins::MC);
	infos[0].second->SetString("����");
	infos[0].second->AsciiToUnicode();
	infos[0].second->SetOrigin(Origins::ML);
	infos[1].first->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Escape.png"));
	infos[1].first->SetOrigin(Origins::MC);
	infos[1].second->SetString("������");
	infos[1].second->AsciiToUnicode();
	infos[1].second->SetOrigin(Origins::ML);

	Object::Init();
	SetActive(false);
}
	

void SkillBookUi::Reset()
{
	Object::Reset();
}

void SkillBookUi::Release()
{
	Object::Release();
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
	auto inputV = InputMgr::GetAxisDown(Axis::Vertical);
	if (inputV > 0 && state == States::SkillOption)
	{
		collection->HighLightOn();
		options[skillVecIdx].first->HighLightOff();
		state = States::Collection;
	}
	else if (inputV < 0 && state == States::Collection)
	{
		collection->HighLightOff();
		options[skillVecIdx].first->HighLightOn();
		state = States::SkillOption;
	}

	auto inputH = InputMgr::GetAxisDown(Axis::Horizontal);
	if (state == States::SkillOption)
	{
		if (inputH > 0 && skillVecIdx < options.size() - 1)
		{
			options[skillVecIdx].first->HighLightOff();
			++skillVecIdx;
			options[skillVecIdx].first->HighLightOn();
		}
		else if (inputH < 0 && skillVecIdx > 0)
		{
			options[skillVecIdx].first->HighLightOff();
			--skillVecIdx;
			options[skillVecIdx].first->HighLightOn();
		}
	}
}

void SkillBookUi::Draw(RenderWindow& window)
{
	Object::Draw(window);
	for (auto panel : panels)
	{
		panel->Draw(window);
	}
	for (auto& option : options)
	{
		option.first->Draw(window);
		option.second->Draw(window);
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
		options[i].first->SetPos({ skillPanelBnd.left + skillPanelBnd.width * (0.155f + 0.23f * i), skillPanelBnd.top + skillPanelBnd.height * 0.45f });
		options[i].second->SetPos(options[i].first->GetPos());
	}
	collection->SetPos({ collectionPanelBnd.left + collectionPanelBnd.width * 0.15f, collectionPanelBnd.top + collectionPanelBnd.height * 0.5f });
	for (int i = 0; i < infos.size(); ++i)
	{
		infos[i].first->SetPos({ collectionPanelBnd.left + collectionPanelBnd.width * 0.7f, collectionPanelBnd.top + collectionPanelBnd.height * (0.3f + 0.4f * i) });
		auto infoIconBnd = infos[i].first->GetGlobalBounds();
		infos[i].second->SetPos({ infoIconBnd.left + infoIconBnd.width + 20.f, infos[i].first->GetPos().y });
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
	{
		skillVecIdx = 0;
		options[skillVecIdx].first->HighLightOn();
		collection->HighLightOff();
		state = States::SkillOption;
		auto player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
		auto& skillSets = player->GetSkillSets();
		options[0].second->SetTexture(*RESOURCE_MGR->GetTexture(skillSets[0]->GetIconDir()));
		options[1].second->SetTexture(*RESOURCE_MGR->GetTexture(skillSets[1]->GetIconDir()));
		options[2].second->SetTexture(*RESOURCE_MGR->GetTexture(skillSets[4]->GetIconDir()));
		options[3].second->SetTexture(*RESOURCE_MGR->GetTexture(skillSets[5]->GetIconDir()));
		for (auto& option : options)
		{
			option.second->SetOrigin(Origins::MC);
		}
		Reappear();
	}
}

void SkillBookUi::Reappear()
{
	isMoving = true;
	auto& windowSize = FRAMEWORK->GetWindowSize();
	if (!panels.empty())
		SetPos(Vector2f(windowSize.x * 0.025f, windowSize.y));
}
