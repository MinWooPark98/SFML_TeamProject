#include "TitleUiMgr.h"
#include "../Scene/SceneMgr.h"
#include "../GameObject/Button2.h"
#include "../GameObject/TextObj.h"
#include "../Framework/Framework.h"

TitleUiMgr::TitleUiMgr()
	:UiMgr(SCENE_MGR->GetScene(Scenes::Title))
{
}

TitleUiMgr::~TitleUiMgr()
{
}

void TitleUiMgr::Init()
{
	UiMgr::Init();
	auto& windowSize = FRAMEWORK->GetWindowSize();
	vector<string> sceneNames = { "MapTool", "SkillTool", "Play" };
	for (int i = 0; i < 3; ++i)
	{
		Button2* button = new Button2();
		button->Init();
		button->UseText();
		button->SetText("fonts/NotoSansKR-Bold.otf", 50, Color(255, 255, 255, 153), sceneNames[i]);
		button->SetHitBox(button->GetText()->GetSFMLText().getGlobalBounds(), Color::Transparent);
		button->GetText()->SetOutlineColor(Color::Black);
		button->GetText()->SetOutlineThickness(2.f);
		button->SetPos({ windowSize.x * 0.5f, windowSize.y * (0.3f + 0.2f * i) });
		button->MousePointerOn = bind(&TextObj::SetFillColor, button->GetText(), Color::White);
		button->MousePointerOff = bind(&TextObj::SetFillColor, button->GetText(), Color(255, 255, 255, 153));
		button->ClickOn = bind(&SceneMgr::ChangeScene, SCENE_MGR, (Scenes)(i + 1));
		button->SetOrigin(Origins::MC);
		button->SetUI(true);
		uiObjList[0].push_back(button);
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
			obj->Update(dt);
	}
}

void TitleUiMgr::Draw(RenderWindow& window)
{
	UiMgr::Draw(window);
	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
			obj->Draw(window);
	}
}
