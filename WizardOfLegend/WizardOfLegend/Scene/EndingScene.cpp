#include "EndingScene.h"
#include "../GameObject/SpriteObj.h"
#include "../Framework/Framework.h"
#include "../Framework/InputMgr.h"
#include "SceneMgr.h"
#include <SFML/Graphics.hpp>
#include "../Framework/SoundMgr.h"

bool EndingScene::endingBgm = true;

EndingScene::EndingScene()
	:Scene(Scenes::Ending)
{
}

EndingScene::~EndingScene()
{
}

void EndingScene::Init()
{
	Scene::Init();
	SpriteObj* background = new SpriteObj();
	background->Init();
	background->SetTexture(*GetTexture("graphics/Map/ending/Background10.png"));
	objList[LayerType::Back][0].push_back(background);

	thankYou = new SpriteObj();
	thankYou->Init();
	thankYou->SetTexture(*GetTexture("graphics/Map/ending/the End Thanks For Playing.png"));
	thankYou->SetColor({ 255,255,255,0 });
	objList[LayerType::Back][0].push_back(thankYou);

}

void EndingScene::Release()
{
	Scene::Release();
}

void EndingScene::Reset()
{
	Scene::Reset();
}

void EndingScene::Update(float dt)
{
	Scene::Update(dt);

	if (endingBgm)
	{
		SOUND_MGR->SetVolume(20.f);
		SOUND_MGR->Play("sounds/Hub.wav", 1);
		SOUND_MGR->SetVolume(100.f);
		endingBgm = false;
	}
	FadeIn(thankYou,dt);
	if (InputMgr::GetKeyDown(Keyboard::Escape))
	{
		SCENE_MGR->ChangeScene(Scenes::Title);
		return;
	}
}

void EndingScene::Draw(RenderWindow& window)
{
	Scene::Draw(window);
}

void EndingScene::Enter()
{
	Scene::Enter();
	Vector2f size = (Vector2f)FRAMEWORK->GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter(size * 0.5f);
	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);
	endingBgm = true;
}

void EndingScene::Exit()
{
	Scene::Exit();
}

void EndingScene::FadeIn(SpriteObj* obj, float dt)
{
	if (obj->GetSprite().getColor().a == 255)
		return;
	if (currentAlpha == endAlpha)
		return;
	currentTime += dt;
	if (currentTime >= targetTime)
	{
		currentTime == 0;
	}
	else
	{
		currentAlpha = startAlpha + (endAlpha - startAlpha) * currentTime / targetTime;
	}
	
	Color fadeColor = obj->GetSprite().getColor();
	fadeColor.a = currentAlpha;
	obj->SetColor(fadeColor);
}
