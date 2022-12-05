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
#include "../GameObject/Cliff.h"
#include "../GameObject/CastingCircle.h"
#include "../GameObject/FinalBoss.h"

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

	uiMgr = new PlayUiMgr();
	uiMgr->Init();

	isMap = true;
	auto& data = FILE_MGR->GetMap("TUTORIAL");

	uiMgr = new PlayUiMgr();
	uiMgr->Init();

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
			sector->SetOutlineColor({ 0, 0, 0, 0 });
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
			draw->Init();
			draw->SetName(obj.type);
			draw->SetTexture(*RESOURCE_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);
			draw->SetHitBox(obj.path);
			draw->SetObjType(Object::ObjTypes::Wall);

			objList[LayerType::Object][0].push_back(draw);
		}
		else if (obj.type == "TILE")
		{
			SpriteObj* draw = new SpriteObj();
			draw->Init();
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
			draw->Init();
			draw->SetName(obj.type);
			draw->SetFileName(obj.path);
			draw->SetTexture(*RESOURCE_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);
			draw->SetHitBox(obj.path);
			draw->SetObjType(Object::ObjTypes::ETC);
			if (obj.path == "graphics/Map/Object/GateFull.png" ||
				obj.path == "graphics/Map/Palette/LeftGate.png" ||
				obj.path == "graphics/Map/Palette/RightGate.png")
			{
				draw->SetActive(false);
			}
			objList[LayerType::Object][0].push_back(draw);
		}
		else if (obj.type == "PLAYER")
		{
			player = new Player();
			((PlayUiMgr*)uiMgr)->SetPlayer(player);
			player->Init();
			((PlayUiMgr*)uiMgr)->SetPlayer(player);
			player->SetName(obj.type);
			player->SetPos(obj.position);
			player->SetObjType(Object::ObjTypes::Player);
			player->SetLastPosition({ 0,0 });
			objList[LayerType::Object][5].push_back(player);
			auto& skillSet = player->GetSkillSets();
			skillSet[0]->Set("FireBall");
			skillSet[1]->Set("FireDash");
			skillSet[2]->Set("JumpMeteor");
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
				lancer->SetActive(false);
				lancer->SetLastPosition({ 0,0 });
				objList[LayerType::Object][0].push_back(lancer);
				//cout << obj.position.x <<","<<obj.position.y<< endl;

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
				archer->SetActive(false);
				archer->SetLastPosition({ 0,0 });
				objList[LayerType::Object][1].push_back(archer);
			}
			else if (obj.path == "graphics/Map/ArcherBosspng.png")
			{
				heavyBombingArcher = new HeavyBombingArcher();
				heavyBombingArcher->Init();
				((PlayUiMgr*)uiMgr)->SetHeavyBombingArcher(heavyBombingArcher);
				heavyBombingArcher->SetName(obj.type);
				heavyBombingArcher->SetPos(obj.position);
				heavyBombingArcher->SetCardPos(heavyBombingArcher->GetPos());
				heavyBombingArcher->SetObjType(Object::ObjTypes::Enemy);
				heavyBombingArcher->SetColor(2);
				heavyBombingArcher->SetActive(false);
				heavyBombingArcher->SetLastPosition({ 0,0 });
				objList[LayerType::Object][2].push_back(heavyBombingArcher);
			}
			else if (obj.path == "graphics/Map/FireBoss.png")
			{
				fireBoss = new FireBoss();
				fireBoss->Init();
				((PlayUiMgr*)uiMgr)->SetFireBoss(fireBoss);
				fireBoss->SetName(obj.type);
				fireBoss->SetPos(obj.position);
				fireBoss->SetObjType(Object::ObjTypes::Enemy);
				fireBoss->SetActive(false);
				fireBoss->SetLastPosition({ 0,0 });
				objList[LayerType::Object][3].push_back(fireBoss);
			}
			else if (obj.path == "graphics/Map/FinalBoss.png")
			{
				finalBoss = new FinalBoss();
				finalBoss->Init();
				((PlayUiMgr*)uiMgr)->SetFinalBoss(finalBoss);
				finalBoss->SetName(obj.type);
				finalBoss->SetPos(obj.position);
				finalBoss->SetObjType(Object::ObjTypes::FinalBoss);
				finalBoss->SetActive(false);
				finalBoss->SetLastPosition({ 0,0 });
				objList[LayerType::Object][0].push_back(finalBoss);
			}
		}
		else if (obj.type == "CLIFF")
		{
			Cliff* cliff = new Cliff();
			cliff->Init();
			cliff->SetName(obj.type);
			cliff->SetPos(obj.position);
			cliff->SetSize(obj.size);
			cliff->SetHitBox((FloatRect)cliff->GetCliffShape()->getGlobalBounds());
			cliff->SetHitBox({100, 100, 100, 100}, Color::Blue);
			cliff->SetObjType(Object::ObjTypes::Cliff);
			objList[LayerType::Object][3].push_back(cliff);
			collisionList[0][Object::ObjTypes::Cliff].push_back(cliff);
		}
	}

	for (int i = 0; i < room.size(); i++)
	{
		for (auto& a : objList)
		{
			for (auto& b : a.second)
			{
				for (auto& c : b.second)
				{
					if (Utils::OBB(c->GetHitBox(), room[i].GetHitBox()))
					{
						collisionList[i][c->GetObjType()].push_back(c);
					}
				}
			}
		}
		room[i].SetAliveEnemyCount(collisionList[i][Object::ObjTypes::Enemy].size() + collisionList[i][Object::ObjTypes::FinalBoss].size());
		//cout << i << "�� ���ʹ� �� : " << collisionList[i][Object::ObjTypes::Enemy].size() << endl;
	}

	auto& tiles = objList[LayerType::Tile][0];
	mapSize.left = 0;
	mapSize.top = 0;
	mapSize.width = (tiles.back())->GetPos().x + 16;
	mapSize.height = (tiles.back())->GetPos().y;

	for (int i = 0; i < room.size(); i++)
	{
		for (auto& enemy : collisionList[i][Object::ObjTypes::Enemy])
		{
			((Enemy*)enemy)->SetPlayer(player);
		}
		for (auto& finalBoss : collisionList[i][Object::ObjTypes::FinalBoss])
		{
			((FinalBoss*)finalBoss)->SetPlayer(player);
		}
	}
	fireBoss->SetPlayerLastPos(player->GetPos());
}

