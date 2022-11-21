#include "MapToolScene.h"
#include "../GameObject/Button.h"
#include "../Framework/ResourceMgr.h"
#include "../Scene/SceneMgr.h"
#include "../Ui/MapToolUiMgr.h"
#include "../GameObject/DrawObj.h"
#include "../Framework/InputMgr.h"
#include <algorithm>
#include "../Framework/Framework.h"

MapToolScene::MapToolScene()
	: Scene(Scenes::MapTool)
{
}

void MapToolScene::Init()
{
	Reset();
}

void MapToolScene::Reset()
{
	for (int i = 0; i < HEIGHTCNT; i++)
	{
		for (int j = 0; j < WIDTHCNT; j++)
		{
			auto tile = new Button(nullptr);
			greeds[i].push_back(tile);
			tile->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Map/greed.png"), true);
			tile->SetPos({ 16.f * j, 16.f * i });
			objList[LayerType::Back][i].push_back(tile);
			tile->SetUiView(false);
		}
	}
	
	uiMgr = new MapToolUiMgr(this);
	uiMgr->Init();
	nowType = LayerType::Object;

	//DrawObj* draw = new DrawObj(uiMgr);
	//auto editorObjs = FILE_MGR->GetEditorObjs();
	//auto playerData = editorObjs["PLAYER"];
	//draw->SetType("PLAYER");
	//draw->SetPath(playerData[0].texPath);
	//draw->SetTexture(*RESOURCE_MGR->GetTexture(draw->GetPath()), true);
	//draw->SetOrigin(Origins::BC);
	//draw->SetMove(false);
	//draw->SetPos(greeds[0][0]->GetPos() + Vector2f{ 30.f, 60.f });
	//draw->SetData(playerData[0]);
	//objList[nowType][0].push_back(draw);
	//greedObjs[nowType][0][0] = draw;

	//player = draw;
}

void MapToolScene::Update(float dt)
{
	Scene::Update(dt);
	uiMgr->Update(dt);

	auto uimgr = ((MapToolUiMgr*)uiMgr);
	if (uimgr->IsExit() || InputMgr::GetKeyDown(Keyboard::Escape))
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
	for (int i = 0; i < HEIGHTCNT; i++)
	{
		for (int j = 0; j < WIDTHCNT; j++)
		{
			if (greeds[i][j]->IsClick())
			{
				if (nowType == LayerType::Object && playerPos == Vector2i{ i,j })
					return;

				DrawObj* nowDraw = ((MapToolUiMgr*)uiMgr)->GetDraw();
				auto& nowGreedObjs = greedObjs[nowType];

				if (nowDraw == nullptr || ((MapToolUiMgr*)uiMgr)->IsUnder())
				{
					Button* findObj = nullptr;
					if (nowGreedObjs.find(i) != nowGreedObjs.end())
					{
						if (nowGreedObjs[i].find(j) != nowGreedObjs[i].end())
						{
							findObj = nowGreedObjs[i][j];
							auto deleteObj = find(objList[nowType][i].begin(), objList[nowType][i].end(), findObj);
							objList[nowType][i].erase(deleteObj);
							greedObjs[nowType][i].erase(nowGreedObjs[i].find(j));

							delete findObj;
						}
					}
					return;
				}

				Button* findObj = nullptr;
				if (nowGreedObjs.find(i) != nowGreedObjs.end())
				{
					if (nowGreedObjs[i].find(j) != nowGreedObjs[i].end())
					{
						findObj = nowGreedObjs[i][j];

						if (nowDraw->GetType() == "PLAYER")
							return;

						auto deleteObj = find(objList[nowType][i].begin(), objList[nowType][i].end(), findObj);
						objList[nowType][i].erase(deleteObj);
						greedObjs[nowType][i].erase(nowGreedObjs[i].find(j));

						delete findObj;
					}
				}

				DrawObj* draw = new DrawObj(uiMgr);
				draw->SetType(nowDraw->GetType());
				draw->SetPath(nowDraw->GetPath());
				draw->SetTexture(*RESOURCE_MGR->GetTexture(draw->GetPath()), true);
				draw->SetOrigin(Origins::BC);
				draw->SetMove(false);
				draw->SetPos(greeds[i][j]->GetPos() + Vector2f{ 8.f, 16.f });
				cout << greeds[0][0]->GetPos().x << "," << greeds[0][0]->GetPos().y << endl;
				draw->SetData(nowDraw->GetData());
				objList[nowType][i].push_back(draw);
				greedObjs[nowType][i][j] = draw;

				if (nowDraw->GetType() == "PLAYER")
				{
					if (player != nullptr)
					{
						int pi = playerPos.x;
						int pj = playerPos.y;
						if (nowGreedObjs.find(pi) != nowGreedObjs.end())
						{
							if (nowGreedObjs[pi].find(pj) != nowGreedObjs[pi].end())
							{
								findObj = nowGreedObjs[pi][pj];
								auto deleteObj = find(objList[nowType][pi].begin(), objList[nowType][pi].end(), findObj);
								objList[nowType][pi].erase(deleteObj);
								greedObjs[nowType][pi].erase(nowGreedObjs[pi].find(pj));

								delete findObj;
							}
						}
					}
					player = draw;
					playerPos = { i,j };
				}
			}
		}
	}
}

