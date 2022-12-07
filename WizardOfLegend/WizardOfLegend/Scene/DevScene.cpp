#include "DevScene.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../Ui/DevUiMgr.h"
#include "../Framework/InputMgr.h"
#include "SceneMgr.h"
#include "../GameObject/Skill.h"

DevScene::DevScene()
	:Scene(Scenes::Dev)
{
}

DevScene::~DevScene()
{
}

void DevScene::Init()
{
	Scene::Init();

	Player* player = new Player();
	player->Init();
	player->SetSkillToolMode();
	player->SetName("PLAYER");
	objList[LayerType::Object][0].push_back(player);

	uiMgr = new DevUiMgr();
	uiMgr->Init();
}

void DevScene::Release()
{
	Scene::Release();
}

void DevScene::Reset()
{
	Scene::Reset();
}

void DevScene::Update(float dt)
{
	Scene::Update(dt);
	uiMgr->Update(dt);
}

void DevScene::Draw(RenderWindow& window)
{
	Scene::Draw(window);
}

void DevScene::Enter()
{
	Scene::Enter();
	Vector2f size = (Vector2f)FRAMEWORK->GetWindowSize();
	worldView.setSize(size * 0.25f);
	worldView.setCenter(size * 0.125f);

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);
}

void DevScene::Exit()
{
	Scene::Exit();
}