void PlayScene::Update(float dt)
{
	worldView.setCenter(player->GetPos());
	Scene::Update(dt);
	
	if (InputMgr::GetKeyDown(Keyboard::Key::Escape))
	{
		if (GetPause())
		{
			if (!((PlayUiMgr*)uiMgr)->IsOption())
				SetPause(false);
		}
		else
			SetPause(true);
	}

	//�÷��̾� �� ��ġ 
	for (int i = 0; i < room.size(); i++)
	{
		auto& collListPlayer = collisionList[i][Object::ObjTypes::Player];
		if (player->GetLowHitBounds().intersects(room[i].GetHitBounds()))
		{
			if (find(playerRooms.begin(), playerRooms.end(), i) == playerRooms.end())
			{
				collisionList[i][Object::ObjTypes::Player].push_back(player);
				playerRooms.push_back(i);
			}
			SpawnEnemy(i,dt);
			AllDieEnemy(i);
		}
		else
		{
			if (find(playerRooms.begin(), playerRooms.end(), i) != playerRooms.end())
			{
				collisionList[i][Object::ObjTypes::Player].remove(player);
				playerRooms.remove(i);
			}
		}
	}

	for (int i = 0; i < (int)Object::ObjTypes::Count; ++i)
	{
		switch ((Object::ObjTypes)i)
		{
		case Object::ObjTypes::Enemy:
		case Object::ObjTypes::FinalBoss:
		case Object::ObjTypes::Player:
			for (int j = 0; j < collisionList.size(); ++j)
			{
				if (collisionList[j][Object::ObjTypes::Player].empty())
					continue;
				for (auto& obj : collisionList[j][(Object::ObjTypes)i])
				{
					OnCollisionWall(j, obj);
					OnCollisionETC(j, obj);
				}
			}
			break;
		default:
			break;
		}
	}
	
	auto& usingProjectiles = projectiles->GetUseList();
	for (auto& projectile : usingProjectiles)
	{
		if (((Projectile*)projectile)->GetAtkShape() != Skill::AttackShape::Wave)
			continue;
		for (int i = 0; i < collisionList.size(); ++i)
		{
			if (collisionList[i][Object::ObjTypes::Player].empty())
				continue;
			for (auto& coll : collisionList[i][Object::ObjTypes::Wall])
			{
				if (projectile->GetHitBounds().intersects(coll->GetHitBounds()))
				{
					projectile->SetMoving(false);
					continue;
				}
			}
		}
	}
}

