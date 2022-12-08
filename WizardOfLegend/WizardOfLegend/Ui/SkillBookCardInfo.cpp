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

SkillBookCardInfo::SkillBookCardInfo()
	:frame(nullptr), element(0), skillIdx(0), currPlayerSkillSetIdx(0), isMoving(true), moveSpeed(3600.f)
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

		if (skillInfos[element].empty())
		{
			element = lastElem;
			return;
		}
		else
			skillIdx = 0;
	}
	if (inputH != 0)
	{
		if (inputH < 0 && skillIdx > 0)
			--skillIdx;
		else if (inputH > 0 && skillIdx < (int)skillInfos[element].size() - 1)
			++skillIdx;
		else
			return;
	}

	SetDrawinCards();
}

void SkillBookCardInfo::Draw(RenderWindow& window)
{
	Object::Draw(window);
	frame->Draw(window);
	for (auto& card : drawingCards)
	{
		card.first->Draw(window);
		card.second->Draw(window);
	}
}

void SkillBookCardInfo::Reposition()
{
	frame->SetPos(position);
	auto frameBnd = frame->GetGlobalBounds();
	cards[0].first->SetPos({ frameBnd.left + frameBnd.width * 0.245f, frameBnd.top + frameBnd.height * 0.4f });
	cards[1].first->SetPos({ frameBnd.left + frameBnd.width * 0.33f, frameBnd.top + frameBnd.height * 0.385f });
	cards[2].first->SetPos({ frameBnd.left + frameBnd.width * 0.495f, frameBnd.top + frameBnd.height * 0.37f });
	cards[3].first->SetPos({ frameBnd.left + frameBnd.width * 0.66f, frameBnd.top + frameBnd.height * 0.385f });
	cards[4].first->SetPos({ frameBnd.left + frameBnd.width * 0.745f, frameBnd.top + frameBnd.height * 0.4f });
	for (auto& card : cards)
	{
		card.second->SetPos(card.first->GetPos());
	}
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
	Disappear();
	playerSkillSets[currPlayerSkillSetIdx]->Set(skillInfos[element][skillIdx].first);
	if (ChangeSkillBookUi != nullptr)
		ChangeSkillBookUi();
}

void SkillBookCardInfo::SetActive(bool active)
{
	Object::SetActive(active);
	if (!active)
	{
		if (Deactivate != nullptr)
			Deactivate();
		return;
	}
	element = 0;
	skillIdx = 0;
	drawingCards.clear();
	playerSkillSets.clear();
	skillInfos.clear();

	auto skillSetData = DATATABLE_MGR->Get<SkillSetTable>(DataTable::Types::SkillSet);
	auto skillData = DATATABLE_MGR->Get<SkillTable>(DataTable::Types::Skill);
	auto player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
	auto& skillSets = player->GetSkillSets();
	playerSkillSets.push_back(skillSets[0]);
	playerSkillSets.push_back(skillSets[1]);
	playerSkillSets.push_back(skillSets[4]);
	playerSkillSets.push_back(skillSets[5]);
	for (int i = 0; i < (int)Skill::Element::Count; ++i)
	{
		skillInfos.push_back(vector<pair<string, string>>());
		auto data_elem = skillSetData->Get((Skill::Element)i);
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
	SetDrawinCards();
	Reappear();
}
