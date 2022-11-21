#include "MapToolUiMgr.h"
#include "../GameObject/Button.h"
#include "DrawSelect.h"
#include "../GameObject/DrawObj.h"
#include "../Framework/ResourceMgr.h"
#include "../GameObject/SpriteObj.h"
#include "../Framework/InputMgr.h"
#include "../Scene/SceneMgr.h"
#include "../Framework/FileMgr.h"
#include "../Scene/MapToolScene.h"
#include "SaveWindowBox.h"
#include "LoadWindowBox.h"
#include "../Framework/Framework.h"

MapToolUiMgr::MapToolUiMgr(Scene* scene)
	:UiMgr(scene)
{
}

MapToolUiMgr::~MapToolUiMgr()
{
}

void MapToolUiMgr::Init()
{
	Vector2f WindowSize = (Vector2f)FRAMEWORK->GetWindowSize();
	paletteBook = new Button(this);
	paletteBook->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Map/MapToolBook.png"), true);
	paletteBook->SetPos({ WindowSize.x*0.5f,WindowSize.y *0.5f});
	paletteBook->SetOrigin(Origins::TC);
	uiObjList[0].push_back(paletteBook);

	editorObjs = FILE_MGR->GetEditorObjs();

	saveBtn = new Button(this);
	saveBtn->SetClkColor(true);
	saveBtn->SetText(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"),
		75, Color::White, "SAVE", true);
	saveBtn->SetOrigin(Origins::TL);
	saveBtn->SetPos({ 50,50 });
	uiObjList[0].push_back(saveBtn);


	loadBtn = new Button(this);
	loadBtn->SetClkColor(true);
	loadBtn->SetText(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"),
		75, Color::White, "LOAD", true);
	loadBtn->SetOrigin(Origins::TL);
	loadBtn->SetPos({ 50,120 });
	uiObjList[0].push_back(loadBtn);

	//exitBtn = new Button(this);
	//exitBtn->SetClkColor(true);
	//exitBtn->SetText(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"),
	//	75, Color::White, "EXIT", true);
	//exitBtn->SetOrigin(Origins::TL);
	//exitBtn->SetPos({ 50,190 });
	//uiObjList[0].push_back(exitBtn);

	selects = { "Tile","Wall","Object","PLAYER","ENEMY"};
	selectTextSize = { 40,40,40,40,40 };
	selectPosY = { 54,54,54,54,54 };

	selIdx = 0;
	selectBtn = new Button(this);
	selectBtn->SetClkColor(true);
	selectBtn->SetText(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"),
		selectTextSize[selIdx], Color::White, selects[selIdx], true); //TILE TREE STONE PLAYER ENEMY BOX ANOTHER
	selectBtn->SetOrigin(Origins::MC);
	selectBtn->SetPos(paletteBook->GetPos() + Vector2f{ -paletteBook->GetHitBounds().width*0.5f, selectPosY[selIdx]});
	uiObjList[0].push_back(selectBtn);

	for (auto& type : editorObjs)
	{
		float offsetX = 0;
		float offsetY = 0;
		for (auto& obj : type.second)
		{
			DrawSelect* draw = new DrawSelect(this);
			drawObj.push_back(draw);
			draw->Set(type.first, obj.texPath, obj.uiPath);
			draw->SetPos({ paletteBook->GetPos().x-340+offsetX, paletteBook->GetPos().y+120+offsetY});
			offsetX += 50;
			if (offsetX == 300)
			{
				offsetX = 0;
				offsetY += 50;

				if (offsetY == 300)
				{
					offsetY = 0;
				}
			}
			draw->SetData(obj);
			uiObjList[1].push_back(draw);
			type_selects[type.first].push_back(draw);
			draw->SetActive(true);
		}
	}

	saveWindow = new SaveWindowBox(this);
	saveWindow->SetPos({ 350,50 });
	saveWindow->Init();
	uiObjList[1].push_back(saveWindow);
	for (auto& obj : type_selects[selects[selIdx]])
	{
		obj->SetActive(true);
	}

	loadWindow = new LoadWindowBox(this);
	loadWindow->SetPos({ 350,50 });
	loadWindow->Init();
	uiObjList[1].push_back(loadWindow);
}

void MapToolUiMgr::Release()
{
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
	Object::Release();
}

void MapToolUiMgr::Reset()
{
}

