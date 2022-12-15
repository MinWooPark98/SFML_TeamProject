#include "Portal.h"
#include "../Scene/PlayScene.h"
#include "../Scene/SceneMgr.h"
#include "../GameObject/Player.h"
#include "../Framework/ResourceMgr.h"

Portal::Portal()
{
}

Portal::~Portal()
{
}

void Portal::Init()
{
	SetTexture(*RESOURCE_MGR->GetTexture("graphics/Map/Object/Portal.png"));
	SetOrigin(Origins::MC);
	SetHitBox({20, 20, 10, 20}, Color::Red);
	SetHitBoxOrigin(Origins::MC);
	SetActive(false);
	currScene = SCENE_MGR->GetCurrentScene();
}

void Portal::Update(float dt)
{
	SpriteObj::Update(dt);

	Player* player = (Player*)currScene->FindGameObj("PLAYER");
	if (isPortalCreat)
	{
		if (player != nullptr)
		{
			SetActive(true);

			if (player->GetHitBounds().intersects(GetHitBounds()))
			{
				if (((PlayScene*)currScene)->GetMapName()=="TUTORIALFIGHT")
				{
					player->SetPlatinum(0);
					player->AddPlatinum(22);
				}
				ChangeMap();
			}				
		}
	}
	else
	{
		if (player != nullptr)
			SetActive(false);
	}
}

void Portal::Draw(RenderWindow& window)
{
	if (GetActive())
		SpriteObj::Draw(window);
}

void Portal::ChanegePlayerPos(const Vector2f& pos)
{

}

void Portal::SetChanegeMap(string mapName)
{
	currScene = SCENE_MGR->GetCurrentScene();
	((PlayScene*)currScene)->SetMapName(mapName);
}

void Portal::ChangeMap()
{
	currScene = SCENE_MGR->GetCurrentScene();
	((PlayScene*)currScene)->Exit();
	((PlayScene*)currScene)->Enter();
}