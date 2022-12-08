#include "DevUiMgr.h"
#include "../Framework/Framework.h"
#include "../Scene/SceneMgr.h"
#include "SkillBookUi.h"
#include "../Framework/InputMgr.h"

DevUiMgr::DevUiMgr()
	:UiMgr(SCENE_MGR->GetScene(Scenes::Dev)), skillBook(nullptr)
{
}

DevUiMgr::~DevUiMgr()
{
}

void DevUiMgr::Init()
{
	UiMgr::Init();
	skillBook = new SkillBookUi();
	skillBook->Init();
	skillBook->SetActive(false);
	uiObjList[0].push_back(skillBook);
}

void DevUiMgr::Release()
{
	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
		{
			obj->Release();
			delete obj;
		}
	}
}

void DevUiMgr::Update(float dt)
{
	UiMgr::Update(dt);
	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
			obj->Update(dt);
	}
	if (InputMgr::GetKeyDown(Keyboard::F1))
		skillBook->SetActive(true);
}

void DevUiMgr::Draw(RenderWindow& window)
{
	UiMgr::Draw(window);
	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
		{
			if(obj->GetActive())
				obj->Draw(window);
		}
	}
}
