#include "PlayScene.h"
#include "../GameObject/Player.h"

PlayScene::PlayScene()
	:Scene(Scenes::Play)
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Init()
{
	Player* player = new Player();
	player->Init();
	objList.push_back(player);
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
}

void PlayScene::Exit()
{
	Scene::Exit();
}
