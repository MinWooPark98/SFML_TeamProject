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

	player = new Player();
	player->Init();
	objList[LayerType::Object][0].push_back(player);
}

void PlayScene::Update(float dt)
{
	if (InputMgr::GetKeyDown(Keyboard::Space))
	{
		SCENE_MGR->ChangeScene(Scenes::MapTool);
	}

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
		objList[LayerType::Object][0].push_back(archer);
		archer->SetPlayer(player);
	}
	if (InputMgr::GetKeyDown(Keyboard::Key::Num3))
	{
		HeavyBombingArcher* heavyBombingArcher = new HeavyBombingArcher();
		heavyBombingArcher->Init();
		heavyBombingArcher->SetColor(2);
		objList[LayerType::Object][0].push_back(heavyBombingArcher);
		heavyBombingArcher->SetPlayer(player);
	}
	if (InputMgr::GetKeyDown(Keyboard::Key::Num4))
	{
		fireBoss = new FireBoss();
		fireBoss->Init();
		fireBoss->SetPlayerLastPos(player->GetPos());
		objList[LayerType::Object][0].push_back(fireBoss);
		fireBoss->SetPlayer(player);
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
