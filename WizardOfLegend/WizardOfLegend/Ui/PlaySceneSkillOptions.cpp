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
	frame->SetTexture(*RESOURCE_MGR->GetTexture("graphics/ItemInfoPanel.png"));
	frame->SetColor(Color(50, 50, 50, 255));
	frame->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.5f });
	frame->SetOrigin(Origins::MC);
	frame->SetScale({ 9, 16 });
	frame->SetUI(true);
	selectSkillSet.push_back(frame);

	TextObj* optionText = new TextObj();
	optionText->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	optionText->SetSize(40);
	optionText->SetFillColor(Color::White);
	optionText->SetOutlineColor(Color::Black);
	optionText->SetOutlineThickness(2.f);
	optionText->SetText("P A U S E");
	optionText->SetOrigin(Origins::MC);
	optionText->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.15f });
	optionText->SetUI(true);
	selectSkillSet.push_back(optionText);

	Object* border = new Object();
	border->SetHitBox({ 0.f, 0.f, frame->GetSize().x * 2.5f, 2.f }, Color::White);
	border->SetHitBoxOrigin(Origins::MC);
	border->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.25f });
	border->SetUI(true);
	selectSkillSet.push_back(border);

	vector<string> sceneNames = { "SKILL1 : ", "SKILL2 : " ,"SKILL3 : ", "SKILL4 : ", "SKILL5 : ", "SKILL6 : " };
	for (int i = 0; i < sceneNames.size(); ++i)
	{
		TextObj* text = new TextObj();
		text->Init();
		text->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
		text->SetText(sceneNames[i]);
		text->SetSize(35);
		text->SetFillColor(Color::White);
		text->SetPos({ windowSize.x * 0.4f, windowSize.y * (0.3f + 0.1f * i) });
		text->SetOrigin(Origins::MR);
		text->SetUI(true);
		selectSkillSet.push_back(text);

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
