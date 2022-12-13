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
	Effect::Init();
	animation = new Animator();
	animation->AddClip(*RESOURCE_MGR->GetAnimationClip("HitEffect"));
	animation->SetTarget(&sprite);
	
	{
		AnimationEvent ev;
		ev.clipId = "HitEffect";
		ev.frame = RESOURCE_MGR->GetAnimationClip(ev.clipId)->GetFrameCount() - 1;
		ev.onEvent = bind(&HitSpark::SetActive, this, false);
		animation->AddEvent(ev);
	}
}

void HitSpark::Reset()
{
	Effect::Reset();
	animation->Play("HitEffect");
}

void HitSpark::Update(float dt)
{
	Effect::Update(dt);
}

void HitSpark::Draw(RenderWindow& window)
{
	Effect::Draw(window);
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
