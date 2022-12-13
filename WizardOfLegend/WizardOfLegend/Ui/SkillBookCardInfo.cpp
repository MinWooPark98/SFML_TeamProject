#include "SkillBookCardInfo.h"
#include "../GameObject/SpriteObj.h"
#include "../GameObject/TextObj.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/FrameWork.h"
#include "../Framework/InputMgr.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/SkillSetTable.h"
#include "../Scene/SceneMgr.h"
#include "../DataTable/SkillTable.h"
#include "KeyButton.h"
#include "../DataTable/SkillSetIntroTable.h"
#include "../GameObject/Interactive/SkillBook.h"

SkillBookCardInfo::SkillBookCardInfo()
	:frame(nullptr), element(0), skillIdx(0), currPlayerSkillSetIdx(0), isMoving(true), moveSpeed(7200.f), skillName(nullptr), skillIntro(nullptr)
{
}

SkillBookCardInfo::~SkillBookCardInfo()
{
}

void SkillBookCardInfo::Init()
{
	Object::Init();
	frame = new SpriteObj();
	frame->Init();
	frame->SetTexture(*RESOURCE_MGR->GetTexture("graphics/SpellbookUICardInfo.png"));
	frame->SetOrigin(Origins::MC);
	frame->SetScale({ 3.75f, 4.f });
	frame->SetUI(true);

	for (int i = 0; i < 5; ++i)
	{
		SpriteObj* card = new SpriteObj();
		card->Init();
		card->SetTexture(*RESOURCE_MGR->GetTexture("graphics/ArcanaLargeFront.png"));
		card->SetOrigin(Origins::MC);
		card->SetScale({ 3.75f, 4.f });
		card->SetUI(true);

		SpriteObj* icon = new SpriteObj();
		icon->Init();
		icon->SetScale({ 3.75f, 4.f });

		cards.push_back({ card, icon });
	}

	vector<pair<string, string>> elementBtns = { {"graphics/SpellBookTabFire.png", "graphics/SpellBookTabFireSelected.png"}, {"graphics/SpellBookTabAir.png", "graphics/SpellBookTabAirSelected.png"}, {"graphics/SpellBookTabEarth.png", "graphics/SpellBookTabEarthSelected.png"}, {"graphics/SpellBookTabLightning.png", "graphics/SpellBookTabLightningSelected.png"}, {"graphics/SpellBookTabWater.png", "graphics/SpellBookTabWaterSelected.png"}, {"graphics/SpellBookTabChaos.png", "graphics/SpellBookTabChaosSelected.png"} };
	vector<string> elemIconDirs = { "graphics/SpellbookUIEleIconFireSel.png", "graphics/SpellbookUIEleIconAirSel.png", "graphics/SpellbookUIEleIconEarthSel.png", "graphics/SpellbookUIEleIconLightningSel.png", "graphics/SpellbookUIEleIconWaterSel.png", "graphics/SpellbookUIEleIconChaosSel.png" };
	vector<string> elemPageDirs = { "graphics/SpellbookUIFirePage.png", "graphics/SpellbookUIAirPage.png", "graphics/SpellbookUIEarthPage.png", "graphics/SpellbookUILightningPage.png", "graphics/SpellbookUIWaterPage.png" };
	for (int i = 0; i < 6; ++i)
	{
		KeyButton* newButton = new KeyButton();
		newButton->Init();
		newButton->GetOption()->SetScale({ 4.f, 4.f });
		newButton->GetHighLight()->SetScale({ 4.f, 4.f });
		newButton->SetOption(elementBtns[i].first);
		newButton->HighLightOnFunc = bind(&KeyButton::SetOption, newButton, elementBtns[i].second);
		newButton->HighLightOffFunc = bind(&KeyButton::SetOption, newButton, elementBtns[i].first);
		showElements.push_back(newButton);

		SpriteObj* elemIcon = new SpriteObj();
		elemIcon->Init();
		elemIcon->SetTexture(*RESOURCE_MGR->GetTexture(elemIconDirs[i]));
		elemIcon->SetOrigin(Origins::MC);
		elemIcon->SetScale({ 3.75f, 3.75f });
		elemIcons.push_back(elemIcon);

		if (i < 5)
		{
			SpriteObj* elemPage = new SpriteObj();
			elemPage->Init();
			elemPage->SetTexture(*RESOURCE_MGR->GetTexture(elemPageDirs[i]));
			elemPage->SetOrigin(Origins::TC);
			elemPage->SetScale({ 3.75f, 4.f });
			elemPages.push_back(elemPage);
		}
	}

	skillName = new TextObj();
	skillName->Init();
	skillName->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	skillName->SetSize(35);
	skillName->SetFillColor(Color(57, 40, 21, 255));

	skillIntro = new TextObj();
	skillIntro->Init();
	skillIntro->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	skillIntro->SetSize(25);
	skillIntro->SetFillColor(Color(57, 40, 21, 255));

	SetActive(false);
}

void SkillBookCardInfo::Reset()
{
	Object::Reset();
}

void SkillBookCardInfo::Release()
{
	Object::Release();
}

