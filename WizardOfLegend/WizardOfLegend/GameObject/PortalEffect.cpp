#include "PortalEffect.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/Animator.h"
#include "../Framework/Utils.h"

PortalEffect::PortalEffect()
{
}

PortalEffect::~PortalEffect()
{
}

void PortalEffect::Init()
{
	Effect::Init();
	animation = new Animator();
	animation->AddClip(*RESOURCE_MGR->GetAnimationClip("Teleport"));
	animation->SetTarget(&sprite);
}

void PortalEffect::Reset()
{
	Effect::Reset();
}

void PortalEffect::Update(float dt)
{
	Effect::Update(dt);

	if (showing == false)
	{
		animation->Play("Teleport");
		showing = true;
	}

	if (animation->GetCurrentFrame() == 9)
	{
		SetActive(false);
		showing = false;
	}
}

void PortalEffect::Draw(RenderWindow& window)
{
	Effect::Draw(window);
}

void PortalEffect::ShowPortalEffect(Vector2f playerPos)
{
	SetPos(playerPos);
}
