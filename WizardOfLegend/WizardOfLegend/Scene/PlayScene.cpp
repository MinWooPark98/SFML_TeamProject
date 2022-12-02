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
				lancer->SetActive(false);
				objList[LayerType::Object][0].push_back(lancer);
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
				objList[LayerType::Object][1].push_back(archer);
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
				heavyBombingArcher->SetActive(false);
				objList[LayerType::Object][2].push_back(heavyBombingArcher);
			}
			else if (obj.path == "graphics/Map/FireBoss.png")
			{
				fireBoss = new FireBoss();
				fireBoss->Init();
				fireBoss->SetName(obj.type);
				fireBoss->SetPos(obj.position);
				fireBoss->SetObjType(Object::ObjTypes::Enemy);
				fireBoss->SetActive(false);
				objList[LayerType::Object][3].push_back(fireBoss);
			}
		}
		else if (obj.type == "CLIFF")
		{
			Cliff* cliff = new Cliff();
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

	for (int i = 0; i < room.size(); i++)
	{
		for (auto& enemy : collisionList[i][Object::ObjTypes::Enemy])
		{
			((Enemy*)enemy)->SetPlayer(player);
		}
	}
	fireBoss->SetPlayerLastPos(player->GetPos());

	uiMgr = new PlayUiMgr();
	uiMgr->Init();

	((PlayUiMgr*)uiMgr)->SetBossCurHp(fireBoss->GetCurHp());
	((PlayUiMgr*)uiMgr)->SetBossMaxHp(fireBoss->GetMaxHp());

	((PlayUiMgr*)uiMgr)->SetPlayerCurHp(player->GetMaxHp());
	((PlayUiMgr*)uiMgr)->SetPlayerMaxHp(player->GetMaxHp());
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

	//플레이어 방 위치 
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
					//OnCollisionETC(j, obj);
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

	if (fireBoss->GetIsAlive())
	{
		((PlayUiMgr*)uiMgr)->SetBossName("FLAME QUEEN");
		if (fireBoss->GetFireKick()->GetActive())
		{
			if (fireBoss->GetIsKick())
			{
				if (Utils::OBB(player->GetHitBox(), fireBoss->GetFireBossKickHitBox()))
				{
					((PlayUiMgr*)uiMgr)->SetMonsterDamage(fireBoss->GetDamage());
					fireBoss->SetIsKick(false);
				}
			}
			else
				((PlayUiMgr*)uiMgr)->SetMonsterDamage(0);
		}

		if (((PlayUiMgr*)uiMgr)->GetBossCurHp() <= 0)
		{
			fireBoss->SetCurHp(0);
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

void PlayScene::SpawnEnemy(int i)
{
	for (auto& c_list : collisionList[i])
	{
		for (auto& obj : c_list.second)
		{
			if (obj->GetObjType() == Object::ObjTypes::Enemy)
			{
				obj->SetActive(true);
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
