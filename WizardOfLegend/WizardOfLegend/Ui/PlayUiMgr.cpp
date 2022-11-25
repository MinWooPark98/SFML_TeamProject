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

	uiObjList[0].push_back(HpBarHurt);
	uiObjList[0].push_back(HpBarFill);

	OverdriveActiveBar = new SpriteObj();
	OverdriveActiveBar->SetTexture(*RESOURCE_MGR->GetTexture("graphics/OverdriveActiveBarFill.png"));
	OverdriveActiveBar->SetPos({ windowSize.x * 0.074f, windowSize.y * 0.11f });
	OverdriveActiveBar->SetScale({4, 4});
	OverdriveActiveBar->SetSize({ 0, OverdriveActiveBar->GetSize().y });
	uiObjList[0].push_back(OverdriveActiveBar);

	playerStatusBarPortrait = new SpriteObj();
	playerStatusBarPortrait->SetTexture(*RESOURCE_MGR->GetTexture("graphics/PlayerStatusBarPortrait.png"));
	playerStatusBarPortrait->SetPos({ windowSize.x * 0.038f, windowSize.y * 0.07f });
	playerStatusBarPortrait->SetScale({4, 4});
	playerStatusBarPortrait->SetSpriteShader();
	playerStatusBarPortrait->SetSpritePalette(64, 64, "graphics/WizardPalette.png");
	playerStatusBarPortrait->SetSpriteColor(1); // 플레이어랑 색깔 연동해야함
	uiObjList[0].push_back(playerStatusBarPortrait);

	hpText = new TextObj();
	hpText->SetFont(*RESOURCE_MGR->GetFont("fonts/MunroSmall.ttf"));
	hpText->SetSize(40);
	hpText->SetFillColor(Color::White);
	hpText->SetText(to_string(525) + "/" + to_string(525)); // 플레이어 maxhp로 변경
	hpText->SetPos({250, 50});
	uiObjList[0].push_back(hpText);
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


	if (testOverdrive && overdriveBarSize < maxOverdriveBarSize)
		OverdriveActiveBar->SetSize({ overdriveBarSize += (OverdriveActiveBar->GetSize().x / 100), OverdriveActiveBar->GetSize().y * 4 });
	else if (!testOverdrive && overdriveBarSize > 0)
		OverdriveActiveBar->SetSize({ overdriveBarSize -= (OverdriveActiveBar->GetSize().x / 100), OverdriveActiveBar->GetSize().y * 4 });

	HpBarSizeControl(dt);
}

void PlayUiMgr::Draw(RenderWindow& window)
{
	UiMgr::Draw(window);
	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
			obj->Draw(window);
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