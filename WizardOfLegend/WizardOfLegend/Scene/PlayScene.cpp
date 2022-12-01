#include "PlayScene.h"
#include "../GameObject/Lancer.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../GameObject/Skill.h"
#include "../GameObject/Projectile.h"
#include "../GameObject/Archer.h"
#include "../GameObject/HeavyBombingArcher.h"
#include "../GameObject/FireBoss.h"
#include "../Framework/InputMgr.h"
#include "../Scene/SceneMgr.h"
#include "../Ui/PlayUiMgr.h"
#include "../GameObject/SkillSet.h"
#include "../GameObject/Sector.h"

PlayScene::PlayScene()
	:Scene(Scenes::Play)
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Init()
{
	Scene::Init();

	isMap = true;
	auto& data = FILE_MGR->GetMap("TUTORIAL");

	for (auto& obj : data)
	{
		if (obj.type == "SECTOR")
		{
			Sector* sector = new Sector();
			sector->SetName(obj.type);
			sector->SetPos(obj.position);
			sector->SetSize(obj.size);
			sector->SetHitBox((FloatRect)sector->GetSectorShape()->getGlobalBounds());
			sector->SetObjType(Object::ObjTypes::Sector);
			room.push_back(*sector);
			objList[LayerType::Sector][0].push_back(sector);
		}
	}
	
	for (int i = 0; i < room.size(); i++)
	{
		collisionList.push_back(map<Object::ObjTypes, list<Object*>>());
	}

	for (auto& obj : data)
		{

		if (obj.type == "WALL")
		{
			SpriteObj* draw = new SpriteObj();
			draw->SetName(obj.type);
			draw->SetTexture(*RESOURCE_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);
			draw->SetHitBox(obj.path);
			draw->SetObjType(Object::ObjTypes::Wall);
			if (obj.path == "graphics/Map/Palette/Empty.png")
			{
				draw->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Map/Empty.png"));
				draw->SetObjType(Object::ObjTypes::rnejddl);				
			}

			objList[LayerType::Object][0].push_back(draw);
		}
		else if (obj.type == "TILE")
		{
			SpriteObj* draw = new SpriteObj();
			draw->SetName(obj.type);
			draw->SetTexture(*RESOURCE_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);
			draw->SetObjType(Object::ObjTypes::Tile);

			objList[LayerType::Tile][0].push_back(draw);
		}
		else if (obj.type == "OBJECT")
		{
			SpriteObj* draw = new SpriteObj();
			draw->SetName(obj.type);
			draw->SetTexture(*RESOURCE_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);
			draw->SetHitBox(obj.path);
			draw->SetObjType(Object::ObjTypes::ETC);

			objList[LayerType::Object][0].push_back(draw);
		}
		else if (obj.type == "PLAYER")
		{
			player = new Player();
			player->Init();
			player->SetName(obj.type);
			player->SetPos(obj.position);
			player->SetObjType(Object::ObjTypes::Player);
			objList[LayerType::Object][5].push_back(player);
			collisionList[0][Object::ObjTypes::Player].push_back(player);
			auto& skillSet = player->GetSkillSets();
			skillSet[0]->Set("FireBall");
			skillSet[1]->Set("JumpMeteor");
			skillSet[2]->Set("FireDash");
			skillSet[4]->Set("DragonArc");
			skillSet[5]->Set("FireFull");
		}
		else if (obj.type == "ENEMY")
		{
			if (obj.path == "graphics/Map/Lancer.png")
			{
				Lancer* lancer = new Lancer();
				lancer->Init();
				lancer->SetName(obj.type);
				lancer->SetPos(obj.position);
				lancer->SetCardPos(lancer->GetPos());
				lancer->SetObjType(Object::ObjTypes::Enemy);
				objList[LayerType::Object][0].push_back(lancer);
				collisionList[0][Object::ObjTypes::Enemy].push_back(lancer);
			}
			else if (obj.path == "graphics/Map/ArcherNormal.png")
			{
				Archer* archer = new Archer();
				archer->Init();
				archer->SetName(obj.type);
				archer->SetPos(obj.position);
				archer->SetCardPos(archer->GetPos());
				archer->SetObjType(Object::ObjTypes::Enemy);
				archer->SetColor(3);
				objList[LayerType::Object][1].push_back(archer);
				collisionList[0][Object::ObjTypes::Enemy].push_back(archer);
			}
			else if (obj.path == "graphics/Map/ArcherBosspng.png")
			{
				HeavyBombingArcher* heavyBombingArcher = new HeavyBombingArcher();
				heavyBombingArcher->Init();
				heavyBombingArcher->SetName(obj.type);
				heavyBombingArcher->SetPos(obj.position);
				heavyBombingArcher->SetCardPos(heavyBombingArcher->GetPos());
				heavyBombingArcher->SetObjType(Object::ObjTypes::Enemy);

				heavyBombingArcher->SetColor(2);
				objList[LayerType::Object][2].push_back(heavyBombingArcher);
				collisionList[0][Object::ObjTypes::Enemy].push_back(heavyBombingArcher);
			}
			else if (obj.path == "graphics/Map/FireBoss.png")
			{
				fireBoss = new FireBoss();
				fireBoss->Init();
				fireBoss->SetName(obj.type);
				fireBoss->SetPos(obj.position);
				fireBoss->SetObjType(Object::ObjTypes::Enemy);
				objList[LayerType::Object][3].push_back(fireBoss);
				collisionList[0][Object::ObjTypes::Enemy].push_back(fireBoss);
			}
		}
	}

	for (auto& a : objList)
	{
		for (auto& b : a.second)
		{
			for (auto& c : b.second)
			{
				for (int i = 0; i < room.size(); i++)
				{
					if (Utils::OBB(c->GetHitBox(), room[i].GetHitBox()))
					{
						collisionList[i][c->GetObjType()].push_back(c);
						cout << (int)c->GetObjType() << endl; // 5 = Sector
					}
				}
			}
		}
	}

	auto& tiles = objList[LayerType::Tile][0];
	mapSize.left = 0;
	mapSize.top = 0;
	mapSize.width = (tiles.back())->GetPos().x + 16;
	mapSize.height = (tiles.back())->GetPos().y;

	for (auto& enemy : collisionList[0][Object::ObjTypes::Enemy])
	{
		((Enemy*)enemy)->SetPlayer(player);
	}

	uiMgr = new PlayUiMgr();
	uiMgr->Init();
}

