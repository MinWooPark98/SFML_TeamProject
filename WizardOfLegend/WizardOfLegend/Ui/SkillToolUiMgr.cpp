#include "SkillToolUiMgr.h"
#include "../Scene/SceneMgr.h"
#include "../GameObject/Button.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/Framework.h"
#include "../GameObject/TextObj.h"
#include "SelectOption.h"

SkillToolUiMgr::SkillToolUiMgr()
	:UiMgr(SCENE_MGR->GetScene(Scenes::SkillTool))
{
}

SkillToolUiMgr::~SkillToolUiMgr()
{
}

void SkillToolUiMgr::Init()
{
	UiMgr::Init();
	auto windowSize = (Vector2f)FRAMEWORK->GetWindowSize();

	SelectOption* options = new SelectOption();
	options->Init();
	uiObjList[0].push_back(options);

	/*Button* button = new Button(this);
	button->Init();
	button->UseText();
	button->SetText(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"), 20, Color::White, "SkillName", true);
	button->SetHitBox({ 0.f, 0.f, 200.f, 30.f }, Color(150, 150, 150, 255));
	button->GetHitBox().setOutlineThickness(2.f);
	button->SetDevMode(true);
	button->SetClkColor(true);
	button->SetPos({ windowSize.x * 0.7f, 0.f });
	button->SetOrigin(Origins::TL);
	uiObjList[0].push_back(button);*/
}

void SkillToolUiMgr::Release()
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

void SkillToolUiMgr::SetPos(const Vector2f& pos)
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

void SkillToolUiMgr::Update(float dt)
{
	UiMgr::Update(dt);
	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
			obj->Update(dt);
	}
}

void SkillToolUiMgr::Draw(RenderWindow& window)
{
	UiMgr::Draw(window);
	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
			obj->Draw(window);
	}
}
