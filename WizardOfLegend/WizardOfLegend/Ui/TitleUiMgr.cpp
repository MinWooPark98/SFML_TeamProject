#include "TitleUiMgr.h"
#include "../Scene/SceneMgr.h"
#include "../GameObject/Button2.h"
#include "../GameObject/TextObj.h"
#include "../Framework/Framework.h"
#include "../GameObject/SpriteObj.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"

TitleUiMgr::TitleUiMgr()
	:UiMgr(SCENE_MGR->GetScene(Scenes::Title)), windowSize()
{
}

TitleUiMgr::~TitleUiMgr()
{
}

void TitleUiMgr::Init()
{
	UiMgr::Init();
	windowSize = FRAMEWORK->GetWindowSize();


	titleLogo = new SpriteObj();
	titleLogo->SetTexture(*RESOURCE_MGR->GetTexture("graphics/TitleLogo.png"));
	titleLogo->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.5f });
	titleLogo->SetOrigin(Origins::MC);
	uiObjList[0].push_back(titleLogo);

	backgrondShadow = new RectangleShape();
	backgrondShadow->setSize({ 2000, 1500 });
	backgrondShadow->setFillColor({ 0, 0, 0, 0 });

	vector<string> sceneNames = { "PLAY", "MAP TOOL", "SKILL TOOL", "EXIT", "Press Any Key To Start" };
	for (int i = 0; i < 5; ++i)
	{
		Button2* button = new Button2();
		button->Init();
		button->UseText();

		if (i < 4)
			button->SetText("fonts/NotoSansKR-Bold.otf", 40, Color(255, 255, 255, 153), sceneNames[i]);
		else
			button->SetText("fonts/NotoSansKR-Bold.otf", 60, Color(255, 255, 255, 180), sceneNames[i]);

		button->SetHitBox(button->GetText()->GetSFMLText().getGlobalBounds(), Color::Transparent);
		button->GetText()->SetOutlineColor(Color::Black);
		button->GetText()->SetOutlineThickness(2.f);

		if (i < 4)
		{
			button->SetPos({ windowSize.x * 0.5f, windowSize.y * (0.5f + 0.1f * i) });
			button->MousePointerOn = bind(&TextObj::SetFillColor, button->GetText(), Color::White);
			button->MousePointerOff = bind(&TextObj::SetFillColor, button->GetText(), Color(255, 255, 255, 153));
		}
		else
			button->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.6f});
		
		if (i < 3)
			button->ClickOn = bind(&SceneMgr::ChangeScene, SCENE_MGR, (Scenes)(i + 1));
		else if (i == 3)
			button->ClickOn = bind(&SceneMgr::Exit, SCENE_MGR);

		button->SetOrigin(Origins::MC);
		button->SetUI(true);

		if (i != 4)
			uiObjList[1].push_back(button);
		else
			uiObjList[2].push_back(button);
	}
}

void TitleUiMgr::Release()
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

void TitleUiMgr::SetPos(const Vector2f& pos)
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

void TitleUiMgr::Update(float dt)
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

	if (logoMove)
	{
		if (InputMgr::GetKeyDown(Keyboard::Key::Escape))
			logoMove = false;
	}
	else if (!logoMove && (!InputMgr::GetKeyDownList().empty() || !InputMgr::GetMouseDownList().empty()))
	{
		logoMove = true;
	}

	if (logoMove)
	{
		if (titleLogo->GetPos().y > windowSize.y * 0.3f)
		{
			titleLogo->Translate({ 0, titleLogo->GetPos().y * (dt * 2) * -1 });
			startTextActiveTimer = 0.f;

			if (titleLogo->GetPos().y <= windowSize.y * 0.3f)
				titleLogo->SetPos({titleLogo->GetPos().x, windowSize.y * 0.3f});
		}

		if (backgroundShadowValue < 170)
			backgrondShadow->setFillColor({ 0, 0, 0, (Uint8)(backgroundShadowValue += 0.3f) });
	}

	else if (!logoMove)
	{
		if (titleLogo->GetPos().y < windowSize.y * 0.5f)
			titleLogo->Translate({ 0, titleLogo->GetPos().y * (dt * 2) });

		if (backgroundShadowValue > 0)
			backgrondShadow->setFillColor({ 0, 0, 0, (Uint8)(backgroundShadowValue -= 0.3f) });
	}

	if (titleLogo->GetPos().y <= windowSize.y * 0.3f)
	{
		for (auto& uiObjs : uiObjList[1])
			uiObjs->SetActive(true);
	}
	else
	{
		for (auto& uiObjs : uiObjList[1])
			uiObjs->SetActive(false);
	}

	if (!logoMove)
		startTextActiveTimer += dt;
}

void TitleUiMgr::Draw(RenderWindow& window)
{
	window.draw(*backgrondShadow);

	UiMgr::Draw(window);

	for (auto& uiObjs : uiObjList[0])
		uiObjs->Draw(window);

	if (titleLogo->GetPos().y <= windowSize.y * 0.3f)
	{
		for (auto& uiObjs : uiObjList[1])
			uiObjs->Draw(window);
	}
	if (titleLogo->GetPos().y >= windowSize.y * 0.5f && (int)(startTextActiveTimer / 0.5f) % 2 == 0)
	{
		for (auto& uiObjs : uiObjList[2])
			uiObjs->Draw(window);
	}
}