void MapToolUiMgr::Update(float dt)
{
	if (saveWindow->GetActive())
	{
		saveWindow->Update(dt);
		if (saveWindow->IsCancle())
			saveWindow->SetActive(false);
		return;
	}
	if (loadWindow->GetActive())
	{
		loadBtn->Update(dt);
		if (loadBtn->IsUp())
		{
			loadWindow->SetActive(!loadWindow->GetActive());
			((MapToolUiMgr*)(parentScene->GetUiMgr()))->DeleteDraw();
		}
		loadWindow->Update(dt);
		return;
	}	

	if (nowDraw != nullptr)
	{
		nowDraw->Update(dt);
	}

	cout << (int)saveBtn->GetState() << endl;
	if (saveBtn->IsUp())
	{
		saveWindow->SetActive(true);
		((MapToolUiMgr*)(parentScene->GetUiMgr()))->DeleteDraw();
	}
	if (loadBtn->IsUp())
	{
		loadWindow->SetActive(!loadWindow->GetActive());
		((MapToolUiMgr*)(parentScene->GetUiMgr()))->DeleteDraw();
	}
	if (selectBtn->IsClick())
	{
		for (auto& obj : type_selects[selects[selIdx]])
		{
			obj->SetActive(false);
		}
		selIdx = (selects.size() + selIdx + 1) % selects.size();
		selectBtn->SetText(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"),
			selectTextSize[selIdx], Color::White, selects[selIdx], true); //TILE TREE STONE PLAYER ENEMY BOX ANOTHER
		selectBtn->SetOrigin(Origins::MC);
		selectBtn->SetPos(paletteBook->GetPos() + Vector2f{ -paletteBook->GetHitBounds().width*0.5f, selectPosY[selIdx]});

		for (auto& obj : type_selects[selects[selIdx]])
		{
			obj->SetActive(true);
		}

		((MapToolUiMgr*)(parentScene->GetUiMgr()))->DeleteDraw();
	}

	if (!enabled)
		return;
	bool isEvent = false;

	if (nowEvObj != nullptr)
	{
	    if (nowEvObj->IsMove() && nowEvObj->IsDrag())
	    {
	        nowEvObj->Update(dt);
	        isEvent = true;
	    }
	    else if (nowEvObj->IsMoveRight() && nowEvObj->IsDragRight())
	    {
	        nowEvObj->Update(dt);
	        isEvent = true;
	    }
	}
	for (auto uiObjs = uiObjList.rbegin(); uiObjs != uiObjList.rend(); uiObjs++)
	{
		for (auto it = (*uiObjs).second.rbegin(); it != (*uiObjs).second.rend(); it++)
		{
			if ((*it) != nowEvObj)
				(*it)->EventClear();

			if (!isEvent)
			{
				(*it)->Update(dt);
			}
			if ((*it)->GetEvent() && !isEvent)
			{
				isEvent = true;
				nowEvObj = *it;
			}
		}
	}


	if (nowEvObj != nullptr && ((nowEvObj->GetState() == UiState::Exit) || (nowEvObj->GetState() == UiState::Enter)))
	{
	    for (auto uiObjs = uiObjList.rbegin(); uiObjs != uiObjList.rend(); uiObjs++)
	    {
	        for (auto it = (*uiObjs).second.rbegin(); it != (*uiObjs).second.rend(); it++)
	        {
	            if ((*it != nowEvObj))
	            {
	                if ((*it)->GetState() == UiState::Stay || (*it)->GetState() == UiState::Down)
	                {
	                    (*it)->SetState(UiState::Exit); 
	                    (*it)->SetEvent(true);
	                }
	                if ((*it)->GetState() == UiState::Stay || (*it)->GetState() == UiState::DownRight)
	                {
	                    (*it)->SetState(UiState::Exit); 
	                    (*it)->SetEvent(true);
	                }
	                (*it)->ColorClear(); 
	            }
	        }
	    }
	    nowEvObj = nullptr;
	}
	
}

void MapToolUiMgr::Draw(RenderWindow& window)
{
	window.setView(parentScene->GetUiView());
	if (nowDraw != nullptr)
		nowDraw->Draw(window);

	if (!enabled)
		return;
	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
			obj->Draw(window);
	}
}

void MapToolUiMgr::SetPos(const Vector2f& pos)
{
	Vector2f delta = pos - position;
	for (auto& uiObjs : uiObjList)
	{
		for (auto& obj : uiObjs.second)
			obj->Translate(delta);
	}
	position = pos;
	Object::SetPos(pos);
}

void MapToolUiMgr::Select(DrawSelect* select)
{
	if (nowDraw == nullptr)
		nowDraw = new DrawObj(this);
	nowDraw->SetTexture(*RESOURCE_MGR->GetTexture(select->GetTexturePath()), true);
	nowDraw->SetOrigin(Origins::BC);
	nowDraw->SetType(select->GetType());
	nowDraw->SetPath(select->GetPath());
	nowDraw->SetData(select->GetData());
	((MapToolScene*)parentScene)->SetType(select->GetType());
}

void MapToolUiMgr::DeleteDraw()
{
	delete nowDraw;
	nowDraw = nullptr;
}

bool MapToolUiMgr::IsUnder()
{
	auto mousePos = InputMgr::GetMousePos();
	mousePos = SCENE_MGR->GetCurrentScene()->ScreenToUiPosition((Vector2i)mousePos);
	//cout << Utils::IsRange(paletteBook->GetSpriteObj()->GetGlobalBounds(), mousePos) << endl;
	return Utils::IsRange(paletteBook->GetSpriteObj()->GetGlobalBounds(), mousePos);
}

void MapToolUiMgr::SetLoadPath(string path)
{
	saveWindow->SetPath(path);
}
void MapToolUiMgr::SetLoadInit()
{
	loadWindow->Reset();
}


bool MapToolUiMgr::IsSave()
{
	return saveWindow->IsSave();
}

bool MapToolUiMgr::IsLoad()
{
	return loadWindow->IsLoad();
}

bool MapToolUiMgr::LoadActive()
{
	return loadWindow->GetActive();
}

string MapToolUiMgr::loadFile()
{
	return loadWindow->GetLoadPath();
}
//
//bool MapToolUiMgr::IsExit()
//{
//	return exitBtn->IsDown() || exitBtn->IsClick();
//}

string MapToolUiMgr::GetPath()
{
	return saveWindow->GetPath();
}