void MapToolScene::Draw(RenderWindow& window)
{
	uiMgr->Draw(window);
	Scene::Draw(window);
}


void MapToolScene::Enter()
{
	float WindowWidth = FRAMEWORK->GetWindowSize().x;
	float WindowHeight = FRAMEWORK->GetWindowSize().y;
	SCENE_MGR->GetCurrentScene()->GetWorldView().setCenter({ WindowWidth / 2.f, WindowHeight / 2.f });
	SCENE_MGR->GetCurrentScene()->GetWorldView().setSize({ WindowWidth , WindowHeight });
	SCENE_MGR->GetCurrentScene()->GetUiView().setCenter({ WindowWidth / 2.f, WindowHeight / 2.f });
	SCENE_MGR->GetCurrentScene()->GetUiView().setSize({ WindowWidth , WindowHeight });
	Init();
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
	objList[LayerType::Tile].clear();
	objList[LayerType::Object].clear();
	greedObjs.clear();

	player = nullptr;

}

MapToolScene::~MapToolScene()
{
}

void MapToolScene::SetType(string t)
{
	if (t == "TREE" || t == "STONE" || t == "ENEMY" || t == "PLAYER" || t == "BLOCK")
	{
		nowType = LayerType::Object;
	}
	if (t == "TILE")
	{
		nowType = LayerType::Tile;
	}
}

void MapToolScene::Save()
{
	saveObjs.clear();
	string path = ((MapToolUiMgr*)(uiMgr))->GetPath();
	for (auto& layer : greedObjs)
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
	objList[LayerType::Tile].clear();
	objList[LayerType::Object].clear();
	greedObjs.clear();

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

		int i = ((int)obj.position.x - 30) / 60;
		int j = (int)obj.position.y / 60 - 1;
		if (obj.type == "TREE" || obj.type == "STONE" || obj.type == "ENEMY" || obj.type == "PLAYER" || obj.type == "BLOCK")
		{
			objList[LayerType::Object][j].push_back(draw);
			greedObjs[LayerType::Object][j][i] = draw;

			if (obj.type == "PLAYER")
			{
				player = draw;
				playerPos = Vector2i{ j,i };
			}
		}
		else if (obj.type == "TILE")
		{
			objList[LayerType::Tile][j].push_back(draw);
			greedObjs[LayerType::Tile][j][i] = draw;
		}
	}

	((MapToolUiMgr*)uiMgr)->SetLoadPath(path);
}
