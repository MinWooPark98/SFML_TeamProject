#include "TitleScene.h"
#include "../GameObject/SpriteObj.h"
#include "../Framework/Framework.h"
#include "../Ui/TitleUiMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/SoundMgr.h"
#include "SceneMgr.h"

bool TitleScene::titleBgm = true;

TitleScene::TitleScene()
	:Scene(Scenes::Title)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	Scene::Init();
	SpriteObj* background = new SpriteObj();
	background->Init();
	background->SetTexture(*GetTexture("graphics/background1.png"));
	objList[LayerType::Back][0].push_back(background);

	uiMgr = new TitleUiMgr();
	uiMgr->Init();
}

void TitleScene::Release()
{
	Scene::Release();
}

void TitleScene::Reset()
{
	Scene::Reset();
}

void TitleScene::Update(float dt)
{
	Scene::Update(dt);

	if (titleBgm)
	{
		SOUND_MGR->Play("sounds/TitleScreen_Piano.wav", 1);
		titleBgm = false;
	}
}

void TitleScene::Draw(RenderWindow& window)
{
	Scene::Draw(window);
}

void TitleScene::Enter()
{
	Scene::Enter();
	Vector2f size = (Vector2f)FRAMEWORK->GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter(size * 0.5f);
	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);
	titleBgm = true;
}

void TitleScene::Exit()
{
	Scene::Exit();
}
