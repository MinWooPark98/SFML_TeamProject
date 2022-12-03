#include "SkillToolScene.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../Ui/SkillToolUiMgr.h"
#include "../Framework/InputMgr.h"
#include "SceneMgr.h"
#include "../GameObject/Skill.h"

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
	player->SetName("PLAYER");
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
	for (auto& layer : objList)
	{
		for (auto& obj_pair : layer.second)
		{
			auto& objs = obj_pair.second;
			for (auto& obj : objs)
			{
				if (obj->GetActive())
				{
					obj->Reset();
				}
			}
		}
	}
	if (uiMgr != nullptr && uiMgr->GetActive())
		uiMgr->Reset();
}

void SkillToolScene::Update(float dt)
{
	Scene::Update(dt);
	if (InputMgr::GetKeyDown(Keyboard::Escape))
	{
		SCENE_MGR->ChangeScene(Scenes::Title);
		return;
	}
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

	Reset();
}

void SkillToolScene::Exit()
{
	Scene::Exit();
}
