#include "DevScene.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../Ui/DevUiMgr.h"
#include "../Framework/InputMgr.h"
#include "SceneMgr.h"
#include "../GameObject/Skill.h"
#include "../GameObject/Interactive/SkillBook.h"
#include "../Ui/SkillBookUi.h"

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
	auto& windowSize = FRAMEWORK->GetWindowSize();

	Player* player = new Player();
	player->Init();
	player->Translate({ -240.f, 90.f });
	player->SetName("PLAYER");
	player->SetSkillSet(0, "FireBall");
	player->SetSkillSet(1, "FireDash");
	player->SetSkillSet(4, "DragonArc");
	player->SetSkillSet(5, "JumpMeteor");
	objList[LayerType::Object][0].push_back(player);

	SkillBook* book = new SkillBook();
	book->Init();
	book->SetPos((Vector2f)windowSize * 0.125f);
	book->SetPlayer(player);
	objList[LayerType::Object][0].push_back(book);

	uiMgr = new DevUiMgr();
	uiMgr->Init();

	book->Interact = bind(&SkillBookUi::SetActive, (SkillBookUi*)uiMgr->FindUiObj("SKILLBOOKUI"), true);
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
