#include "PlayScene.h"
#include "../GameObject/Lancer.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../GameObject/Skill.h"
#include "../GameObject/Projectile.h"
#include "../GameObject/Archer.h"
#include "../GameObject/HeavyBombingArcher.h"
#include "../GameObject/FireBoss.h"
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
	auto& skills = player->GetSkills();
	skills[0]->SetSkill("FireBall");
	skills[4]->SetSkill("DragonArc");
	skills[5]->SetSkill("FireFull");
	objList[LayerType::Object][0].push_back(player);

	Lancer* lancer = new Lancer();
	lancer->Init();
	objList[LayerType::Object][0].push_back(lancer);

	Archer* archer = new Archer();
	archer->Init();
	archer->SetColor(3);
	objList[LayerType::Object][0].push_back(archer);
	
	HeavyBombingArcher* heavyBombingArcher = new HeavyBombingArcher();
	heavyBombingArcher->Init();
	heavyBombingArcher->SetColor(2);
	objList[LayerType::Object][0].push_back(heavyBombingArcher);
	
	FireBoss* fireBoss = new FireBoss();
	fireBoss->Init();
	objList[LayerType::Object][0].push_back(fireBoss);
	
	lancer->SetPlayer(player);
	archer->SetPlayer(player);
	heavyBombingArcher->SetPlayer(player);
	fireBoss->SetPlayer(player);
}

void PlayScene::Update(float dt)
{
	Scene::Update(dt);
	if (InputMgr::GetKeyDown(Keyboard::Escape))
	{
		SCENE_MGR->ChangeScene(Scenes::Title);
		return;
	}
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
