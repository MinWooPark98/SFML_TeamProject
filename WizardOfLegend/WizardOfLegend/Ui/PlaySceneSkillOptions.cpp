#include "PlaySceneSkillOptions.h"
#include "../GameObject/TextObj.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/Framework.h"
#include "../GameObject/SpriteObj.h"
#include "../GameObject/Button2.h"
#include "../Scene/SceneMgr.h"
#include "DataTableList.h"
#include "../GameObject/Player.h"
#include "../GameObject/SkillSet.h"

PlaySceneSkillOptions::PlaySceneSkillOptions()
	:vecIdx(-1)
{
}

PlaySceneSkillOptions::~PlaySceneSkillOptions()
{
}

void PlaySceneSkillOptions::Init()
{
	Object::Init();
	SetActive(false);
	auto& windowSize = FRAMEWORK->GetWindowSize();

	SpriteObj* frame = new SpriteObj();
	frame->Init();
	frame->SetTexture(*RESOURCE_MGR->GetTexture("graphics/ItemInfoPanel.png"));
	frame->SetColor(Color(50, 50, 50, 255));
	frame->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.5f });
	frame->SetOrigin(Origins::MC);
	frame->SetScale({ 9, 16 });
	frame->SetUI(true);
	selectSkillSet.push_back(frame);

	TextObj* optionText = new TextObj();
	optionText->Init();
	optionText->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	optionText->SetSize(40);
	optionText->SetFillColor(Color::White);
	optionText->SetOutlineColor(Color::Black);
	optionText->SetOutlineThickness(2.f);
	optionText->SetText("SKILL SET");
	optionText->SetOrigin(Origins::MC);
	optionText->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.15f });
	optionText->SetUI(true);
	selectSkillSet.push_back(optionText);

	border.setSize({ frame->GetSize().x * 8.f, 2.f });
	border.setFillColor(Color::White);
	Utils::SetOrigin(border, Origins::MC);
	border.setPosition({ windowSize.x * 0.5f, windowSize.y * 0.2f });

	vector<pair<string, string>> skillSetNames = { { "   SKILL1 : ", "LeftMouse" }, { "  SKILL2 : ", "SpaceBar" }, { "   SKILL3 : ", "RightMouse" }, { "   SKILL4 : ", "Q" }, { "   SKILL5 : ", "E" }, { "   SKILL6 : ", "R" } };
	for (int i = 0; i < skillSetNames.size(); ++i)
	{
		TextObj* text = new TextObj();
		text->Init();
		text->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
		text->SetText(skillSetNames[i].first);
		text->SetSize(35);
		text->SetFillColor(Color::White);
		text->SetPos({ windowSize.x * 0.4f, windowSize.y * (0.3f + 0.1f * i) });
		text->SetOrigin(Origins::MR);
		text->SetUI(true);
		selectSkillSet.push_back(text);

		TextObj* text2 = new TextObj();
		text2->Init();
		text2->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
		text2->SetText(skillSetNames[i].second);
		text2->SetSize(20);
		text2->SetFillColor({ 255, 255, 255, 153 });
		text2->SetPos({ text->GetGlobalBounds().left + text->GetGlobalBounds().width * 0.5f, windowSize.y * (0.3f + 0.1f * i) - 25.f });
		text2->SetOrigin(Origins::MC);
		text2->SetUI(true);
		selectSkillSet.push_back(text2);

		Button2* button = new Button2();
		button->Init();
		button->UseText();
		button->SetText("fonts/NotoSansKR-Bold.otf", 35, Color(255, 255, 255, 153), "---");
		button->SetButtonBox(button->GetText()->GetSFMLText().getGlobalBounds(), Color::Transparent);
		button->GetText()->SetOutlineColor(Color::Black);
		button->GetText()->SetOutlineThickness(2.f);
		button->SetPos({ windowSize.x * 0.5f, windowSize.y * (0.3f + 0.1f * i) });
		button->MousePointerOn = bind(&TextObj::SetFillColor, button->GetText(), Color::White);
		button->MousePointerOff = bind(&TextObj::SetFillColor, button->GetText(), Color(255, 255, 255, 153));
		button->ClickOn = bind(&PlaySceneSkillOptions::SelectSkillSet, this, i);
		button->SetOrigin(Origins::ML);
		button->SetUI(true);
		currSkillSets.push_back(button);

		DataTableList* skillSets = new DataTableList();
		skillSets->Init();
		skillSets->SetDataTable(DataTable::Types::SkillSet);
		skillSets->Selected = bind(&PlaySceneSkillOptions::SetPlayerSkillSet, this, placeholders::_1);
		skillSetLists.push_back(skillSets);
	}
}

void PlaySceneSkillOptions::Release()
{
	Object::Release();
	for (auto obj : selectSkillSet)
	{
		obj->Release();
	}
	for (auto obj : skillSetLists)
	{
		obj->Release();
	}
	for (auto obj : currSkillSets)
	{
		obj->Release();
	}
}

void PlaySceneSkillOptions::Update(float dt)
{
	Object::Update(dt);
	if (InputMgr::GetKeyDown(Keyboard::Escape))
		SetActive(false);
	for (auto obj : selectSkillSet)
	{
		obj->Update(dt);
	}
	for (auto obj : currSkillSets)
	{
		obj->Update(dt);
	}
	Player* player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
	for (int i = 0; i < currSkillSets.size(); ++i)
	{
		const string& str = player->GetSkillSets()[i]->GetSkillSetName();
		if (str.empty())
		{
			currSkillSets[i]->GetText()->SetString("---");
			currSkillSets[i]->SetButtonBox({0.f, 0.f, 50.f, 35.f}, Color::Transparent);
		}
		else
		{
			currSkillSets[i]->GetText()->SetString(str);
			currSkillSets[i]->SetButtonBox(currSkillSets[i]->GetText()->GetSFMLText().getGlobalBounds(), Color::Transparent);
		}
	}
}

void PlaySceneSkillOptions::Draw(RenderWindow& window)
{
	Object::Draw(window);
	for (auto obj : selectSkillSet)
	{
		obj->Draw(window);
	}
	window.draw(border);
	for (auto obj : currSkillSets)
	{
		obj->Draw(window);
	}
}

void PlaySceneSkillOptions::SelectSkillSet(int idx)
{
	vecIdx = idx;
	skillSetLists[idx]->SetActive(true);
}

void PlaySceneSkillOptions::SetPlayerSkillSet(const string& skillSetName)
{
	Player* player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
	player->GetSkillSets()[vecIdx]->Set(skillSetName);
}
