#include "SkillBookUi.h"
#include "../GameObject/SpriteObj.h"
#include "KeyButton.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/FrameWork.h"
#include "../GameObject/TextObj.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/Player.h"
#include "../Scene/SceneMgr.h"
#include "../GameObject/SkillSet.h"
#include "SkillBookCardInfo.h"
#include "../GameObject/Interactive/SkillBook.h"
#include "../DataTable/SavedDataTable.h"
#include "../DataTable/DataTableMGR.h"

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
		KeyButton* button = new KeyButton();
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
	options[0].first->SetButtonName("����");
	options[1].first->SetButtonName("����");
	options[2].first->SetButtonName("ǥ��");
	options[3].first->SetButtonName("�ñ״�ó");

	collection = new KeyButton();
	collection->Init();
	collection->SetOption("graphics/SpellBookOpen2_2.png");
	collection->SetButtonName("��� �Ƹ�ī��");
	collection->SetTextPlace(KeyButton::TextPlace::Aside);
	collection->HighLightOnFunc = bind(&KeyButton::SetOption, collection, "graphics/ArcanaAllSkillsFocused.png");
	collection->HighLightOffFunc = bind(&KeyButton::SetOption, collection, "graphics/SpellBookOpen2_2.png");

	for (int i = 0; i < 2; ++i)
	{
		SpriteObj* icon = new SpriteObj();
		icon->Init();
		icon->SetScale({ 3.75f, 3.75f });
		TextObj* text = new TextObj();
		text->Init();
		text->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
		text->SetSize(22);
		text->SetFillColor(Color(155, 155, 155, 255));
		keyInfos.push_back({ icon, text });
	}
	keyInfos[0].first->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Space.png"));
	keyInfos[0].first->SetOrigin(Origins::MC);
	keyInfos[0].second->SetString("����");
	keyInfos[0].second->AsciiToUnicode();
	keyInfos[0].second->SetOrigin(Origins::ML);
	keyInfos[1].first->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Escape.png"));
	keyInfos[1].first->SetOrigin(Origins::MC);
	keyInfos[1].second->SetString("������");
	keyInfos[1].second->AsciiToUnicode();
	keyInfos[1].second->SetOrigin(Origins::ML);

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
	for (auto& pair : keyInfos)
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
	for (int i = 0; i < keyInfos.size(); ++i)
	{
		keyInfos[i].first->SetPos({ collectionPanelBnd.left + collectionPanelBnd.width * 0.7f, collectionPanelBnd.top + collectionPanelBnd.height * (0.3f + 0.4f * i) });
		auto infoIconBnd = keyInfos[i].first->GetGlobalBounds();
		keyInfos[i].second->SetPos({ infoIconBnd.left + infoIconBnd.width + 20.f, keyInfos[i].first->GetPos().y });
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
	auto currScene = SCENE_MGR->GetCurrentScene();
	if (active)
	{
		InputMgr::StackedOrderAdd(this);
		skillVecIdx = 0;
		options[skillVecIdx].first->HighLightOn();
		collection->HighLightOff();
		state = States::SkillOption;
		auto player = (Player*)currScene->FindGameObj("PLAYER");
		auto& skillSets = player->GetSkillSets();
		playerSkillSets.push_back(skillSets[0]);
		playerSkillSets.push_back(skillSets[1]);
		playerSkillSets.push_back(skillSets[4]);
		playerSkillSets.push_back(skillSets[5]);
		for (int i = 0; i < options.size(); ++i)
		{
			if(!playerSkillSets[i]->GetSkillSetName().empty())
				options[i].second->SetTexture(*RESOURCE_MGR->GetTexture(playerSkillSets[i]->GetIconDir()));
		}
		for (auto& option : options)
		{
			option.second->SetOrigin(Origins::MC);
		}
		Reappear();
		((SkillBook*)currScene->FindGameObj("SKILLBOOK"))->SetState(SkillBook::States::OpenReady);
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
		auto skillBook = ((SkillBook*)currScene->FindGameObj("SKILLBOOK"));
		if(skillBook != nullptr)
			skillBook->SetState(SkillBook::States::Close);
		vector<string> skillSetNames;
		for (auto skillSet : playerSkillSets)
		{
			skillSetNames.push_back(skillSet->GetSkillSetName());
		}
		DATATABLE_MGR->Get<SavedDataTable>(DataTable::Types::SavedData)->ChangeSkills(skillSetNames);
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
	options[skillVecIdx].second->SetOrigin(Origins::MC);
}
