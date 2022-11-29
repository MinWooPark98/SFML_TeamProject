#include "MapToolScene.h"
#include "../GameObject/Button.h"
#include "../Framework/ResourceMgr.h"
#include "../Scene/SceneMgr.h"
#include "../Ui/MapToolUiMgr.h"
#include "../GameObject/DrawObj.h"
#include "../Framework/InputMgr.h"
#include <algorithm>
#include "../Framework/Framework.h"
#include "../GameObject/Sector.h"

MapToolScene::MapToolScene()
	: Scene(Scenes::MapTool)
{
}

MapToolScene::~MapToolScene()
{
}

void MapToolScene::Init()
{
	Scene::Init();

	for (int i = 0; i < HEIGHTCNT; i++)
	{
		for (int j = 0; j < WIDTHCNT; j++)
		{
			auto tile = new Button(nullptr);
			grids[i].push_back(tile);
			tile->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Map/grid.png"), true);
			tile->SetPos({ 16.f * j, 16.f * i });
			objList[LayerType::Back][i].push_back(tile);
			tile->SetUiView(false);
		}
	}
	
	uiMgr = new MapToolUiMgr(this);
	uiMgr->Init();
	nowType = LayerType::Object;
}

void MapToolScene::Reset()
{
	Release();
}

void MapToolScene::Update(float dt)
{
	Scene::Update(dt);
	//uiMgr->Update(dt);
	//최소점 최대점

	auto uimgr = ((MapToolUiMgr*)uiMgr);
	if (InputMgr::GetKeyDown(Keyboard::Escape))
	{
		SCENE_MGR->ChangeScene(Scenes::Title);
		return;
	}
	if (uimgr->IsSave())
	{
		Save();
		return;
	}
	if (uimgr->IsLoad())
	{
		string path = uimgr->loadFile();
		Load(path);
		return;
	}
	if (!uimgr->LoadActive())
	{
		if (InputMgr::GetMouseButton(Mouse::Right))
		{
			uimgr->DeleteDraw();
			return;
		}
		if (InputMgr::GetMouseWheelMoved() < 0)
		{
			Vector2f size = worldView.getSize();
			worldView.setSize(size.x * 1.1f, size.y * 1.1f);
		}
		if (InputMgr::GetMouseWheelMoved() > 0)
		{
			Vector2f size = worldView.getSize();
			worldView.setSize(size.x * 0.9f, size.y * 0.9f);
		}
		if (InputMgr::GetMouseButton(Mouse::Middle))
		{
			Vector2f pos = InputMgr::GetMousePosDisplacement();
			worldView.setCenter(worldView.getCenter() + pos);
		}
	}
	
	
	Vector2f v = objMousePos;
	v.x /= 16;
	v.y /= 16;
	int i = v.x;
	int j = v.y;
	cout << endl;
	cout << i << endl;
	cout << j << endl;
	if (i < 150 && j < 150&& i > 0 && j > 0)
	{
		if (grids[j][i]->IsClick())
		{
			//cout << i << "," << j << endl;
			if (nowType == LayerType::Object && playerPos == Vector2i{ j,i })
				return;

			//DrawObj* nowDraw = ((MapToolUiMgr*)uiMgr)->GetDraw();
			nowDraw = ((MapToolUiMgr*)uiMgr)->GetDraw();
			auto& nowgridObjs = gridObjs[nowType];
			auto& nowsectorObjs = sectors[nowType];


			//삭제코드
			if (nowDraw == nullptr || ((MapToolUiMgr*)uiMgr)->IsPaletteBook())
			{
				Button* findObj = nullptr;
				if (nowgridObjs.find(j) != nowgridObjs.end())
				{
					if (nowgridObjs[j].find(i) != nowgridObjs[j].end())
					{
						findObj = nowgridObjs[j][i];
						auto deleteObj = find(objList[nowType][i].begin(), objList[nowType][i].end(), findObj);
						objList[nowType][j].erase(deleteObj);
						gridObjs[nowType][j].erase(nowgridObjs[j].find(i));

						delete findObj;
					}
				}
				return;
			}
			if (nowDraw == nullptr || ((MapToolUiMgr*)uiMgr)->IsPaletteBook())
			{
				Sector* findObj = nullptr;
				if (nowsectorObjs.find(j) != nowsectorObjs.end())
				{
					if (nowsectorObjs[j].find(i) != nowsectorObjs[j].end())
					{
						findObj = nowsectorObjs[j][i];
						auto deleteObj = find(objList[nowType][j].begin(), objList[nowType][j].end(), findObj);
						objList[nowType][j].erase(deleteObj);
						sectors[nowType][j].erase(nowsectorObjs[j].find(i));

						delete findObj;
					}
				}
				return;
			}

			Button* findObj = nullptr;
			if (nowgridObjs.find(j) != nowgridObjs.end())
			{
				if (nowgridObjs[j].find(i) != nowgridObjs[j].end())
				{
					findObj = nowgridObjs[j][i];

					if (nowDraw->GetType() == "PLAYER")
						return;

					auto deleteObj = find(objList[nowType][j].begin(), objList[nowType][j].end(), findObj);
					objList[nowType][j].erase(deleteObj);
					gridObjs[nowType][j].erase(nowgridObjs[j].find(i));

					delete findObj;
				}
			}

			if (nowDraw->GetType() == "SECTOR")
			{
				if (InputMgr::GetMouseButtonDown(Mouse::Left))
				{
					sector = new Sector();
					sector->SetPos(grids[j][i]->GetPos());
					cout << "sector1 " << grids[j][i]->GetPos().x << "," << grids[j][i]->GetPos().y << endl;
					isNowDraw = true;
					sectorJ = j;
					sectorI = i;
				}
				sector->UpdateNowDraw(dt, nowDraw);
				sector->SetSize({ grids[j][i]->GetPos().x - sector->GetPos().x + 16,grids[j][i]->GetPos().y - sector->GetPos().y + 16 });
				cout << "grids " << grids[j][i]->GetPos().x << "," << grids[j][i]->GetPos().y << endl;
				cout << "sector2 " << sector->GetPos().x << "," << sector->GetPos().y << endl;

			}
			else
			{
				DrawObj* draw = new DrawObj(uiMgr);
				draw->SetType(nowDraw->GetType());
				draw->SetPath(nowDraw->GetPath());
				draw->SetTexture(*RESOURCE_MGR->GetTexture(draw->GetPath()), true);
				draw->SetOrigin(Origins::BC);
				draw->SetMove(false);
				draw->SetPos(grids[j][i]->GetPos() + Vector2f{ 8.f, 16.f });
				draw->SetData(nowDraw->GetData());

				objList[nowType][j].push_back(draw);
				gridObjs[nowType][j][i] = draw;

				if (nowDraw->GetType() == "PLAYER")
				{
					if (player != nullptr)
					{
						int pj = playerPos.x;
						int pi = playerPos.y;
						if (nowgridObjs.find(pj) != nowgridObjs.end())
						{
							if (nowgridObjs[pj].find(pi) != nowgridObjs[pj].end())
							{
								findObj = nowgridObjs[pj][pi];
								auto deleteObj = find(objList[nowType][pj].begin(), objList[nowType][pj].end(), findObj);
								objList[nowType][pj].erase(deleteObj);
								gridObjs[nowType][pj].erase(nowgridObjs[pj].find(pi));

								delete findObj;
							}
						}
					}
					player = draw;
					playerPos = { j,i };

				}
			}

		}
		if (nowDraw != nullptr && isNowDraw)
		{
			if (nowDraw->GetType() == "SECTOR" && InputMgr::GetMouseButtonUp(Mouse::Left))
			{
				isNowDraw = false;

				objList[nowType][sectorJ].push_back(sector);
				sectors[nowType][sectorJ][sectorI] = sector;
				cout << "up" << endl;
			}
		}
	}
	//for (int i = 0; i < HEIGHTCNT; i++)
	//{
	//	for (int j = 0; j < WIDTHCNT; j++)
	//	{
			
	//	}
	//}
}

