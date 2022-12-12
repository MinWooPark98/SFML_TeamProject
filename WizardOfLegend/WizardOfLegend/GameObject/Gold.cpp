#include "Gold.h"
#include "../Framework/Utils.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMgr.h"
#include "../Scene/PlayScene.h"
#include "../GameObject/Player.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/SoundMgr.h"

void Gold::Init()
{
	SpriteObj::Init();
	SetTexture(*RESOURCE_MGR->GetTexture("graphics/Coin_0.png"));
	SetHitBox({ 20.f, 20.f, 5.f, 5.f }, Color::Red);
	hitbox.setOrigin(GetHitBox().getSize().x * 0.5f, GetHitBox().getSize().y * 0.5f);
	goldAnimation.SetTarget(&sprite);
	goldAnimation.AddClip(*RESOURCE_MGR->GetAnimationClip("Gold"));
}

void Gold::Update(float dt)
{
	SpriteObj::Update(dt);
	goldAnimation.Update(dt);
	Drop(dt);
}

void Gold::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
}

void Gold::Reset()
{
	SpriteObj::Reset();
	animationPlay = true;
}

void Gold::Drop(float dt)
{
	Scene* currScene = SCENE_MGR->GetCurrentScene();
	player = (Player*)currScene->FindGameObj("PLAYER");
	if (player != nullptr)
	{
		if (player->GetHitBounds().intersects(GetHitBounds()) && GetActive())
		{
			SOUND_MGR->Play("sounds/DropItem.wav");
			GetGold(player);
			animationPlay = true;
			SetActive(false);
		}
		else
		{
			auto move = Utils::Normalize(player->GetPos() - GetPos());
			Translate(dt * moveSpeed * move);

			if (animationPlay)
			{
				goldAnimation.Play("Gold");
				animationPlay = false;
			}
		}
	}
}

void Gold::GetGold(Player* player)
{
	player->AddGold(gold);
}

void Gold::SetGoldPos(Vector2f pos)
{
	SetPos(pos + Utils::RandAreaPoint() * 25.f);
}
