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
			ObjTypeList[Object::ObjTypes::Wall].push_back(draw);
		}
		if (obj.type == "TILE")
		{
		SpriteObj* draw = new SpriteObj();
		draw->SetName(obj.type);
		draw->SetTexture(*RESOURCE_MGR->GetTexture(obj.path));
		draw->SetOrigin(Origins::BC);
		draw->SetPos(obj.position);
		draw->SetObjType(Object::ObjTypes::Tile);
		ObjTypeList[Object::ObjTypes::Tile].push_back(draw);

		objList[LayerType::Tile][0].push_back(draw);
		}
		if (obj.type == "OBJECT")
		{
			SpriteObj* draw = new SpriteObj();
			draw->SetName(obj.type);
			draw->SetTexture(*RESOURCE_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);
			draw->SetHitBox(obj.path);
			draw->SetObjType(Object::ObjTypes::ETC);

			objList[LayerType::Object][0].push_back(draw);
			ObjTypeList[Object::ObjTypes::ETC].push_back(draw);
		}
		if (obj.type == "PLAYER")
		{
			player = new Player();
			player->Init();
			player->SetName(obj.type);
			player->SetPos(obj.position);
			player->SetObjType(Object::ObjTypes::Player);
			objList[LayerType::Object][5].push_back(player);
			ObjTypeList[Object::ObjTypes::Player].push_back(player);
			auto& skills = player->GetSkills();
			skills[0]->SetSkill("FireBall");
			skills[1]->SetSkill("Dragon");
			skills[2]->SetSkill("FireDash");
			skills[4]->SetSkill("DragonArc");
			skills[5]->SetSkill("FireFull");
		}
		if (obj.type == "ENEMY")
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
				ObjTypeList[Object::ObjTypes::Enemy].push_back(lancer);
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
				ObjTypeList[Object::ObjTypes::Enemy].push_back(archer);
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
				ObjTypeList[Object::ObjTypes::Enemy].push_back(heavyBombingArcher);
			}
			else if (obj.path == "graphics/Map/FireBoss.png")
			{
				fireBoss = new FireBoss();
				fireBoss->Init();
				fireBoss->SetName(obj.type);
				fireBoss->SetPos(obj.position);
				fireBoss->SetObjType(Object::ObjTypes::Enemy);

				fireBoss->SetPlayerLastPos(player->GetPos());
				objList[LayerType::Object][3].push_back(fireBoss);
				ObjTypeList[Object::ObjTypes::Enemy].push_back(fireBoss);
			}
		}
	}
	auto& tiles = objList[LayerType::Tile][0];
	mapSize.left = 0;
	mapSize.top = 0;
	mapSize.width = (tiles.back())->GetPos().x + 16;
	mapSize.height = (tiles.back())->GetPos().y;

	for (auto& enemy : ObjTypeList[Object::ObjTypes::Enemy])
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
	if (InputMgr::GetKeyDown(Keyboard::Escape))
	{
		SCENE_MGR->ChangeScene(Scenes::Title);
		return;
	}


}

void PlayScene::Draw(RenderWindow& window)
{
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

	uiView.setSize(size * 0.25f);
	uiView.setCenter(size * 0.125f);
}

void PlayScene::Exit()
{
	Scene::Exit();
}