void MapToolScene::Draw(RenderWindow& window)
{
	//uiMgr->Draw(window);
	Vector2i min = { (int)(worldView.getCenter().x - (int)worldView.getSize().x * 0.5f), (int)(worldView.getCenter().y - (int)worldView.getSize().y * 0.5f) };
	Vector2i max = { (int)(worldView.getCenter().x + (int)worldView.getSize().x * 0.5f), (int)(worldView.getCenter().y + (int)worldView.getSize().y * 0.5f) };
	for (auto& layer : objList)
	{
		for (auto& obj_pair : layer.second)
		{
			auto& objs = obj_pair.second;
			for (auto& obj : objs)
			{
				if (obj->GetPos().x<max.x+16&& obj->GetPos().y < max.y+16&& obj->GetPos().x > min.x-16 && obj->GetPos().y > min.y-16)
				{
					obj->SetActive(true);
				}
				else
				{
					obj->SetActive(false);
				}
			}
		}
	}
	Scene::Draw(window);
	if (sector != nullptr)
	{
		window.setView(worldView);
		sector->Draw(window);
	}
	//for (auto sectorlist : sectors)
	//{
	//	window.draw(*sectorlist);
	//}
}


void MapToolScene::Enter()
{
	float WindowWidth = FRAMEWORK->GetWindowSize().x;
	float WindowHeight = FRAMEWORK->GetWindowSize().y;
	SCENE_MGR->GetCurrentScene()->GetWorldView().setCenter({ WindowWidth / 2.f, WindowHeight / 2.f });
	SCENE_MGR->GetCurrentScene()->GetWorldView().setSize({ WindowWidth , WindowHeight });
	SCENE_MGR->GetCurrentScene()->GetUiView().setCenter({ WindowWidth / 2.f, WindowHeight / 2.f });
	SCENE_MGR->GetCurrentScene()->GetUiView().setSize({ WindowWidth , WindowHeight });
}

void MapToolScene::Exit()
{
	Release();
}

