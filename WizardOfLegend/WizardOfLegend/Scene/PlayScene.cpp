#include "PlayScene.h"
#include "../GameObject/Lancer.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../GameObject/Projectile.h"

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

	lancer = new Lancer();
	lancer->Init();
	objList.push_back(lancer);

	Player* player = new Player();
	player->Init();
	objList.push_back(player);
}

void PlayScene::Update(float dt)
{
	for (auto obj : objList)
	{
		obj->Update(dt);
	}
}

void PlayScene::Draw(RenderWindow& window)
{
	for (auto obj : objList)
	{
		obj->Draw(window);
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
	worldView.setSize(size * 0.5f);
	worldView.setCenter(size * 0.25f);
	uiView.setSize(size * 0.5f);
	uiView.setCenter(size * 0.25f);
}

void PlayScene::Exit()
{
	Scene::Exit();
}
