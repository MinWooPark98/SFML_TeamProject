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
	Vector2i size = FRAMEWORK->GetWindowSize();

	worldView.setSize(size.x, size.y);
	worldView.setCenter(size.x * 0.25f, size.y * 0.5f);
	worldView.setViewport(FloatRect(0.f, 0.f, 0.5f, 1.f));

	uiView.setSize(size.x, size.y);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);
}

void SkillToolScene::Exit()
{
	Scene::Exit();
}
