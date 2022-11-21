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


	player = new Player();
	player->Init();
	objList[LayerType::Object][5].push_back(player);
	auto& skills = player->GetSkills();
	skills[0]->SetSkill("FireBall");
	skills[4]->SetSkill("DragonArc");
	skills[5]->SetSkill("FireFull");

	isMap = true;
	auto& data = FILE_MGR->GetMap("TUTORIAL");

	for (auto& obj : data)
	{
		if (obj.type == "TILE")
		{
		SpriteObj* draw = new SpriteObj();
		draw->SetName(obj.type);
		draw->SetTexture(*RESOURCE_MGR->GetTexture(obj.path));
		draw->SetOrigin(Origins::BC);
		draw->SetPos(obj.position);
		//draw->SetHitBox(obj.path);

		int i = ((int)obj.position.x - 30) / 60;
		int j = (int)obj.position.y / 60 - 1;
		objList[LayerType::Tile][0].push_back(draw);
		}
	}
	auto& tiles = objList[LayerType::Tile][0];
	mapSize.left = 0;
	mapSize.top = 0;
	mapSize.width = (tiles.back())->GetPos().x + 30;
	mapSize.height = (tiles.back())->GetPos().y;
}

void PlayScene::Update(float dt)
{
	if (InputMgr::GetKeyDown(Keyboard::Key::Num1))
	{
		Lancer* lancer = new Lancer();
		lancer->Init();
		objList[LayerType::Object][0].push_back(lancer);
		lancer->SetPlayer(player);
	}
	if (InputMgr::GetKeyDown(Keyboard::Key::Num2))
	{
		Archer* archer = new Archer();
		archer->Init();
		archer->SetColor(3);
		objList[LayerType::Object][1].push_back(archer);
		archer->SetPlayer(player);
	}
	if (InputMgr::GetKeyDown(Keyboard::Key::Num3))
	{
		HeavyBombingArcher* heavyBombingArcher = new HeavyBombingArcher();
		heavyBombingArcher->Init();
		heavyBombingArcher->SetColor(2);
		objList[LayerType::Object][2].push_back(heavyBombingArcher);
		heavyBombingArcher->SetPlayer(player);
	}
	if (InputMgr::GetKeyDown(Keyboard::Key::Num4))
	{
		fireBoss = new FireBoss();
		fireBoss->Init();
		fireBoss->SetPlayerLastPos(player->GetPos());
		objList[LayerType::Object][3].push_back(fireBoss);
		fireBoss->SetPlayer(player);
	}

	if (InputMgr::GetKeyDown(Keyboard::Key::K))
	{
		for (auto it : objList[LayerType::Object][0])
		{
			if (!objList[LayerType::Object][0].empty())
				delete it;
		}
		objList[LayerType::Object][0].clear();
	}
	if (InputMgr::GetKeyDown(Keyboard::Key::L))
	{
		for (auto it : objList[LayerType::Object][1])
		{
			if (!objList[LayerType::Object][1].empty())
				delete it;
		}
		objList[LayerType::Object][1].clear();
	}
	if (InputMgr::GetKeyDown(Keyboard::Key::J))
	{
		for (auto it : objList[LayerType::Object][2])
		{
			if (!objList[LayerType::Object][2].empty())
				delete it;
		}
		objList[LayerType::Object][2].clear();
	}
	if (InputMgr::GetKeyDown(Keyboard::Key::H))
	{
		for (auto it : objList[LayerType::Object][3])
		{
			if (!objList[LayerType::Object][3].empty())
				delete it;
		}
		objList[LayerType::Object][3].clear();
	}
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
	worldView.setSize(size * 0.5f);
	worldView.setCenter(size * 0.25f);

	uiView.setSize(size * 0.5f);
	uiView.setCenter(size * 0.25f);
}

void PlayScene::Exit()
{
	Scene::Exit();
}
