#include "ChaosFragments.h"
#include "../Framework/Utils.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMgr.h"
#include "../Scene/PlayScene.h"
#include "../GameObject/Player.h"
#include "../Framework/ResourceMgr.h"

void ChaosFragments::Init()
{
	SpriteObj::Init();
	SetTexture(*RESOURCE_MGR->GetTexture("graphics/Platinum.png"));
	SetHitBox({ 20.f, 20.f, 5.f, 5.f }, Color::Red);
	hitbox.setOrigin(GetHitBox().getSize().x * 0.5f, GetHitBox().getSize().y * 0.5f);
	animation.SetTarget(&sprite);
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("Platinum"));
}

void ChaosFragments::Update(float dt)
{
	SpriteObj::Update(dt);
	animation.Update(dt);
	Drop(dt);
}

void ChaosFragments::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
}

void ChaosFragments::Reset()
{
	SpriteObj::Reset();
	animationPlay = true;
}

void ChaosFragments::Drop(float dt)
{
	Scene* currScene = SCENE_MGR->GetCurrentScene();
	player = (Player*)currScene->FindGameObj("PLAYER");

	if (player->GetHitBounds().intersects(GetHitBounds()) && GetActive())
	{
		GetChaos(player);
		animationPlay = true;
		SetActive(false);
	}
	else
	{
		auto move = Utils::Normalize(player->GetPos() - GetPos());
		Translate(dt * moveSpeed * move);

		if (animationPlay)
		{
			animation.Play("Platinum");
			animationPlay = false;
		}
	}
}

void ChaosFragments::GetChaos(Player* player)
{
	int addPlatinum = Utils::RandomRange(1, 4);
	player->AddPlatinum(addPlatinum);
}

void ChaosFragments::SetPosition(Vector2f pos)
{
	SetPos(pos + Utils::RandAreaPoint() * 10.f);
}
