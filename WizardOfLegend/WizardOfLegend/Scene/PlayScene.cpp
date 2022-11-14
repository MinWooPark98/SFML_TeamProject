#include "PlayScene.h"
#include "../GameObject/Lancer.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../GameObject/Projectile.h"
#include "../GameObject/Archer.h"
#include "../GameObject/HeavyBombingArcher.h"

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

	Player* player = new Player();
	player->Init();
	objList.push_back(player);

	Lancer* lancer = new Lancer();
	lancer->Init();
	objList.push_back(lancer);

	Archer* archer = new Archer();
	archer->Init();
	archer->SetColor(3);
	objList.push_back(archer);

	HeavyBombingArcher* heavyBombingArcher = new HeavyBombingArcher();
	heavyBombingArcher->Init();
	heavyBombingArcher->SetColor(2);
	objList.push_back(heavyBombingArcher);

	lancer->SetPlayer(player);
	archer->SetPlayer(player);
	heavyBombingArcher->SetPlayer(player);
}

void PlayScene::Update(float dt)
{
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
