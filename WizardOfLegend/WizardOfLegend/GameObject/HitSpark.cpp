#include "HitSpark.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/Animator.h"
#include "../Framework/Utils.h"

HitSpark::HitSpark()
{
}

HitSpark::~HitSpark()
{
}

void HitSpark::Init()
{
	SpriteObj::Init();
	animation = new Animator();
	animation->AddClip(*RESOURCE_MGR->GetAnimationClip("HitEffect"));
	animation->SetTarget(&sprite);
}

void HitSpark::Reset()
{
	SpriteObj::Reset();
}

void HitSpark::Update(float dt)
{
	SpriteObj::Update(dt);
	animation->Update(dt);
	if (showing == false)
	{
		animation->Play("HitEffect");
		showing = true;
	}

	if (animation->GetCurrentFrame() == 6)
	{
		SetActive(false);
		showing = false;
	}
}

void HitSpark::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
	
}

void HitSpark::EnemyHitSparkFire(Vector2f hitPoint)
{
	SetPos(hitPoint);
	Rotate(Utils::RandomRange(0,100));
}

void HitSpark::PlayerHitSparkFire(Vector2f hitPoint)
{
	SetPos(hitPoint);
	Rotate(Utils::RandomRange(0, 100));
	SetColor(Color::Red);
}
