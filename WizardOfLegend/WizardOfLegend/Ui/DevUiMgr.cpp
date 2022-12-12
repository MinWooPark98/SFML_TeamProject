#include "DevUiMgr.h"
#include "../Framework/Framework.h"
#include "../Scene/SceneMgr.h"
#include "../Framework/InputMgr.h"
#include "SkillBookUi.h"
#include "ItemBoxUi.h"
#include "WardrobeUi.h"

DevUiMgr::DevUiMgr()
	:UiMgr(SCENE_MGR->GetScene(Scenes::Dev))
{
}

DevUiMgr::~DevUiMgr()
{
}

void DevUiMgr::Init()
{
	UiMgr::Init();

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
		{
			if(obj->GetActive())
				obj->Update(dt);
		}
	}
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

Object* DevUiMgr::FindUiObj(const string& name)
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
