#include "SceneMgr.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "MapToolScene.h"
#include "SkillToolScene.h"

bool SceneMgr::Init()
{
    sceneMap[Scenes::Title] = new TitleScene();
    sceneMap[Scenes::MapTool] = new MapToolScene();
    sceneMap[Scenes::SkillTool] = new SkillToolScene();
    sceneMap[Scenes::Play] = new PlayScene();
    currScene = Scenes::Title;

    for (auto& pair : sceneMap)
    {
        pair.second->Init();
    }

    sceneMap[currScene]->Enter();

    return true;
}

Scene* SceneMgr::GetScene(Scenes scene)
{
    return sceneMap[scene];
}

void SceneMgr::ChangeScene(Scenes scene)
{
    sceneMap[currScene]->Exit();
    currScene = scene;
    sceneMap[currScene]->Enter();
}

Scene* SceneMgr::GetCurrentScene()
{
    return sceneMap[currScene];
}

void SceneMgr::Update(float dt)
{
    sceneMap[currScene]->Update(dt);
}

void SceneMgr::Draw(RenderWindow& window)
{
    sceneMap[currScene]->Draw(window);
}