void SkillBookCardInfo::Update(float dt)
{
	Object::Update(dt);
	if (isMoving)
	{
		Translate(direction * moveSpeed * dt);
		auto& windowSize = FRAMEWORK->GetWindowSize();
		if (direction.x < 0.f && position.x < -windowSize.x * 0.25f)
		{
			isMoving = false;
			SetActive(false);
		}
		else if (direction.x > 0.f && position.x > windowSize.x * 0.25f)
			isMoving = false;
		return;
	}
	if (InputMgr::GetKeyDown(Keyboard::Space))
		ChangeSkill();
	if (InputMgr::GetKeyDown(Keyboard::Escape))
		Disappear();

	auto inputV = InputMgr::GetAxisDown(Axis::Vertical);
	auto inputH = InputMgr::GetAxisDown(Axis::Horizontal);
	if (inputV == 0 && inputH == 0)
		return;

	if (inputV != 0)
	{
		int lastElem = element;
		element += inputV;
		if (element < 0)
			element = (int)skillInfos.size() - 1;
		else if (element > (int)skillInfos.size() - 1)
			element = 0;
		skillIdx = 0;
	}
	SetElementHighLight();

	if (inputH != 0)
	{
		if (inputH < 0 && skillIdx > 0)
			--skillIdx;
		else if (inputH > 0 && skillIdx < (int)skillInfos[element].size() - 1)
			++skillIdx;
	}
	SetDrawinCards();
	SetSkillIntro();
}

void SkillBookCardInfo::Draw(RenderWindow& window)
{
	Object::Draw(window);
	frame->Draw(window);
	if (element < 5)
		elemPages[element]->Draw(window);
	elemIcons[element]->Draw(window);
	for (auto& card : drawingCards)
	{
		card.first->Draw(window);
		card.second->Draw(window);
	}
	for (auto& showElement : showElements)
	{
		showElement->Draw(window);
	}
	skillName->Draw(window);
	skillIntro->Draw(window);
}

void SkillBookCardInfo::Reposition()
{
	frame->SetPos(position);
	auto frameBnd = frame->GetGlobalBounds();
	for (int i = 0; i < elemPages.size(); ++i)
	{
		Vector2f pos = { frameBnd.left + frameBnd.width * 0.5f, frameBnd.top };
		if (i == 3)
			elemPages[i]->SetPos({ pos.x, pos.y + 30.f });
		else
			elemPages[i]->SetPos(pos);
	}
	cards[0].first->SetPos({ frameBnd.left + frameBnd.width * 0.245f, frameBnd.top + frameBnd.height * 0.4f });
	cards[1].first->SetPos({ frameBnd.left + frameBnd.width * 0.33f, frameBnd.top + frameBnd.height * 0.385f });
	cards[2].first->SetPos({ frameBnd.left + frameBnd.width * 0.495f, frameBnd.top + frameBnd.height * 0.37f });
	cards[3].first->SetPos({ frameBnd.left + frameBnd.width * 0.66f, frameBnd.top + frameBnd.height * 0.385f });
	cards[4].first->SetPos({ frameBnd.left + frameBnd.width * 0.745f, frameBnd.top + frameBnd.height * 0.4f });
	for (auto& card : cards)
	{
		card.second->SetPos(card.first->GetPos());
	}

	for (int i = 0; i < showElements.size(); ++i)
	{
		showElements[i]->SetPos({ frameBnd.left + frameBnd.width * 0.98f, frameBnd.top + frameBnd.height * (0.15f + i * 0.125f) });
	}
	for (auto elemIcon : elemIcons)
	{
		elemIcon->SetPos({ frameBnd.left + frameBnd.width * 0.49f, frameBnd.top + frameBnd.height * 0.185f });
	}

	skillName->SetPos({ frameBnd.left + frameBnd.width * 0.5f, frameBnd.top + frameBnd.height * 0.535f });
	skillIntro->SetPos({ frameBnd.left + frameBnd.width * 0.125f, frameBnd.top + frameBnd.height * 0.595f });
} 

void SkillBookCardInfo::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	Reposition();
}

void SkillBookCardInfo::Translate(const Vector2f& delta)
{
	Object::Translate(delta);
	Reposition();
}

void SkillBookCardInfo::Reappear()
{
	isMoving = true;
	auto& windowSize = FRAMEWORK->GetWindowSize();
	if (frame != nullptr)
		SetPos(Vector2f(-windowSize.x * 0.25f, windowSize.y * 0.5f));
	direction = { 1.f, 0.f };
}

void SkillBookCardInfo::Disappear()
{
	isMoving = true;
	direction = { -1.f, 0.f };
}

void SkillBookCardInfo::SetElementHighLight()
{
	for (int i = 0; i < showElements.size(); ++i)
	{
		if (i != element)
		{
			if (showElements[i]->GetHighLightOn())
				showElements[i]->HighLightOff();
		}
		else if (!showElements[i]->GetHighLightOn())
			showElements[i]->HighLightOn();
	}
}

