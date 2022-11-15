#include "SkillToolScene.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"

SkillToolScene::SkillToolScene()
	:Scene(Scenes::SkillTool)
{
}

SkillToolScene::~SkillToolScene()
{
}

void SkillToolScene::Init()
{
	Scene::Init();

	Player* player = new Player();
	player->Init();
	objList.push_back(player);
}

void SkillToolScene::Release()
{
	Scene::Release();
}

void SkillToolScene::Reset()
{
	Scene::Reset();
}

void SkillToolScene::Enter()
{
	Scene::Enter();
	Vector2f size = (Vector2f)FRAMEWORK->GetWindowSize();

	worldView.setSize(size * 0.5f);
	worldView.setCenter(size * 0.25f);
	worldView.setViewport(FloatRect(0.f, 0.f, 0.8f, 1.f));

	uiView.setSize(size * 0.5f);
	uiView.setCenter(size * 0.25f);
}

void SkillToolScene::Exit()
{
	Scene::Exit();
}
