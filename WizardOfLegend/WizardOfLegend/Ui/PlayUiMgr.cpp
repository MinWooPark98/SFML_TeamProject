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

PlayUiMgr::PlayUiMgr()
	: UiMgr(SCENE_MGR->GetScene(Scenes::Play)), windowSize()
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
		hpText->SetText(to_string(525) + "/" + to_string(525)); // 플레이어 maxhp로 변경
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
		OverdriveActiveBar->SetSize({ 0, OverdriveActiveBar->GetSize().y });
		uiObjList[0].push_back(OverdriveActiveBar);

		playerStatusBarPortrait = new SpriteObj();
		playerStatusBarPortrait->SetTexture(*RESOURCE_MGR->GetTexture("graphics/PlayerStatusBarPortrait.png"));
		playerStatusBarPortrait->SetPos({ windowSize.x * 0.038f, windowSize.y * 0.07f });
		playerStatusBarPortrait->SetScale({ 4, 4 });
		playerStatusBarPortrait->SetSpriteShader();
		playerStatusBarPortrait->SetSpritePalette(64, 64, "graphics/WizardPalette.png");
		playerStatusBarPortrait->SetSpriteColor(1); // 플레이어랑 색깔 연동해야함
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

			// 옵션 화면이랑 연결
			//if (i == 0)
				//button->ClickOn = bind(&SceneMgr::ChangeScene, SCENE_MGR, (Scenes)(i + 1));
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
		uiObjList[2].push_back(bossHpBarBG);

		for (int i = 0; i < 2; i++)
		{
			SpriteObj* hp = new SpriteObj();

			if (i == 0)
			{
				hp->SetTexture(*RESOURCE_MGR->GetTexture("graphics/EnemyHealthBarFill.png"));
				//hp->SetSize({ bossMaxHpBarSize, 7.f * 4.f});
				hp->SetSize({ 0, 7.f * 4.f});
				hp->SetOrigin(Origins::ML);
				hp->SetPos({ windowSize.x * 0.5f - (hp->GetSize().x * 2) + 2.7f, windowSize.y * 0.12f + 2.f});
				bossHpBarFill = hp;
			}
			else
			{
				hp->SetTexture(*RESOURCE_MGR->GetTexture("graphics/HPBarHurtFill.png"));
				//hp->SetSize({ bossHpBarFill->GetSize().x * 4.f + 5.f, bossHpBarFill->GetSize().y * 4.f });
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

		uiObjList[2].push_back(bossHpBarHurt);
		uiObjList[2].push_back(bossHpBarFill);
		uiObjList[2].push_back(bossName);

		fps = new TextObj();
		fps->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
		fps->SetSize(35);
		fps->SetFillColor(Color::White);
		fps->SetOutlineColor(Color::Black);
		fps->SetOutlineThickness(2.f);
		fps->SetText("");
		fps->SetPos({ windowSize.x * 0.8f, windowSize.y * 0.07f });
		uiObjList[0].push_back(fps);
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
	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
		{
			if (obj->GetActive())
				obj->Update(dt);
		}
	}

	if (InputMgr::GetKeyDown(Keyboard::Key::Escape))
	{
		if (isOption)
			isOption = false;
		else
			isOption = true;
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

		OverdriveBarControl(dt);
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
				obj->Draw(window);
		}
		window.draw(*menuRec);
	}
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
		if (bossCurHp < bossMaxHp)
		{
			bossCurHp += 5;

			if (bossCurHp >= bossMaxHp)
			{
				bossCurHp = bossMaxHp;
				isStart = true;
			}
		}

		// Hp Bar Control
		float bossCurHpBarSet = (bossMaxHp - bossCurHp) * (bossMaxHpBarSize / bossMaxHp);
		bossHpBarFill->SetSize({ (float)bossHpBarSize - bossCurHpBarSet, bossHpBarFill->GetSize().y * 4.f });
		bossHpBarHurt->SetSize({ (float)bossHpBarHurtSize - bossCurHpBarSet, bossHpBarFill->GetSize().y * 4.f });
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
			bossHpBarHurt->SetSize({ bossHpBarHurtSize -= (dt * 50), bossHpBarHurt->GetSize().y * 4.f });
		}

		if (bossCurHp <= 0 && isAlive)
		{
			dieTimer -= dt;
			if (dieTimer <= 0.f)
			{
				uiObjList[2].clear();
				isAlive = false;
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

void PlayUiMgr::SetBossName(string name)
{
	bossName->SetText(name);
}