void SkillBookCardInfo::SetSkillIntro()
{
	if (skillInfos[element].empty())
	{
		skillName->SetString("");
		skillIntro->SetString("");
	}
	else
	{
		auto table = DATATABLE_MGR->Get<SkillSetIntroTable>(DataTable::Types::SkillSetIntro);
		auto& intro = table->Get(skillInfos[element][skillIdx].first);
		skillName->SetString(intro.skillName_Kor);
		skillName->AsciiToUnicode();
		skillName->SetOrigin(Origins::TC);
		skillIntro->SetString(intro.skillIntro_Kor);
		skillIntro->AsciiToUnicode();
		skillIntro->SetOrigin(Origins::TL);
	}
}

void SkillBookCardInfo::SetDrawinCards()
{
	drawingCards.clear();
	if (skillIdx > 1)
	{
		cards[0].second->SetTexture(*RESOURCE_MGR->GetTexture(skillInfos[element][skillIdx - 2].second));
		drawingCards.push_back(cards[0]);
	}
	if (skillIdx < (int)skillInfos[element].size() - 2)
	{
		cards[4].second->SetTexture(*RESOURCE_MGR->GetTexture(skillInfos[element][skillIdx + 2].second));
		drawingCards.push_back(cards[4]);
	}
	if (skillIdx > 0)
	{
		cards[1].second->SetTexture(*RESOURCE_MGR->GetTexture(skillInfos[element][skillIdx - 1].second));
		drawingCards.push_back(cards[1]);
	}
	if (skillIdx < (int)skillInfos[element].size() - 1)
	{
		cards[3].second->SetTexture(*RESOURCE_MGR->GetTexture(skillInfos[element][skillIdx + 1].second));
		drawingCards.push_back(cards[3]);
	}
	if (!skillInfos[element].empty())
	{
		cards[2].second->SetTexture(*RESOURCE_MGR->GetTexture(skillInfos[element][skillIdx].second));
		drawingCards.push_back(cards[2]);
	}
	for (auto& card : drawingCards)
	{
		card.second->SetOrigin(Origins::MC);
	}
}

void SkillBookCardInfo::ChangeSkill()
{
	if (skillIdx > (int)skillInfos[element].size() - 1)
		return;
	Disappear();
	auto player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
	player->SetSkillSet(currPlayerSkillSetIdx < 2 ? currPlayerSkillSetIdx : currPlayerSkillSetIdx + 2, skillInfos[element][skillIdx].first, true);
	if (ChangeSkillBookUi != nullptr)
		ChangeSkillBookUi();
}

void SkillBookCardInfo::SetActive(bool active)
{
	Object::SetActive(active);
	auto currScene = SCENE_MGR->GetCurrentScene();
	if (!active)
	{
		if (Deactivate != nullptr)
			Deactivate();
		auto skillBook = ((SkillBook*)currScene->FindGameObj("SKILLBOOK"));
		if(skillBook != nullptr)
			skillBook->SetState(SkillBook::States::CloseReady);
		return;
	}
	element = 0;
	skillIdx = 0;
	drawingCards.clear();
	playerSkillSets.clear();
	skillInfos.clear();

	auto skillSetData = DATATABLE_MGR->Get<SkillSetTable>(DataTable::Types::SkillSet);
	auto skillData = DATATABLE_MGR->Get<SkillTable>(DataTable::Types::Skill);
	auto player = (Player*)currScene->FindGameObj("PLAYER");
	auto& skillSets = player->GetSkillSets();
	playerSkillSets.push_back(skillSets[0]);
	playerSkillSets.push_back(skillSets[1]);
	playerSkillSets.push_back(skillSets[4]);
	playerSkillSets.push_back(skillSets[5]);
	for (int i = 0; i < (int)Skill::Element::Count; ++i)
	{
		skillInfos.push_back(vector<pair<string, string>>());
		map<string, SkillSetTable::SetInfo> data_elem;
		try
		{
			data_elem = skillSetData->Get(SkillSetTable::Locked::Unlocked, (Skill::Element)i);
		}
		catch (exception& expn)
		{
			continue;
		}
		for (auto& data : data_elem)
		{
			bool isExist = false;
			for (auto currSkillSet : playerSkillSets)
			{
				if (data.first == currSkillSet->GetSkillSetName())
				{
					isExist = true;
					break;
				}
			}
			if (isExist)
				continue;

			if (currPlayerSkillSetIdx == 1)
			{
				if (data.second.skillNames.size() > 1 || skillData->Get(data.second.skillNames.front()).playerAction != Player::SkillAction::Dash)
					continue;
			}
			else if (data.second.skillNames.size() == 1 && skillData->Get(data.second.skillNames.front()).playerAction == Player::SkillAction::Dash)
				continue;
			skillInfos[i].push_back({ data.first, data.second.iconDir });
		}
	}
	element = (int)playerSkillSets[currPlayerSkillSetIdx]->GetElement();
	SetElementHighLight();
	SetDrawinCards();
	SetSkillIntro();
	Reappear();
	((SkillBook*)currScene->FindGameObj("SKILLBOOK"))->SetState(SkillBook::States::Open);
}
