#include "SkillToolUiMgr.h"
#include "../Scene/SceneMgr.h"
#include "../GameObject/Button2.h"
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

void SkillToolUiMgr::Reset()
{
	UiMgr::Reset();
	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
		{
			obj->Reset();
		}
	}
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
