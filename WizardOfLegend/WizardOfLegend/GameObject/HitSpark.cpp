#include "HitSpark.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/Animator.h"

HitSpark::HitSpark()
{
}

HitSpark::~HitSpark()
{
}

void HitSpark::Init()
{
	SpriteObj::Init();
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("HitEffect"));

}

void HitSpark::Reset()
{
	SpriteObj::Reset();
}

void HitSpark::Update(float dt)
{
	SpriteObj::Update(dt);
}

void HitSpark::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
}

void HitSpark::HitSparkFire(Vector2f hitPoint)
{
	SetPos(hitPoint);
}
