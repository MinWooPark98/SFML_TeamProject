#include "SkillToolScene.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../Ui/SkillToolUiMgr.h"

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
	player->SetSkillToolMode();
	player->SetName("player");
	objList[LayerType::Object][0].push_back(player);

	uiMgr = new SkillToolUiMgr();
	uiMgr->Init();
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

	worldView.setSize(size.x * 0.35f, size.y * 0.5f);
	worldView.setCenter(size * 0.25f);
	worldView.setViewport(FloatRect(0.f, 0.f, 0.7f, 1.f));

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);
}

void SkillToolScene::Exit()
{
	Scene::Exit();
}
