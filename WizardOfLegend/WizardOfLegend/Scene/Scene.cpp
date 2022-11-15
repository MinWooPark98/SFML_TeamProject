#include "Scene.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/Framework.h"
#include "../Ui/MaptoolUiMgr.h"
#include "../Framework/InputMgr.h"
#include "../Scene/SceneMgr.h"
#include "../Framework/SoundMgr.h"
#include "../GameObject/Projectile.h"

Scene::Scene(Scenes type)
	: type(type), uiMgr(nullptr), isPause(false)
{
}

Scene::~Scene()
{
	Release();
	
}
void Scene::Init()
{
	projectiles = new ObjectPool<Projectile>();
	projectiles->Init(20);
}
void Scene::Release()
{
	if (uiMgr != nullptr)
	{
		((MapToolUiMgr*)uiMgr)->Release();
		uiMgr = nullptr;
	}
	uiMgr = nullptr;
	for (auto& layer : objList)
	{
		for (auto& obj_pair : layer.second)
		{
			auto objs = obj_pair.second;

			for (auto& obj : objs)
			{
				if (obj != nullptr)
					delete obj;
			}
			objs.clear();
		}
		layer.second.clear();
	}
	objList.clear();
}

void Scene::Exit()
{
	SOUND_MGR->StopAll();
}

void Scene::Update(float dt)
{
	Vector2f mousePos = InputMgr::GetMousePos();
	Vector2f windowSize = (Vector2f)FRAMEWORK->GetWindowSize();
	objMousePos = ScreenToWorld((Vector2i)mousePos);
	if (projectiles != nullptr)
		projectiles->Update(dt);
	for (auto& layer : objList)
	{
		for (auto& obj_pair : layer.second)
		{
			auto objs = obj_pair.second;

			for (auto& obj : objs)
			{
				if (obj->GetActive())
				{
					obj->Update(dt);
				}
			}
		}
	}
	if (InputMgr::GetKeyDown(Keyboard::Escape))
		exit(1);
}

void Scene::Draw(RenderWindow& window)
{
	window.setView(worldView);

	if (!isMap)
	{
		for (auto& layer : objList)
		{
			for (auto& obj_pair : layer.second)
			{
				auto objs = obj_pair.second;
				for (auto& obj : objs)
				{
					if (obj->GetActive())
					{
						obj->Draw(window);
					}
				}
			}
		}
		if (uiMgr != nullptr)
			uiMgr->Draw(window);
	}
	else
	{
		//LayerSort();
		int i = 0;
		for (auto& obj : objList[LayerType::Tile])
		{
			for (auto& o : obj.second)
			{
				o->Draw(window);
			}
		}
		//for (auto& obj : drawObjs)
		//{
		//	obj->Draw(window);
		//}

		if (uiMgr != nullptr)
			uiMgr->Draw(window);
	}
}

Texture* Scene::GetTexture(const string path) const
{
	return RESOURCE_MGR->GetTexture(path);
}

Vector2f Scene::ScreenToWorld(Vector2i screenPos)
{
	RenderWindow& window = FRAMEWORK->GetWindow();
	return window.mapPixelToCoords(screenPos, worldView);
}

Vector2f Scene::ScreenToUiPosition(Vector2i screenPos)
{
	RenderWindow& window = FRAMEWORK->GetWindow();
	return window.mapPixelToCoords(screenPos,uiView);
}

void Scene::AddGameObject(Object* obj, LayerType type, int num)
{
	objList[type][num].push_back(obj);
}
Object* Scene::FindGameObj(string name)
{
	for (auto& layer : objList)
	{
		for (auto& obj_pair : layer.second)
		{
			auto objs = obj_pair.second;
			for (auto& obj : objs)
			{
				if (obj->GetName() == name)
				{
					return obj;
				}
			}
		}
	}
}


