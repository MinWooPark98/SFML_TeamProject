#include "PlayScene.h"
#include "../GameObject/Lancer.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
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
	Player* player = new Player();
	player->Init();
	objList[LayerType::Object][0].push_back(player);

	Lancer* lancer = new Lancer();
	lancer->Init();
	objList[LayerType::Object][0].push_back(lancer);

	Archer* archer = new Archer();
	archer->Init();
	archer->SetColor(3);
	objList[LayerType::Object][0].push_back(archer);
	
	HeavyBombingArcher* heavyBombingArcher = new HeavyBombingArcher();
	heavyBombingArcher->Init();
	heavyBombingArcher->SetColor(2);
	objList[LayerType::Object][0].push_back(heavyBombingArcher);
	
	FireBoss* fireBoss = new FireBoss();
	fireBoss->Init();
	objList[LayerType::Object][0].push_back(fireBoss);
	
	lancer->SetPlayer(player);
	archer->SetPlayer(player);
	heavyBombingArcher->SetPlayer(player);
	fireBoss->SetPlayer(player);

	auto& tiles = objList[LayerType::Tile][0];
	mapSize.left = 0;
	mapSize.top = 0;
	mapSize.width = (tiles.back())->GetPos().x + 30;
	mapSize.height = (tiles.back())->GetPos().y;
}

void PlayScene::Update(float dt)
{
	if (InputMgr::GetKeyDown(Keyboard::Space))
	{
		SCENE_MGR->ChangeScene(Scenes::MapTool);
	}
	Scene::Update(dt);
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