void PlayScene::Update(float dt)
{
	worldView.setCenter(player->GetPos());
	Scene::Update(dt);

	if (InputMgr::GetKeyDown(Keyboard::Key::Escape))
	{
		if (!GetPause())
			SetPause(true);
		else
			SetPause(false);
	}

	for (int i = 0; i < room.size(); i++)
	{
		if (player->GetLowHitBounds().intersects(room[i].GetHitBounds()))
		{
			collisionList[i][player->GetObjType()].push_back(player);
			playerRoom = i;
		}
	}

	if (!collisionList[playerRoom].empty())
	{
		for (auto& enemy : collisionList[playerRoom][Object::ObjTypes::Enemy])
		{
			for (auto& coll : collisionList[playerRoom][Object::ObjTypes::Wall])
			{
				if (enemy->GetLowHitBounds().intersects(coll->GetHitBounds()))
				{
					bool leftandRight = false;
					bool topandLow = false;

					// 적 우점
					float enemyRightPoint = enemy->GetLowHitBounds().width + enemy->GetLowHitBounds().left;
					// 적 하점
					float enemyLowPoint = enemy->GetLowHitBounds().height + enemy->GetLowHitBounds().top;

					// 벽 x
					float collXPoint = (coll->GetHitBounds().width * 0.5f) + coll->GetHitBounds().left;
					// 벽 y
					float collYPoint = (coll->GetHitBounds().height * 0.5f) + coll->GetHitBounds().top;

					if (enemy->GetLowHitBounds().left >= collXPoint || enemyRightPoint <= collXPoint)
					{
						leftandRight = true;
					}
					if (enemy->GetLowHitBounds().height <= collYPoint || enemyLowPoint >= collYPoint)
					{
						topandLow = true;
					}

					if (topandLow)
					{
						enemy->SetPos({ enemy->GetPos().x, enemy->GetLastPosition().y });
					}
					if (leftandRight)
					{
						enemy->SetPos({ enemy->GetLastPosition().x, enemy->GetPos().y });
					}
				}
			}
		}
	}

	for (auto& coll : collisionList[playerRoom][Object::ObjTypes::Wall])
	{
		if (player->GetLowHitBounds().intersects(coll->GetHitBounds()))
		{
			bool leftandRight = false;
			bool topandLow = false;

			float playerRightPoint = player->GetLowHitBounds().width + player->GetLowHitBounds().left;
			float playerLowPoint = player->GetLowHitBounds().height + player->GetLowHitBounds().top;

			float collXPoint = (coll->GetHitBounds().width * 0.5f) + coll->GetHitBounds().left;
			float collYPoint = (coll->GetHitBounds().height * 0.5f) + coll->GetHitBounds().top;

			if (player->GetLowHitBounds().height <= collYPoint || playerLowPoint >= collYPoint)
				topandLow = true;
			if (player->GetLowHitBounds().left >= collXPoint || playerRightPoint <= collXPoint)
				leftandRight = true;

			if (topandLow)
			{
				player->SetPos({ player->GetPos().x, player->GetLastPosition().y });
			}
			if (leftandRight)
			{
				player->SetPos({ player->GetLastPosition().x, player->GetPos().y });
			}
		}
	}
}

void PlayScene::Draw(RenderWindow& window)
{
	Vector2i min = { (int)(worldView.getCenter().x - (int)worldView.getSize().x * 0.5f), (int)(worldView.getCenter().y - (int)worldView.getSize().y * 0.5f) };
	Vector2i max = { (int)(worldView.getCenter().x + (int)worldView.getSize().x * 0.5f), (int)(worldView.getCenter().y + (int)worldView.getSize().y * 0.5f) };
	for (auto& layer : objList)
	{
		for (auto& obj_pair : layer.second)
		{
			auto& objs = obj_pair.second;
			for (auto& obj : objs)
			{
				if (obj->GetPos().x<max.x + 32 && obj->GetPos().y < max.y + 32 && obj->GetPos().x > min.x - 32 && obj->GetPos().y > min.y - 32)
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
}

void PlayScene::Release()
{
	Scene::Release();
}

void PlayScene::Reset()
{
	Scene::Reset();
}

void PlayScene::Enter()
{
	Scene::Enter();
	Vector2f size = (Vector2f)FRAMEWORK->GetWindowSize();
	worldView.setSize(size * 0.25f);
	worldView.setCenter(size * 0.125f);

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);
}

void PlayScene::Exit()
{
	Scene::Exit();
}
