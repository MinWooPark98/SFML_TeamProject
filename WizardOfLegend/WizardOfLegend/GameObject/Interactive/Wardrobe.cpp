#include "Wardrobe.h"
#include "../../Framework/Animator.h"
#include "../../Framework/ResourceMgr.h"

Wardrobe::Wardrobe()
	:currState(States::None), idleActionDelay(2.f), idleActionTimer(0.f)
{
}

Wardrobe::~Wardrobe()
{
}

void Wardrobe::Init()
{
	Interactive::Init();
	animator = new Animator();
	animator->SetTarget(&sprite);
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("WardrobeIdle"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("WardrobeOpen"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("WardrobeClose"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("WardrobeBlink"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("WardrobeMustache"));
	{
		vector<string> clipIds = { "WardrobeClose", "WardrobeBlink", "WardrobeMustache" };
		for (int i = 0; i < clipIds.size(); ++i)
		{
			AnimationEvent ev;
			ev.clipId = clipIds[i];
			ev.frame = RESOURCE_MGR->GetAnimationClip(ev.clipId)->GetFrameCount() - 1;
			ev.onEvent = bind(&Wardrobe::SetState, this, States::Idle);
			animator->AddEvent(ev);
		}
	}
	SetHitBox(FloatRect(0.f, 0.f, 46.f, 20.f));
	SetHitBoxOrigin(Origins::BC);
	SetInteractKeyPos({ 0.f, -70.f });

	SetInteractBox({ 56.f, 25.f });
	SetInteractBoxOrigin(Origins::BC);

	SetState(States::Idle);
}

void Wardrobe::Reset()
{
	Interactive::Reset();
	SetState(States::Idle);
	idleActionTimer = 0.f;
}

void Wardrobe::Update(float dt)
{
	Interactive::Update(dt);
	if (currState == States::Idle)
	{
		idleActionTimer += dt;
		if (idleActionTimer > idleActionDelay)
		{
			idleActionTimer = 0.f;
			Utils::RandomRange(0, 2) == 0 ? SetState(States::Blink) : SetState(States::Mustache);
		}
	}
}

void Wardrobe::SetState(States state)
{
	if (currState == state)
		return;
	currState = state;
	switch (state)
	{
	case Wardrobe::States::Idle:
		idleActionTimer = 0.f;
		animator->Play("WardrobeIdle");
		break;
	case Wardrobe::States::Open:
		animator->Play("WardrobeOpen");
		break;
	case Wardrobe::States::Close:
		animator->Play("WardrobeClose");
		break;
	case Wardrobe::States::Blink:
		animator->Play("WardrobeBlink");
		break;
	case Wardrobe::States::Mustache:
		animator->Play("WardrobeMustache");
		break;
	default:
		break;
	}
}
