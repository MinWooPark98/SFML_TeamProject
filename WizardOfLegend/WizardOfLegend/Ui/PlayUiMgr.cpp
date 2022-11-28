#include "PlayUiMgr.h"
#include "TitleUiMgr.h"
#include "../Scene/SceneMgr.h"
#include "../GameObject/Button2.h"
#include "../GameObject/TextObj.h"
#include "../Framework/Framework.h"
#include "../GameObject/SpriteObj.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"

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
		playerMarker = new SpriteObj();
		playerMarker->SetTexture(*RESOURCE_MGR->GetTexture("graphics/PlayerMarker #142163.png"));
		playerMarker->SetOrigin(Origins::MC);
		playerMarker->SetScale({ 6, 6 });
		playerMarker->SetColor({ 255, 255, 255, 30 });
		playerMarker->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.5f });
		uiObjList[0].push_back(playerMarker);

		playerMarkerOutLine = new SpriteObj();
		playerMarkerOutLine->SetTexture(*RESOURCE_MGR->GetTexture("graphics/PlayerMarkerOutline.png"));
		playerMarkerOutLine->SetOrigin(Origins::MC);
		playerMarkerOutLine->SetScale({ 6, 6 });
		playerMarkerOutLine->SetColor({ 255, 255, 255, 50 });
		playerMarkerOutLine->SetPos(playerMarker->GetPos());
		uiObjList[0].push_back(playerMarkerOutLine);

		playerMarkerGlow = new SpriteObj();
		playerMarkerGlow->SetTexture(*RESOURCE_MGR->GetTexture("graphics/PlayerMarkerGlow.png"));
		playerMarkerGlow->SetOrigin(Origins::MC);
		playerMarkerGlow->SetScale({ 5.5, 5.5 });
		playerMarkerGlow->SetColor({ 255, 255, 255, 30 });
		playerMarkerGlow->SetPos(playerMarker->GetPos());
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
		
		vector<string> sceneNames = { "OPTION", "TITLE" ,"EXIT" };
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

	if (InputMgr::GetKeyDown(Keyboard::Key::A))
		testOverdrive = true;
	if (InputMgr::GetKeyDown(Keyboard::Key::S))
		testOverdrive = false;


	if (InputMgr::GetKeyDown(Keyboard::Key::D))
		testHp = true;
	if (InputMgr::GetKeyDown(Keyboard::Key::F))
		testHp = false;


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

		if (testOverdrive && overdriveBarSize < maxOverdriveBarSize)
			OverdriveActiveBar->SetSize({ overdriveBarSize += (OverdriveActiveBar->GetSize().x / 100), OverdriveActiveBar->GetSize().y * 4 });
		else if (!testOverdrive && overdriveBarSize > 0)
			OverdriveActiveBar->SetSize({ overdriveBarSize -= (OverdriveActiveBar->GetSize().x / 100), OverdriveActiveBar->GetSize().y * 4 });

		HpBarSizeControl(dt);

		playerMarker->GetSprite().setRotation(Utils::Angle({ windowSize.x * 0.5f, windowSize.y * 0.5f }, InputMgr::GetMousePos()) + 90);
		playerMarkerOutLine->GetSprite().setRotation(playerMarker->GetSprite().getRotation());
		playerMarkerGlow->GetSprite().setRotation(playerMarker->GetSprite().getRotation());
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
	// 데미지 받음
	if (InputMgr::GetKeyDown(Keyboard::Key::G))
	{
		int damage = 50; // 몬스터 데미지로 변경

		if (playerCurHp - damage <= 0.f)
		{
			playerCurHp = 0.f;
			HpBarFill->SetSize({ 0, HpBarFill->GetSize().y * 4 });
		}
		else
		{
			playerCurHp -= damage;
		}
	}

	// 회복
	if (InputMgr::GetKeyDown(Keyboard::Key::Z))
	{
		int heal = 50; // 회복율로 변경

		if (playerCurHp + heal <= playerMaxHp)
		{
			playerCurHp += heal;
		}
		else
		{
			int overHeal = (playerCurHp + heal) - playerMaxHp;
			playerCurHp += (heal - overHeal);
		}
	}

	// Hp Bar Control
	int playerCurHpBarSet = (playerMaxHp - playerCurHp) * (maxHpBarSize / playerMaxHp); // hp바 사이즈 비율
	HpBarFill->SetSize({ (float)hpBarSize - playerCurHpBarSet, HpBarFill->GetSize().y * 4 });
	hpText->SetText(to_string(playerCurHp) + "/" + to_string(playerMaxHp));


	// HP Yellow Bar Control
	if (hpBarSize - playerCurHpBarSet < hpBarHurtSize)
	{
		HpBarHurt->SetSize({ hpBarHurtSize -= (dt * 50), HpBarHurt->GetSize().y * 4 });
	}
	else if (hpBarSize - playerCurHpBarSet > hpBarHurtSize)
	{
		hpBarHurtSize = hpBarSize - playerCurHpBarSet;
		HpBarHurt->SetSize({ hpBarHurtSize, HpBarHurt->GetSize().y * 4 });
	}
}