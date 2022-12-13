#include "ItemBox.h"
#include "../../Framework/Animator.h"
#include "../../Framework/ResourceMgr.h"

ItemBox::ItemBox()
	:currState(States::None), idleActionDelay(4.f), idleActionTimer(0.f)
{
}

ItemBox::~ItemBox()
{
}

void ItemBox::Init()
{
	Interactive::Init();
	animator = new Animator();
	animator->SetTarget(&sprite);
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("ItemBoxIdle"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("ItemBoxOpen"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("ItemBoxClose"));
	{
		vector<string> clipIds = { "ItemBoxClose" };
		for (int i = 0; i < clipIds.size(); ++i)
		{
			AnimationEvent ev;
			ev.clipId = clipIds[i];
			ev.frame = RESOURCE_MGR->GetAnimationClip(ev.clipId)->GetFrameCount() - 1;
			ev.onEvent = bind(&ItemBox::SetState, this, States::Idle);
			animator->AddEvent(ev);
		}
	}
	SetHitBox(FloatRect(0.f, 0.f, 46.f, 25.f));
	SetHitBoxOrigin(Origins::BC);
	SetInteractKeyPos({ 0.f, -50.f });

	SetInteractBox({ 56.f, 30.f });
	SetInteractBoxOrigin(Origins::BC);

	SetState(States::Idle);
}

void ItemBox::Reset()
{
	Interactive::Reset();
	SetState(States::Idle);
	idleActionTimer = 0.f;
}

void ItemBox::Update(float dt)
{
	Interactive::Update(dt);
}

void ItemBox::SetState(States state)
{
	if (currState == state)
		return;
	currState = state;
	switch (state)
	{
	case ItemBox::States::Idle:
		animator->Play("ItemBoxIdle");
		break;
	case ItemBox::States::Open:
		animator->Play("ItemBoxOpen");
		break;
	case ItemBox::States::Close:
		animator->Play("ItemBoxClose");
		break;
	default:
		break;
	}
}
