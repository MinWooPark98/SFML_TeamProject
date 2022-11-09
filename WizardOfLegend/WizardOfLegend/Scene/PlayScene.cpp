#include "PlayScene.h"
#include "../GameObject/Lancer.h"

PlayScene::PlayScene()
	:Scene(Scenes::Play)
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Init()
{
	lancer = new Lancer();
	lancer->Init();
	objList.push_back(lancer);
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