void MapToolScene::Release()
{
	for (auto& objs : objList[LayerType::Object])
	{
		for (auto it = objs.second.begin(); it != objs.second.end();)
		{
			auto del = *it;
			it = objs.second.erase(it);
			if (del != nullptr)
			{
				delete del;
			}
		}
		objs.second.clear();
	}

	for (auto& objs : objList[LayerType::Tile])
	{
		for (auto it = objs.second.begin(); it != objs.second.end();)
		{
			auto del = *it;
			it = objs.second.erase(it);
			if (del != nullptr)
			{
				delete del;
			}
		}
		objs.second.clear();
	}
	for (auto& objs : objList[LayerType::Sector])
	{
		for (auto it = objs.second.begin(); it != objs.second.end();)
		{
			auto del = *it;
			it = objs.second.erase(it);
			if (del != nullptr)
			{
				delete del;
			}
		}
		objs.second.clear();
	}
	sectors.clear();

	objList[LayerType::Tile].clear();
	objList[LayerType::Object].clear();
	objList[LayerType::Sector].clear();
	gridObjs.clear();
	sectors.clear();

	player = nullptr;

}


void MapToolScene::SetType(string t)
{
	if (t == "SECTOR")
	{
		nowType = LayerType::Sector;
	}
	if (t == "OBJECT" || t == "ENEMY" || t == "PLAYER")
	{
		nowType = LayerType::Object;
	}
	if (t == "WALL" || t == "TILE")
	{
		nowType = LayerType::Tile;
	}
}

void MapToolScene::Save()
{
	saveObjs.clear();
	string path = ((MapToolUiMgr*)(uiMgr))->GetPath();
	for (auto& layer : gridObjs)
	{
		for (auto& objs : layer.second)
		{
			for (auto& obj : objs.second)
			{
				auto& nowObject = obj.second;
				ObjectData data;
				data.type = nowObject->GetType();
				data.path = nowObject->GetPath();
				data.position = nowObject->GetPos();
				data.size = nowObject->GetSize();
				saveObjs.push_back(data);
			}
		}
	}
	for (auto& layer : sectors)
	{
		for (auto& objs : layer.second)
		{
			for (auto& obj : objs.second)
			{
				auto& nowObject = obj.second;
				ObjectData data;
				data.type = nowObject->GetType();
				data.path = nowObject->GetPath();
				data.position = nowObject->GetPos();
				data.size = nowObject->GetSize();
				saveObjs.push_back(data);
			}
		}
	}
	
	if (path == "")
		return;

	FILE_MGR->SaveMap(saveObjs, path);
	((MapToolUiMgr*)uiMgr)->SetLoadInit();
}

void MapToolScene::Load(string path)
{
	for (auto& objs : objList[LayerType::Object])
	{
		for (auto it = objs.second.begin(); it != objs.second.end();)
		{
			auto del = *it;
			it = objs.second.erase(it);
			if (del != nullptr)
			{
				delete del;
			}
		}
		objs.second.clear();
	}

	for (auto& objs : objList[LayerType::Tile])
	{
		for (auto it = objs.second.begin(); it != objs.second.end();)
		{
			auto del = *it;
			it = objs.second.erase(it);
			if (del != nullptr)
			{
				delete del;
			}
		}
		objs.second.clear();
	}

	for (auto& objs : objList[LayerType::Sector])
	{
		for (auto it = objs.second.begin(); it != objs.second.end();)
		{
			auto del = *it;
			it = objs.second.erase(it);
			if (del != nullptr)
			{
				delete del;
			}
		}
		objs.second.clear();
	}
	objList[LayerType::Tile].clear();
	objList[LayerType::Object].clear();
	objList[LayerType::Sector].clear();
	gridObjs.clear();
	sectors.clear();
	//로드 파일 그리드에 적용
	player = nullptr;
	auto& data = FILE_MGR->GetMap(path);
	for (auto& obj : data)
	{
		DrawObj* draw = new DrawObj(uiMgr);
		draw->SetType(obj.type);
		draw->SetPath(obj.path);
		draw->SetTexture(*RESOURCE_MGR->GetTexture(draw->GetPath()), true);
		draw->SetOrigin(Origins::BC);
		draw->SetMove(false);
		draw->SetPos(obj.position);

		Sector* sector = new Sector();
		sector->SetType(obj.type);
		sector->SetPath(obj.path);
		sector->SetPos(obj.position);
		sector->SetSize(obj.size);

		int i = ((int)obj.position.x - 8) / 16;
		int j = (int)obj.position.y / 16 - 1;
		if (obj.type == "WALL" || obj.type == "OBJECT" || obj.type == "ENEMY" )
		{
			objList[LayerType::Object][j].push_back(draw);
			gridObjs[LayerType::Object][j][i] = draw;

			if (obj.type == "PLAYER")
			{
				player = draw;
				playerPos = Vector2i{ j,i };
			}
		}
		else if (obj.type == "TILE")
		{
			objList[LayerType::Tile][j].push_back(draw);
			gridObjs[LayerType::Tile][j][i] = draw;
		}

		else if (obj.type == "SECTOR")
		{
			objList[LayerType::Sector][j].push_back(sector);
			sectors[LayerType::Sector][j][i] = sector;
		}
	}

	((MapToolUiMgr*)uiMgr)->SetLoadPath(path);
}
