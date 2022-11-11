#include "PlayScene.h"
#include "../GameObject/Lancer.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../GameObject/Projectile.h"
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

	Player* player = new Player();
	player->Init();
	objList.push_back(player);

	lancer = new Lancer();
	lancer->Init();
	objList.push_back(lancer);

	lancer->SetPlayer(player);
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
