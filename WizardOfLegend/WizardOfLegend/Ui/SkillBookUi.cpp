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
#include "SkillBookCardInfo.h"

SkillBookUi::SkillBookUi()
	:collection(nullptr), skillVecIdx(0), skillInfo(nullptr), isMoving(true), moveSpeed(5400.f), state(States::SkillOption)
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
		button->HighLightOnFunc = bind(&SkillBookUi::OptionHighLightOn, this, i);
		button->HighLightOffFunc = bind(&SkillBookUi::OptionHighLightOff, this, i);

		SpriteObj* skill = new SpriteObj();
		skill->Init();
		skill->SetScale({ 3.75f, 4.f });
		
		options.push_back({ button, skill });
		OptionHighLightOff(i);
	}
	options[0].first->SetButtonName("기초");
	options[1].first->SetButtonName("돌진");
	options[2].first->SetButtonName("표준");
	options[3].first->SetButtonName("시그니처");

	collection = new SkillBookButton();
	collection->Init();
	collection->SetOption("graphics/SpellBookOpen2_2.png");
	collection->SetButtonName("모든 아르카나");
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
	infos[0].second->SetString("선택");
	infos[0].second->AsciiToUnicode();
	infos[0].second->SetOrigin(Origins::ML);
	infos[1].first->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Escape.png"));
	infos[1].first->SetOrigin(Origins::MC);
	infos[1].second->SetString("나가기");
	infos[1].second->AsciiToUnicode();
	infos[1].second->SetOrigin(Origins::ML);

	skillInfo = new SkillBookCardInfo();
	skillInfo->Init();
	skillInfo->Deactivate = bind(&SkillBookUi::Reappear, this);
	skillInfo->ChangeSkillBookUi = bind(&SkillBookUi::ResetCurrOptionIcon, this);

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

	if (skillInfo != nullptr && skillInfo->GetActive())
	{
		skillInfo->Update(dt);
		if (!isMoving)
			return;
	}

	if (InputMgr::GetKeyDown(Keyboard::Escape))
	{
		SetActive(false);
		return;
	}

	if (isMoving)
	{
		Translate(direction * moveSpeed * dt);
		auto& windowSize = FRAMEWORK->GetWindowSize();
		if (direction.y < 0.f && position.y < windowSize.y * 0.3f)
		{
			isMoving = false;
		}
		else if (direction.y > 0.f && position.y > windowSize.y)
		{
			isMoving = false;
		}
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

	if (InputMgr::GetKeyDown(Keyboard::Space))
	{
		switch (state)
		{
		case SkillBookUi::States::SkillOption:
			skillInfo->SetCurrPlayerSkillSetIdx(skillVecIdx);
			skillInfo->SetActive(true);
			Disappear();
			break;
		case SkillBookUi::States::Collection:
			break;
		default:
			break;
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
	if (skillInfo != nullptr && skillInfo->GetActive())
		skillInfo->Draw(window);
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
		InputMgr::StackedOrderAdd(this);
		skillVecIdx = 0;
		options[skillVecIdx].first->HighLightOn();
		collection->HighLightOff();
		state = States::SkillOption;
		auto player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
		auto& skillSets = player->GetSkillSets();
		playerSkillSets.push_back(skillSets[0]);
		playerSkillSets.push_back(skillSets[1]);
		playerSkillSets.push_back(skillSets[4]);
		playerSkillSets.push_back(skillSets[5]);
		for (int i = 0; i < options.size(); ++i)
		{
			options[i].second->SetTexture(*RESOURCE_MGR->GetTexture(playerSkillSets[i]->GetIconDir()));
		}
		for (auto& option : options)
		{
			option.second->SetOrigin(Origins::MC);
		}
		Reappear();
	}
	else
	{
		InputMgr::StackedOrderRemove(this);
		for (auto& option : options)
		{
			if(option.first->GetHighLightOn())
				option.first->HighLightOff();
		}
		if(collection->GetHighLightOn())
			collection->HighLightOff();
	}
}

void SkillBookUi::Reappear()
{
	isMoving = true;
	auto& windowSize = FRAMEWORK->GetWindowSize();
	if (!panels.empty())
		SetPos(Vector2f(windowSize.x * 0.025f, windowSize.y));
	direction = { 0.f, -1.f };
}

void SkillBookUi::Disappear()
{
	isMoving = true;
	direction = { 0.f, 1.f };
}

void SkillBookUi::OptionHighLightOn(int idx)
{
	options[idx].first->GetOption()->SetColor(Color(255, 255, 255, 255));
	options[idx].second->SetColor(Color(255, 255, 255, 255));
}

void SkillBookUi::OptionHighLightOff(int idx)
{
	options[idx].first->GetOption()->SetColor(Color(100, 100, 100, 255));
	options[idx].second->SetColor(Color(100, 100, 100, 255));
}

void SkillBookUi::ResetCurrOptionIcon()
{
	options[skillVecIdx].second->SetTexture(*RESOURCE_MGR->GetTexture(playerSkillSets[skillVecIdx]->GetIconDir()));
}
