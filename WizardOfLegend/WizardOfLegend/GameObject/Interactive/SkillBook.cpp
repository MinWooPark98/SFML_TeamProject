#include "SkillBook.h"
#include "../../Framework/Animator.h"
#include "../../Framework/ResourceMgr.h"

SkillBook::SkillBook()
	:currState(States::None), idleActionDelay(3.f), idleActionTimer(0.f)
{
}

SkillBook::~SkillBook()
{
}

void SkillBook::Init()
{
	Interactive::Init();
	animator = new Animator();
	animator->SetTarget(&sprite);
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("SkillBookIdle"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("SkillBookTurn"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("SkillBookOpenReady"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("SkillBookOpen"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("SkillBookBigChomp"));
	{
		vector<string> clipIds = { "SkillBookTurn", "SkillBookBigChomp" };
		for (int i = 0; i < clipIds.size(); ++i)
		{
			AnimationEvent ev;
			ev.clipId = clipIds[i];
			ev.frame = RESOURCE_MGR->GetAnimationClip(ev.clipId)->GetFrameCount() - 1;
			ev.onEvent = bind(&SkillBook::SetState, this, States::Idle);
			animator->AddEvent(ev);
		}
	}
	SetHitBox(FloatRect(0.f, 0.f, 36.f, 24.f));
	SetHitBoxOrigin(Origins::TC);
	SetInteractKeyPos({ 0.f, -30.f });

	SetState(States::Idle);
}

void SkillBook::Release()
{
	Interactive::Release();
}

void SkillBook::Reset()
{
	Interactive::Reset();
}

void SkillBook::Update(float dt)
{
	Interactive::Update(dt);
	if (currState == States::Idle)
	{
		idleActionTimer += dt;
		if (idleActionTimer > idleActionDelay)
		{
			idleActionTimer = 0.f;
			Utils::RandomRange(0, 2) == 0 ? SetState(States::Turn) : SetState(States::BigChomp);
		}
	}
}

void SkillBook::SetState(States state)
{
	if (currState == state)
		return;
	currState = state;
	switch (state)
	{
	case SkillBook::States::Idle:
		animator->Play("SkillBookIdle");
		break;
	case SkillBook::States::OpenReady:
		animator->Play("SkillBookOpenReady");
		break;
	case SkillBook::States::Open:
		animator->Play("SkillBookOpen");
		break;
	case SkillBook::States::Turn:
		animator->Play("SkillBookTurn");
		break;
	case SkillBook::States::BigChomp:
		animator->Play("SkillBookBigChomp");
		break;
	default:
		break;
	}
}
