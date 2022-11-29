#include "CastingCircle.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"

CastingCircle::CastingCircle()
	:animator(nullptr), duration(0.f), timer(0.f), dmgType(Skill::DamageType::Once), attackDmg(0), isOnAtkDelay(false), atkDelay(0.f), atkTimer(0.f)
{
}

CastingCircle::~CastingCircle()
{
}

void CastingCircle::Init()
{
	SpriteObj::Init();
	animator = new Animator();
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("CastingCircle"));
	animator->SetTarget(&sprite);
}

void CastingCircle::Release()
{
	SpriteObj::Release();
	if (animator != nullptr)
		delete animator;
	animator = nullptr;
}

void CastingCircle::Reset()
{
	SpriteObj::Reset();
	timer = 0.f;
	isOnAtkDelay = false;
	atkTimer = 0.f;
	sprite.setColor({ 255, 255, 255, 255 });
}

void CastingCircle::Update(float dt)
{
	SpriteObj::Update(dt);
	animator->Update(dt);
	timer += dt;
	if (timer >= duration)
	{
		if(dmgType == Skill::DamageType::Once)
			// Once 충돌 처리
		SetActive(false);
	}

	if (dmgType == Skill::DamageType::Periodic)
	{
		if (!isOnAtkDelay)
		{
			// 충돌 검사 및 데미지
		}
		else
		{
			atkTimer += dt;
			if (atkTimer >= atkDelay)
			{
				isOnAtkDelay = false;
				atkTimer = 0.f;
			}
		}
	}
}

void CastingCircle::Do()
{
	if(animator != nullptr)
		animator->Play("CastingCircle");
}
