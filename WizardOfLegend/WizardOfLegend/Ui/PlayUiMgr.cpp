#include "PlayUiMgr.h"
#include "TitleUiMgr.h"
#include "../Scene/SceneMgr.h"
#include "../GameObject/Button2.h"
#include "../GameObject/TextObj.h"
#include "../Framework/Framework.h"
#include "../GameObject/SpriteObj.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/FireBoss.h"
#include "../GameObject/Player.h"
#include "../GameObject/HeavyBombingArcher.h"
#include "../GameObject/FinalBoss.h"
#include "PlaySceneSkillOptions.h"
#include "SkillCoolDownUi.h"
#include "../Scene/PlayScene.h"
#include "MessageUi.h"
#include "SkillBookUi.h"
#include "ItemBoxUi.h"
#include "WardrobeUi.h"
#include "../Ui/MessageUi.h"
#include "../GameObject/GlassTube.h"
#include "DataTableList.h"
#include "../GameObject/SkillSet.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/NpcTalkTable.h"

PlayUiMgr::PlayUiMgr()
	: UiMgr(SCENE_MGR->GetScene(Scenes::Play)), options(nullptr)
{
}

PlayUiMgr::~PlayUiMgr()
{
}

void PlayUiMgr::Init()
{
	UiMgr::Init();
	windowSize = FRAMEWORK->GetWindowSize();

	SpriteObj* statusBar = new SpriteObj();
	statusBar->SetTexture(*RESOURCE_MGR->GetTexture("graphics/StatusBarBG.png"));
	statusBar->SetScale({4, 4});
	statusBar->SetOrigin(Origins::MC);
	statusBar->SetPos({ windowSize.x * 0.12f, windowSize.y * 0.1f });
	uiObjList[0].push_back(statusBar);

	// Player Marker
	{
		for (int i = 0; i < 3; i++)
		{
			auto marker = new SpriteObj();

			switch (i)
			{
			case 0:
				marker->SetTexture(*RESOURCE_MGR->GetTexture("graphics/PlayerMarker #142163.png"));
				marker->SetScale({ 6, 6 });
				marker->SetColor({ 255, 255, 255, 20 });
				break;
			case 1:
				marker->SetTexture(*RESOURCE_MGR->GetTexture("graphics/PlayerMarkerOutline.png"));
				marker->SetScale({ 6, 6 });
				marker->SetColor({ 255, 255, 255, 30 });
				break;
			case 2:
				marker->SetTexture(*RESOURCE_MGR->GetTexture("graphics/PlayerMarkerGlow.png"));
				marker->SetScale({ 5.5, 5.5 });
				marker->SetColor({ 255, 255, 255, 20 });
				break;
			}

			marker->SetOrigin(Origins::MC);
			marker->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.5f });

			switch (i)
			{
			case 0:
				playerMarker = marker;
				break;
			case 1:
				playerMarkerOutLine = marker;
				break;
			case 2:
				playerMarkerGlow = marker;
				break;
			}
		}

		uiObjList[0].push_back(playerMarker);
		uiObjList[0].push_back(playerMarkerOutLine);
		uiObjList[0].push_back(playerMarkerGlow);
	}

	// HP
	{
		for (int i = 0; i < 2; i++)
		{
			SpriteObj* hp = new SpriteObj();

			if (i == 0)
				hp->SetTexture(*RESOURCE_MGR->GetTexture("graphics/HPBarFill.png"));
			else
				hp->SetTexture(*RESOURCE_MGR->GetTexture("graphics/HPBarHurtFill.png"));

			hp->SetPos({ windowSize.x * 0.074f, windowSize.y * 0.0745f });
			hp->SetScale({ 4, 4 });

			if (i == 0)
				HpBarFill = hp;
			else
				HpBarHurt = hp;
		}

		hpText = new TextObj();
		hpText->SetFont(*RESOURCE_MGR->GetFont("fonts/MunroSmall.ttf"));
		hpText->SetSize(40);
		hpText->SetFillColor(Color::White);
		hpText->SetOutlineColor(Color::Black);
		hpText->SetOutlineThickness(1.5f);
		hpText->SetText(to_string(525) + "/" + to_string(525)); // �÷��̾� maxhp�� ����
		hpText->SetPos({ 250, 50 });

		uiObjList[0].push_back(HpBarHurt);
		uiObjList[0].push_back(HpBarFill);
		uiObjList[0].push_back(hpText);
	}

	// Hp Bar, Overdrive Bar
	{
		OverdriveActiveBar = new SpriteObj();
		OverdriveActiveBar->SetTexture(*RESOURCE_MGR->GetTexture("graphics/OverdriveActiveBarFill.png"));
		OverdriveActiveBar->SetPos({ windowSize.x * 0.074f, windowSize.y * 0.11f });
		OverdriveActiveBar->SetScale({ 4, 4 });
		//OverdriveActiveBar->SetSize({ 0, OverdriveActiveBar->GetSize().y });
		uiObjList[0].push_back(OverdriveActiveBar);

		playerStatusBarPortrait = new SpriteObj();
		playerStatusBarPortrait->SetName("PLAYERSTATUSPORTRAIT");
		playerStatusBarPortrait->UseShader();
		playerStatusBarPortrait->SetTexture(*RESOURCE_MGR->GetTexture("graphics/PlayerStatusBarPortrait.png"));
		playerStatusBarPortrait->SetPos({ windowSize.x * 0.038f, windowSize.y * 0.07f });
		playerStatusBarPortrait->SetScale({ 4, 4 });
		playerStatusBarPortrait->SetSpriteShader();
		playerStatusBarPortrait->SetSpritePalette(64, "graphics/WizardPalette.png");
		uiObjList[0].push_back(playerStatusBarPortrait);
	}

	// Menu
	{
		menu = new SpriteObj();
		menu->SetTexture(*RESOURCE_MGR->GetTexture("graphics/ItemInfoPanel.png"));
		menu->SetColor(Color(50, 50, 50, 220));
		menu->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.5f });
		menu->SetOrigin(Origins::MC);
		menu->SetScale({ 3, 8 });
		uiObjList[1].push_back(menu);


		TextObj* menuText = new TextObj();
		menuText->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
		menuText->SetSize(40);
		menuText->SetFillColor(Color::White);
		menuText->SetOutlineColor(Color::Black);
		menuText->SetOutlineThickness(2.f);
		menuText->SetText("P A U S E");
		menuText->SetOrigin(Origins::MC);
		menuText->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.35f });
		uiObjList[1].push_back(menuText);

		menuRec = new RectangleShape();
		menuRec->setFillColor(Color::White);
		menuRec->setSize({ menu->GetSize().x * 2.5f, 2.f});
		menuRec->setOrigin(menuRec->getSize().x * 0.5f, menuRec->getSize().y * 0.5f);
		menuRec->setPosition({ windowSize.x * 0.5f, windowSize.y * 0.4f });
		
		options = new PlaySceneSkillOptions();
		options->Init();

		vector<string> sceneNames = { "SKILL", "TITLE" ,"EXIT" };
		for (int i = 0; i < 3; ++i)
		{
			Button2* button = new Button2();
			button->Init();
			button->UseText();

			button->SetText("fonts/NotoSansKR-Bold.otf", 35, Color(255, 255, 255, 153), sceneNames[i]);

			button->SetHitBox(button->GetText()->GetSFMLText().getGlobalBounds(), Color::Transparent);
			button->GetText()->SetOutlineColor(Color::Black);
			button->GetText()->SetOutlineThickness(2.f);

			button->SetPos({ windowSize.x * 0.5f, windowSize.y * (0.47f + 0.08f * i) });
			button->MousePointerOn = bind(&TextObj::SetFillColor, button->GetText(), Color::White);
			button->MousePointerOff = bind(&TextObj::SetFillColor, button->GetText(), Color(255, 255, 255, 153));

			if (i == 0)
				button->ClickOn = bind(&PlaySceneSkillOptions::SetActive, options, true);
			if (i == 1)
				button->ClickOn = bind(&SceneMgr::ChangeScene, SCENE_MGR, Scenes::Title);
			else if (i == 2)
				button->ClickOn = bind(&SceneMgr::Exit, SCENE_MGR);

			button->SetOrigin(Origins::MC);
			button->SetUI(true);

			uiObjList[1].push_back(button);
		}
	}

	// Boss Hp Bar
	{
		bossHpBarBG = new SpriteObj();
		bossHpBarBG->SetTexture(*RESOURCE_MGR->GetTexture("graphics/EnemyHealthBarBG.png"));
		bossHpBarBG->SetOrigin(Origins::MC);
		bossHpBarBG->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.12f });
		bossHpBarBG->SetScale({ 4, 3.8 });
		//uiObjList[2].push_back(bossHpBarBG);

		for (int i = 0; i < 2; i++)
		{
			SpriteObj* hp = new SpriteObj();

			if (i == 0)
			{
				hp->SetTexture(*RESOURCE_MGR->GetTexture("graphics/EnemyHealthBarFill.png"));
				hp->SetSize({ 0, 7.f * 4.f});
				hp->SetOrigin(Origins::ML);
				hp->SetPos({ windowSize.x * 0.5f - (hp->GetSize().x * 2) + 2.7f, windowSize.y * 0.12f + 2.f});
				bossHpBarFill = hp;
			}
			else
			{
				hp->SetTexture(*RESOURCE_MGR->GetTexture("graphics/HPBarHurtFill.png"));
				hp->SetSize({ 0, bossHpBarFill->GetSize().y * 4.f });
				hp->SetOrigin(Origins::ML);
				hp->SetPos(bossHpBarFill->GetPos());
				bossHpBarHurt = hp;
			}
		}

		bossName = new TextObj();
		bossName->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
		bossName->SetSize(35);
		bossName->SetFillColor(Color::White);
		bossName->SetOutlineColor(Color::Black);
		bossName->SetOutlineThickness(2.f);
		bossName->SetText("BOSS NAME");
		bossName->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.07f });
	}

	for (int i = 0; i < 6; i++)
	{
		skillSelects.push_back(new SpriteObj());
		skillKeys.push_back(new SpriteObj());
		skills.push_back(new SpriteObj());
		SetSkillIcon(i, "graphics/Disabled.png");
		coolDownUis.push_back(new SkillCoolDownUi());
	}

	for (int i = 0; i < skillSelects.size(); i++)
	{
		skillSelects[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/InactiveCooldownPanel.png"));
		skillSelects[i]->SetScale({4, 4});
		skillSelects[i]->SetPos({ windowSize.x * (0.05f + (0.04f * i)), windowSize.y * 0.94f });
		skillSelects[i]->SetOrigin(Origins::MC);
		uiObjList[0].push_back(skillSelects[i]);
	}

	for (int i = 0; i < skillKeys.size(); i++)
	{
		switch (i)
		{
		case 0:
			skillKeys[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/M0.png"));
			break;
		case 1:
			skillKeys[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Space.png"));
			break;
		case 2:
			skillKeys[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/M1.png"));
			break;
		case 3:
			skillKeys[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Q.png"));
			break;
		case 4:
			skillKeys[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/E.png"));
			break;
		case 5:
			skillKeys[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/R.png"));
			break;
		}
		
		skillKeys[i]->SetScale({ 4, 4 });
		skillKeys[i]->SetPos({ windowSize.x * (0.05f + (0.04f * i)), windowSize.y * 0.87f });
		skillKeys[i]->SetOrigin(Origins::MC);

		skills[i]->SetScale({4, 4});
		skills[i]->SetPos(skillSelects[i]->GetPos());

		uiObjList[0].push_back(skillKeys[i]);
		uiObjList[0].push_back(skills[i]);
	}

	for (int i = 0; i < coolDownUis.size(); ++i)
	{
		coolDownUis[i]->Init();
		coolDownUis[i]->SetPos(skillSelects[i]->GetPos());
		uiObjList[0].push_back(coolDownUis[i]);
	}

	// property
	gold = new SpriteObj();
	gold->SetTexture(*RESOURCE_MGR->GetTexture("graphics/GoldHD.png"));
	gold->SetPos({windowSize.x * 0.48f, windowSize.y * 0.95f});
	gold->SetOrigin(Origins::MC);
	gold->SetScale({3, 3});
	uiObjList[0].push_back(gold);
	goldText = new TextObj();
	goldText->SetFont(*RESOURCE_MGR->GetFont("fonts/MunroSmall.ttf"));
	goldText->SetSize(50);
	goldText->SetFillColor(Color::White);
	goldText->SetOutlineColor(Color::Black);
	goldText->SetOutlineThickness(2.f);
	goldText->SetText("");
	goldText->SetPos({ gold->GetPos().x + (gold->GetSize().x * 4), windowSize.y * 0.95f - (gold->GetSize().y * 2) + 3});
	uiObjList[0].push_back(goldText);

	platinum = new SpriteObj();
	platinum->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Platinum.png"));
	platinum->SetPos({ windowSize.x * 0.48f, windowSize.y * 0.9f });
	platinum->SetOrigin(Origins::MC);
	platinum->SetScale({ 3, 3 });
	uiObjList[0].push_back(platinum);
	platinumText = new TextObj();
	platinumText->SetFont(*RESOURCE_MGR->GetFont("fonts/MunroSmall.ttf"));
	platinumText->SetSize(50);
	platinumText->SetFillColor(Color::White);
	platinumText->SetOutlineColor(Color::Black);
	platinumText->SetOutlineThickness(2.f);
	platinumText->SetText("");
	platinumText->SetPos({ platinum->GetPos().x + (gold->GetSize().x * 4), windowSize.y * 0.9f - (platinum->GetSize().y * 2) + 7 });
	uiObjList[0].push_back(platinumText);


	for (int i = 0; i < 4; i++)
		moveKeyboard.push_back(new SpriteObj());

	Scene* currScene = SCENE_MGR->GetCurrentScene();
	if (((PlayScene*)currScene)->GetMapName() == "TUTORIALMAP")
	{
		for (int i = 0; i < moveKeyboard.size(); i++)
		{
			switch (i)
			{
			case 0:
				moveKeyboard[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/W.png"));
				moveKeyboard[i]->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.35f });
				break;
			case 1:
				moveKeyboard[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/A.png"));
				moveKeyboard[i]->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.35f + moveKeyboard[i]->GetSize().y * 3 });
				break;
			case 2:
				moveKeyboard[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/S.png"));
				moveKeyboard[i]->SetPos({ windowSize.x * 0.5f - (moveKeyboard[i]->GetSize().x * 3), windowSize.y * 0.35f + (moveKeyboard[i]->GetSize().y * 3) });
				break;
			case 3:
				moveKeyboard[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/D.png"));
				moveKeyboard[i]->SetPos({ windowSize.x * 0.5f + (moveKeyboard[i]->GetSize().x * 3), windowSize.y * 0.35f + (moveKeyboard[i]->GetSize().y * 3) });
				break;
			}
			moveKeyboard[i]->SetScale({ 4, 4 });
			moveKeyboard[i]->SetOrigin(Origins::MC);

			uiObjList[0].push_back(moveKeyboard[i]);
		}

		keyboardBright = 255.f;
		isTutorial = true;
	}


	fps = new TextObj();
	fps->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	fps->SetSize(35);
	fps->SetFillColor(Color::White);
	fps->SetOutlineColor(Color::Black);
	fps->SetOutlineThickness(2.f);
	fps->SetText("");
	fps->SetPos({ windowSize.x * 0.8f, windowSize.y * 0.07f });
	uiObjList[0].push_back(fps);

	auto skillBook = new SkillBookUi();
	skillBook->SetName("SKILLBOOKUI");
	skillBook->Init();
	uiObjList[0].push_back(skillBook);

	auto itemBox = new ItemBoxUi();
	itemBox->SetName("ITEMBOXUI");
	itemBox->Init();
	uiObjList[0].push_back(itemBox);

	auto wardrobeUi = new WardrobeUi();
	wardrobeUi->SetName("WARDROBEUI");
	wardrobeUi->Init();
	uiObjList[0].push_back(wardrobeUi);

	msgUi = new MessageUi();
	msgUi->Init();
	uiObjList[0].push_back(msgUi);

	string name = ((PlayScene*)currScene)->GetMapName();
	if (name == "TUTORIALMAP" || name == "TUTORIALFIGHT")
		tutorialMessageSet = true;
}

void PlayUiMgr::Release()
{
	UiMgr::Release();
	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
		{
			if (obj != nullptr)
				delete obj;
			obj = nullptr;
		}
	}
	uiObjList.clear();
	glassTubes.clear();
}

void PlayUiMgr::SetPos(const Vector2f& pos)
{
	UiMgr::SetPos(pos);
	Vector2f delta = pos - position;
	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
			obj->Translate(delta);
	}
	position = pos;
}

void PlayUiMgr::Update(float dt)
{
	UiMgr::Update(dt);
	if (options->GetActive())
	{
		options->Update(dt);
		return;
	}

	if (isTutorial && !moveKeyboard.empty())
		TutorialMoveKeyboardUiControl(dt);

	goldText->SetOrigin(Origins::ML);
	goldText->SetText(to_string(player->GetCurGold()));
	platinumText->SetOrigin(Origins::ML);
	platinumText->SetText(to_string(player->GetCurPlatinum()));

	if (InputMgr::GetKeyDown(Keyboard::Key::Escape))
	{
		if (isOption)
		{
			if(InputMgr::GetEscapable())
				isOption = false;
		}
		else if (InputMgr::GetStackedOrder().empty())
			isOption = true;
	}

	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
		{
			if (obj->GetActive())
				obj->Update(dt);
		}
	}

	if (isOption)
	{
		for (auto& uiObjs : uiObjList[1])
			uiObjs->SetActive(true);
	}
	else
	{
		for (auto& uiObjs : uiObjList[1])
			uiObjs->SetActive(false);

		//OverdriveBarControl(dt);
		HpBarSizeControl(dt);
		BossHpBraSizeControl(dt);

		playerMarker->GetSprite().setRotation(Utils::Angle({ windowSize.x * 0.5f, windowSize.y * 0.5f }, InputMgr::GetMousePos()) + 90);
		playerMarkerOutLine->GetSprite().setRotation(playerMarker->GetSprite().getRotation());
		playerMarkerGlow->GetSprite().setRotation(playerMarker->GetSprite().getRotation());
	}

	if (isDevMode)
	{
		float fpsi = 1.f / dt;
		if (fpsi < 30.f)
			fps->SetSize(100.f);
		fps->SetString(to_string(fpsi));
	}

	GlassControl();
	TutorialMessage();
}

void PlayUiMgr::Draw(RenderWindow& window)
{
	UiMgr::Draw(window);

	if (!isOption)
	{
		for (auto& uiObjs : uiObjList)
		{
			if (uiObjs.second != uiObjList[1])
			{
				for (auto& obj : uiObjs.second)
				{
					if(obj->GetActive())
						obj->Draw(window);
				}
			}
		}
	}
	else
	{
		for (auto& uiObjs : uiObjList)
		{
			for (auto& obj : uiObjs.second)
			{
				if (obj->GetActive())
					obj->Draw(window);
			}
		}
		window.draw(*menuRec);
	}
	if (options->GetActive())
		options->Draw(window);

	if (isDevMode)
		fps->Draw(window);
}

void PlayUiMgr::HpBarSizeControl(float dt)
{
	// Hp Bar Control
	float playerCurHpBarSet = (player->GetCurHp() * (maxHpBarSize / player->GetMaxHp())); // hp
	if (player->GetCurHp() <= 0.f)
	{
		HpBarFill->SetSize({ 0.f, 0.f });
		hpText->SetText("0 / " + to_string(player->GetMaxHp()));
	}
	else
	{
		HpBarFill->SetSize({ playerCurHpBarSet, HpBarFill->GetSize().y * 4 });
		hpText->SetText(to_string(player->GetCurHp()) + "/" + to_string(player->GetMaxHp()));
	}

	// HP Yellow Bar Control
	if (playerCurHpBarSet < hpBarHurtSize)
	{
		HpBarHurt->SetSize({ hpBarHurtSize -= (dt * 50), HpBarHurt->GetSize().y * 4 });
	}
}

void PlayUiMgr::BossHpBraSizeControl(float dt)
{
	bossName->SetOrigin(Origins::MC);

	if (!isStart)
	{
		bool set = false;
		bool alive = false;
		switch(bossType)
		{
        case PlayUiMgr::BossType::Archer:
			if (heavyBombingArcher != nullptr)
			{
				set = heavyBombingArcher->GetIsAlive();
				alive = heavyBombingArcher->GetActive();
			}
			break;
		case PlayUiMgr::BossType::FireBoss:
			if (fireBoss != nullptr)
			{
				set = fireBoss->GetIsAlive();
				alive = fireBoss->GetActive();
			}
			break;
		case PlayUiMgr::BossType::FinalBoss:
			alive = true;
			if (finalBoss != nullptr)
			{
				if (finalBoss->GetState() != FinalBoss::States::Die)
					set = true;
				else
					set = false;
			}
			break;
		}

		if (set)
		{
			if (alive)
			{
				if (uiObjList[2].empty())
				{
					uiObjList[2].push_back(bossHpBarBG);
					uiObjList[2].push_back(bossHpBarHurt);
					uiObjList[2].push_back(bossHpBarFill);
					uiObjList[2].push_back(bossName);
				}
			}

			if (bossCurHp < bossMaxHp)
			{
				bossCurHp += 10;

				if (bossCurHp >= bossMaxHp)
				{
					bossCurHp = bossMaxHp;
					isAlive = true;
					isStart = true;
				}
			}

			// Hp Bar Control
			float bossCurHpBarSet = (bossMaxHp - bossCurHp) * (bossMaxHpBarSize / bossMaxHp);
			bossHpBarFill->SetSize({ (float)bossHpBarSize - bossCurHpBarSet, bossHpBarFill->GetSize().y * 4.f });
			bossHpBarHurt->SetSize({ (float)bossHpBarHurtSize - bossCurHpBarSet, bossHpBarFill->GetSize().y * 4.f });
		}
	}
	else
	{
		switch (bossType)
		{
		case PlayUiMgr::BossType::Archer:
			bossCurHp = heavyBombingArcher->GetCurHp();
			break;
		case PlayUiMgr::BossType::FireBoss:
			bossCurHp = fireBoss->GetCurHp();
			break;
		case PlayUiMgr::BossType::FinalBoss:
			bossCurHp = finalBoss->GetCurHp();
			break;
		}

		// Hp Bar Control
		float bossCurHpBarSet = bossCurHp * (bossMaxHpBarSize / bossMaxHp);
		if (bossCurHp <= 0.f)
		{
			bossHpBarFill->SetSize({ 0.f, 0.f });
		}
		else
		{
			bossHpBarFill->SetSize({ bossCurHpBarSet, bossHpBarHurt->GetSize().y * 4.f });
		}

		// HP Yellow Bar Control
		if (bossCurHpBarSet < bossHpBarHurtSize)
		{
			if (bossHpBarHurt->GetSize().x >= 0.f)
				bossHpBarHurt->SetSize({ bossHpBarHurtSize -= (dt * 50), bossHpBarHurt->GetSize().y * 4.f });
			else
				bossHpBarHurt->SetSize({ 0.f, bossHpBarHurt->GetSize().y * 4.f });
		}

		if (bossCurHp <= 0 && isAlive)
		{
			dieTimer -= dt;
			if (dieTimer <= 0.f)
			{
				uiObjList[2].clear();
				bossHpBarHurtSize = 89.f * 4.f;
				isAlive = false;
				isStart = false;
			}
		}
	}

}

void PlayUiMgr::OverdriveBarControl(float dt)
{
	// Test
	if (InputMgr::GetKeyDown(Keyboard::Key::A))
		testOverdrive = true;
	if (InputMgr::GetKeyDown(Keyboard::Key::S))
		testOverdrive = false;

	if (testOverdrive && overdriveBarSize < maxOverdriveBarSize)
		OverdriveActiveBar->SetSize({ overdriveBarSize += (OverdriveActiveBar->GetSize().x / 100), OverdriveActiveBar->GetSize().y * 4 });
	else if (!testOverdrive && overdriveBarSize > 0)
		OverdriveActiveBar->SetSize({ overdriveBarSize -= (OverdriveActiveBar->GetSize().x / 100), OverdriveActiveBar->GetSize().y * 4 });
}

void PlayUiMgr::SetPlayer(Player* player)
{
	this->player = player;
	for (int i = 0; i < coolDownUis.size(); ++i)
	{
		coolDownUis[i]->SetSkillSet(player->GetSkillSets()[i]);
	}
}

void PlayUiMgr::SetBossName(string name)
{
	bossName->SetText(name);
}

void PlayUiMgr::SetSkillIcon(int idx, const string& texture)
{
	skills[idx]->SetTexture(*RESOURCE_MGR->GetTexture(texture));
	skills[idx]->SetOrigin(Origins::MC);
}

void PlayUiMgr::TutorialMoveKeyboardUiControl(float dt)
{
	keyboardEnabledTimer -= dt;

	if (keyboardEnabledTimer <= 1.f)
	{
		keyboardBright -= dt;
		for (int i = 0; i < moveKeyboard.size(); i++)
		{
			Color fadeColor = moveKeyboard[i]->GetSprite().getColor();
			fadeColor.a = keyboardBright;
			moveKeyboard[i]->SetColor(fadeColor);
		}
	}

	if (keyboardBright <= 0)
		isTutorial = false;
}

void PlayUiMgr::TutorialMessage()
{
	if (tutorialMessageSet)
	{
		if (setting)
		{
			msgUi->SetNpcName("???");
			auto currScene = SCENE_MGR->GetCurrentScene();

			auto messageTable = DATATABLE_MGR->Get<NpcTalkTable>(DataTable::Types::NpcTalk);
			if (((PlayScene*)currScene)->GetMapName() == "TUTORIALMAP")
			{
				auto& p = messageTable->Get("Tutorial1");
				string message = p[0];
				msgUi->SetTexts(message);
			}
			else if (((PlayScene*)currScene)->GetMapName() == "TUTORIALFIGHT")
			{
				auto& p = messageTable->Get("Tutorial2");
				string message = p[0];
				msgUi->SetTexts(message);
			}

			msgUi->SetNpcImage("graphics/BankerPortrait.png");
			msgUi->SetPlayerImage(63);
			msgUi->MessageUiOn();

			setting = false;
		}

		if (msgUi->GetisTalk())
		{
			msgUi->Talk();

			if (InputMgr::GetKeyDown(Keyboard::Space))
			{
				msgUi->UiEnabled(false);
				msgUi->SetIsTalk(false);
				tutorialMessageSet = false;
			}
		}
	}
}

Object* PlayUiMgr::FindUiObj(const string& name)
{
	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
		{
			if (obj->GetName() == name)
				return obj;
		}
	}
	return nullptr;
}

void PlayUiMgr::NewGlassTubes(GlassTube* tube)
{
	glassTubes.push_back(tube);
}

void PlayUiMgr::GlassTubeSet(int index, bool set)
{
	glassTubes[index]->SetIsPlayerAdjacent(set);
}

void PlayUiMgr::SetPlayerSkillSet(const string& skillSetName, int index)
{
	auto currScene = SCENE_MGR->GetCurrentScene();
	Player* player = (Player*)currScene->FindGameObj("PLAYER");
	player->SetSkillSet(index, skillSetName, true);
}

void PlayUiMgr::GlassControl()
{
	if (glassTubes.size() != 0)
	{
		for (int i = 0; i < glassTubes.size(); i++)
		{
			if (glassTubes[i]->GetIsPlayerAdjacent())
			{
				if (InputMgr::GetKeyDown(Keyboard::F))
				{
					msgUi->SetNpcName(glassTubes[i]->GetName());
					msgUi->SetTexts(glassTubes[i]->GetMessages()[0]);
					msgUi->SetNpcImage("graphics/BankerPortrait.png");
					msgUi->SetPlayerImage(63);
					msgUi->MessageUiOn();
				}

				if (msgUi->GetisTalk())
				{
					if (InputMgr::GetKeyDown(Keyboard::Space))
						messageIndex++;

					if (messageIndex < glassTubes[i]->GetMessages().size())
					{
						msgUi->SetTexts(glassTubes[i]->GetMessages()[messageIndex]);
						msgUi->Talk();
					}
				}

				if (messageIndex == glassTubes[i]->GetMessages().size())
				{
					if (InputMgr::GetKeyDown(Keyboard::Space))
					{
						msgUi->UiEnabled(false);
						msgUi->SetIsTalk(false);
						messageIndex = 0;
						switch (i)
						{
						case 0:
							if (player->GetSkillSets()[0]->GetSkillSetName() != "FireBall")
							{
								SetPlayerSkillSet("FireBall", 0);
								tutorialEnd++;
							}
							break;
						case 1:
							if (player->GetSkillSets()[2]->GetSkillSetName() != "DragonArc")
							{
								SetPlayerSkillSet("DragonArc", 2);
								tutorialEnd++;
							}
							break;
						case 2:
							if (player->GetSkillSets()[3]->GetSkillSetName() != "Temp9")
							{
								SetPlayerSkillSet("Temp9", 3);
								tutorialEnd++;
							}
							break;
						case 3:
							if (player->GetSkillSets()[4]->GetSkillSetName() != "JumpMeteor")
							{
								SetPlayerSkillSet("JumpMeteor", 4);
								tutorialEnd++;
							}
							break;
						}
					}
				}
			}
		}
	}
}