void PlayScene::Draw(RenderWindow& window)
{
	Vector2i min = { (int)(worldView.getCenter().x - (int)worldView.getSize().x * 0.5f), (int)(worldView.getCenter().y - (int)worldView.getSize().y * 0.5f) };
	Vector2i max = { (int)(worldView.getCenter().x + (int)worldView.getSize().x * 0.5f), (int)(worldView.getCenter().y + (int)worldView.getSize().y * 0.5f) };

	window.setView(worldView);
	for (auto& layer : objList)
	{
		for (auto& obj_pair : layer.second)
		{
			auto& objs = obj_pair.second;
			for (auto& obj : objs)
			{
				if (obj->GetPos().x<max.x + 80 && obj->GetPos().y < max.y + 80 && obj->GetPos().x > min.x - 80 && obj->GetPos().y > min.y - 80)
				{
					if (obj->GetActive())
					{
						obj->Draw(window);
					}
				}
			}
		}
	}

	if (uiMgr != nullptr && uiMgr->GetActive())
	{
		window.setView(uiView);
		uiMgr->Draw(window);
	}
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

void PlayScene::SpawnEnemy(int i, float dt)
{
	currSpownDelay -= dt;
	if (currSpownDelay <= 0)
	{
		for (auto& c_list : collisionList[i])
		{
			for (auto& obj : c_list.second)
			{
				if ((obj->GetObjType() == Object::ObjTypes::Enemy && ((Enemy*)obj)->GetIsAlive()) || 
					((obj->GetObjType() == Object::ObjTypes::FinalBoss) && ((FinalBoss*)obj)->GetState() != FinalBoss::States::Die))
				{
					obj->SetActive(true);
				}
			}
		}
		for (auto& c_list : collisionList[i])
		{
			for (auto& obj : c_list.second)
			{
				if (obj->GetObjType() == Object::ObjTypes::ETC)
				{
					if (obj->GetFileName() == "graphics/Map/Object/GateFull.png" ||
						obj->GetFileName() == "graphics/Map/Palette/LeftGate.png" ||
						obj->GetFileName() == "graphics/Map/Palette/RightGate.png")
					{
						obj->SetActive(true);
					}
				}
			}
		}
		currSpownDelay = maxSpownDelay;
	}
}

void PlayScene::AllDieEnemy(int i)
{
	for (auto& c_list : collisionList[i])
	{		
		for (auto& obj : c_list.second)
		{
			if (obj->GetObjType() == Object::ObjTypes::Enemy ||
				obj->GetObjType() == Object::ObjTypes::FinalBoss)
			{
				if (((Enemy*)obj)->GetIsAlive())
				{
					if (hpBarSet)
					{
						if (fireBoss->GetActive())
						{
							((PlayUiMgr*)uiMgr)->SetBossType(PlayUiMgr::BossType::FireBoss);
							((PlayUiMgr*)uiMgr)->SetBossMaxHp(fireBoss->GetMaxHp());
							((PlayUiMgr*)uiMgr)->SetBossName("Flame Queen");
							hpBarSet = false;
						}
						if (heavyBombingArcher->GetActive())
						{
							((PlayUiMgr*)uiMgr)->SetBossType(PlayUiMgr::BossType::Archer);
							((PlayUiMgr*)uiMgr)->SetBossMaxHp(heavyBombingArcher->GetMaxHp());
							((PlayUiMgr*)uiMgr)->SetBossName("Heavy Bombing Archer");
							hpBarSet = false;
						}
						if (finalBoss->GetActive())
						{
							((PlayUiMgr*)uiMgr)->SetBossType(PlayUiMgr::BossType::FinalBoss);
							((PlayUiMgr*)uiMgr)->SetBossMaxHp(finalBoss->GetMaxHp());
							((PlayUiMgr*)uiMgr)->SetBossName("Final Boss");
							hpBarSet = false;
						}
					}
					return;
				}
			}
			if (obj->GetObjType() == Object::ObjTypes::FinalBoss)
			{
				if (((FinalBoss*)obj)->GetState() != FinalBoss::States::Die)
					return;
			}
		}
		hpBarSet = true;
		room[i].SetAllEnemyDead(true);
	}
	if (room[i].GetAllEnemyDead())
	{
		for (auto& c_list : collisionList[i])
		{
			for (auto& obj : c_list.second)
			{
				if (obj->GetObjType() == Object::ObjTypes::ETC)
				{
					if (obj->GetFileName() == "graphics/Map/Object/GateFull.png" ||
						obj->GetFileName() == "graphics/Map/Palette/LeftGate.png" ||
						obj->GetFileName() == "graphics/Map/Palette/RightGate.png")
					{
						obj->SetActive(false);
					}
				}
			}
		}
	}
}

void PlayScene::OnCollisionWall(int roomVec, Object* obj)
{
	for (auto& coll : collisionList[roomVec][Object::ObjTypes::Wall])
	{
		if (obj->GetLowHitBounds().intersects(coll->GetHitBounds()))
		{
			bool leftandRight = false;
			bool topandLow = false;

			float objRightPoint = obj->GetLowHitBounds().width + obj->GetLowHitBounds().left;
			float objLowPoint = obj->GetLowHitBounds().height + obj->GetLowHitBounds().top;

			float collXPoint = (coll->GetHitBounds().width * 0.5f) + coll->GetHitBounds().left;
			float collYPoint = (coll->GetHitBounds().height * 0.5f) + coll->GetHitBounds().top;

			if (obj->GetLowHitBounds().height <= collYPoint || objLowPoint >= collYPoint)
				topandLow = true;
			if (obj->GetLowHitBounds().left >= collXPoint || objRightPoint <= collXPoint)
				leftandRight = true;

			if (topandLow)
			{
				obj->SetPos({ obj->GetPos().x, obj->GetLastPosition().y });
			}
			if (leftandRight)
			{
				obj->SetPos({ obj->GetLastPosition().x, obj->GetPos().y });
			}
		}
	}
}

void PlayScene::OnCollisionETC(int roomVec, Object* obj)
{
	for (auto& coll : collisionList[roomVec][Object::ObjTypes::ETC])
	{
		if(!coll->GetActive())
			continue;
		if (obj->GetLowHitBounds().intersects(coll->GetHitBounds()))
		{
			bool leftandRight = false;
			bool topandLow = false;

			float objRightPoint = obj->GetLowHitBounds().width + obj->GetLowHitBounds().left;
			float objLowPoint = obj->GetLowHitBounds().height + obj->GetLowHitBounds().top;

			float collXPoint = (coll->GetHitBounds().width * 0.5f) + coll->GetHitBounds().left;
			float collYPoint = (coll->GetHitBounds().height * 0.5f) + coll->GetHitBounds().top;

			if (obj->GetLowHitBounds().top >= collYPoint || objLowPoint <= collYPoint)
				topandLow = true;
			if (obj->GetLowHitBounds().left >= collXPoint || objRightPoint <= collXPoint)
				leftandRight = true;

			if (topandLow)
			{
				obj->SetPos({ obj->GetPos().x, obj->GetLastPosition().y });
			}
			if (leftandRight)
			{
				obj->SetPos({ obj->GetLastPosition().x, obj->GetPos().y });
			}
		}
	}
}
