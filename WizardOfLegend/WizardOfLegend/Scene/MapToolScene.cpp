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
			worldView.setSize(size.x * 1.06f, size.y * 1.06f);
		}
		if (InputMgr::GetMouseWheelMoved() > 0)
		{
			Vector2f size = worldView.getSize();
			worldView.setSize(size.x * 0.96f, size.y * 0.96f);
		}
		if (InputMgr::GetMouseButton(Mouse::Middle))
		{
			Vector2f pos = InputMgr::GetMousePosDisplacement();
			worldView.setCenter(worldView.getCenter() + pos);
		}
	}
	
	for (int i = 0; i < HEIGHTCNT; i++)
	{
		for (int j = 0; j < WIDTHCNT; j++)
		{
			if (grids[i][j]->IsClick())
			{
				//cout << i << "," << j << endl;
				if (nowType == LayerType::Object && playerPos == Vector2i{ i,j })
					return;

				//DrawObj* nowDraw = ((MapToolUiMgr*)uiMgr)->GetDraw();
				nowDraw = ((MapToolUiMgr*)uiMgr)->GetDraw();
				auto& nowgridObjs = gridObjs[nowType];


				//삭제코드
				if (nowDraw == nullptr || ((MapToolUiMgr*)uiMgr)->IsPaletteBook())
				{
					Button* findObj = nullptr;
					if (nowgridObjs.find(i) != nowgridObjs.end())
					{
						if (nowgridObjs[i].find(j) != nowgridObjs[i].end())
						{
							findObj = nowgridObjs[i][j];
							auto deleteObj = find(objList[nowType][i].begin(), objList[nowType][i].end(), findObj);
							objList[nowType][i].erase(deleteObj);
							gridObjs[nowType][i].erase(nowgridObjs[i].find(j));

							delete findObj;
						}
					}
					return;
				}

				Button* findObj = nullptr;
				if (nowgridObjs.find(i) != nowgridObjs.end())
				{
					if (nowgridObjs[i].find(j) != nowgridObjs[i].end())
					{
						findObj = nowgridObjs[i][j];

						if (nowDraw->GetType() == "PLAYER")
							return;

						auto deleteObj = find(objList[nowType][i].begin(), objList[nowType][i].end(), findObj);
						objList[nowType][i].erase(deleteObj);
						gridObjs[nowType][i].erase(nowgridObjs[i].find(j));

						delete findObj;
					}
				}

				if (nowDraw->GetType() == "SECTOR")
				{
					if (InputMgr::GetMouseButtonDown(Mouse::Left))
					{					
						sector = new Sector();
						sector->SetPos(grids[i][j]->GetPos());
						cout << "sector1 " << grids[i][j]->GetPos().x << "," << grids[i][j]->GetPos().y << endl;
						isNowDraw = true;
					}
					sector->UpdateNowDraw(dt,nowDraw);
					sector->SetSize({ grids[i][j]->GetPos().x - sector->GetPos().x+16,grids[i][j]->GetPos().y - sector->GetPos().y+16 });
					cout<<"grids " << grids[i][j]->GetPos().x << "," << grids[i][j]->GetPos().y << endl;
					cout<<"sector2 " << sector->GetPos().x << "," << sector->GetPos().y << endl;
					

					//endPos = InputMgr::GetMousePos();
					//sector = new RectangleShape();se
					//sector->setPosition({ (Vector2f)startPos});
					//sector->setFillColor({ 0,0,0,0 });
					//sector->setOutlineColor(Color::Red);
					//sector->setOutlineThickness(4.f);
					//sector->setSize({ endPos.x - startPos.x,endPos.y - startPos.y });
					//isNowDraw = true;
				}
				else
				{
					DrawObj* draw = new DrawObj(uiMgr);
					draw->SetType(nowDraw->GetType());
					draw->SetPath(nowDraw->GetPath());
					draw->SetTexture(*RESOURCE_MGR->GetTexture(draw->GetPath()), true);
					draw->SetOrigin(Origins::BC);
					draw->SetMove(false);
					draw->SetPos(grids[i][j]->GetPos() + Vector2f{ 8.f, 16.f });
					draw->SetData(nowDraw->GetData());

					objList[nowType][i].push_back(draw);
					gridObjs[nowType][i][j] = draw;

					if (nowDraw->GetType() == "PLAYER")
					{
						if (player != nullptr)
						{
							int pi = playerPos.x;
							int pj = playerPos.y;
							if (nowgridObjs.find(pi) != nowgridObjs.end())
							{
								if (nowgridObjs[pi].find(pj) != nowgridObjs[pi].end())
								{
									findObj = nowgridObjs[pi][pj];
									auto deleteObj = find(objList[nowType][pi].begin(), objList[nowType][pi].end(), findObj);
									objList[nowType][pi].erase(deleteObj);
									gridObjs[nowType][pi].erase(nowgridObjs[pi].find(pj));

									delete findObj;
								}
							}
						}
						player = draw;
						playerPos = { i,j };

					}
				}
				
			}
			if (nowDraw != nullptr&&isNowDraw)
			{
				if (nowDraw->GetType() == "SECTOR" && InputMgr::GetMouseButtonUp(Mouse::Left))
				{
					//sectors.push_back(sector);
					isNowDraw = false;

					objList[nowType][i].push_back(sector);
					sectors[nowType][i][j] = sector;
					cout << "up" << endl;
				}
			}
		}
	}
}

void MapToolScene::Draw(RenderWindow& window)
{
	//uiMgr->Draw(window);
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
