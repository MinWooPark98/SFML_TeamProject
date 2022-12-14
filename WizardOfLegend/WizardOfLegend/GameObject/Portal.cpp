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
	if (player != nullptr && player->GetState() == Player::States::Die)
	{
		ChangeMap();
	}
	if (isPortalCreat)
	{
		SetActive(true);
		if (player != nullptr)
		{
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
		SetActive(false);
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

void Portal::SetPlayer(Player* player)
{
	this->player = player;
}