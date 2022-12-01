#include "FinalBoss.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/Framework.h"
#include "../Scene/SceneMgr.h"
#include "Skill.h"
#include "Player.h"

FinalBoss::FinalBoss()
	:currState(States::None), animator(nullptr), attackDmg(0), attackCnt(0), attackRange(0.f), speed(400.f), dashDuration(0.5f), dashTimer(0.f), evasionCntLim(3), evasionCnt(0), dashType(DashType::Evasion), lastDir(0.f, 1.f), dashDir(0.f, 1.f), isBackHand(false), vecIdx(0), currSkill(nullptr), maxHp(825), curHp(825), player(nullptr), superArmor(true), superArmorDelay(4.5f), superArmorTimer(0.f)
{
	direction = {0.f, 1.f};
}

FinalBoss::~FinalBoss()
{
}

void FinalBoss::SetState(States state)
{
	if (state != States::NormalSpell && currState == state)
		return;
	switch (state)
	{
	case States::Idle:
		{
			auto angle = Utils::Angle(lastDir);
			if (angle > -135.f && angle < -45.f)
				animator->Play("FinalBossIdleUp");
			else if (angle > 45.f && angle < 135.f)
				animator->Play("FinalBossIdleDown");
			else if (angle >= -45.f && angle <= 45.f)
				animator->Play("FinalBossIdleRight");
			else
				animator->Play("FinalBossIdleLeft");
		}
		break;
	case States::Dash:
		{
			auto angle = Utils::Angle(dashDir);
			if (angle > -135.f && angle < -45.f)
				animator->Play("FinalBossDashUp");
			else if (angle > 45.f && angle < 135.f)
				animator->Play("FinalBossDashDown");
			else if (angle >= -45.f && angle <= 45.f)
				animator->Play("FinalBossDashRight");
			else
				animator->Play("FinalBossDashLeft");
		}
		break;
	case States::Slide:
		if (Utils::EqualFloat(dashDir.x, 0.f))
			dashDir.y > 0.f ? animator->Play("FinalBossSlideDown") : animator->Play("FinalBossSlideUp");
		else
			dashDir.x > 0.f ? animator->Play("FinalBossSlideRight") : animator->Play("FinalBossSlideLeft");
		break;
	case States::NormalSpell:
		{
			auto angle = Utils::Angle(direction);
			if (angle > -135.f && angle <= -45.f)
				isBackHand ? animator->Play("FinalBossBackHandUp") : animator->Play("FinalBossForeHandUp");
			else if (angle > 45.f && angle <= 135.f)
				isBackHand ? animator->Play("FinalBossBackHandDown") : animator->Play("FinalBossForeHandDown");
			else if (angle > -45.f && angle <= 45.f)
				isBackHand ? animator->Play("FinalBossBackHandRight") : animator->Play("FinalBossForeHandRight");
			else
				isBackHand ? animator->Play("FinalBossBackHandLeft") : animator->Play("FinalBossForeHandLeft");
			isBackHand = !isBackHand;
		}
		break;
	case States::SplitCast:
		{
			auto angle = Utils::Angle(direction);
			if (angle > -135.f && angle <= -45.f)
				animator->Play("FinalBossSplitCastUp");
			else if (angle > 45.f && angle <= 135.f)
				animator->Play("FinalBossSplitCastDown");
			else if (angle > -45.f && angle <= 45.f)
				animator->Play("FinalBossSplitCastRight");
			else
				animator->Play("FinalBossSplitCastLeft");
		}
		break;
	case States::GroundSlam:
		lastDir.y >= 0.f ? animator->Play("FinalBossGroundSlamDown") : animator->Play("FinalBossGroundSlamUp");
		break;
	case States::GroundSlamEnd:
		lastDir.y >= 0.f ? animator->Play("FinalBossGroundSlamDownEnd") : animator->Play("FinalBossGroundSlamUpEnd");
		break;
	default:
		break;
	}
	currState = state;
}


void FinalBoss::Init()
{
	SpriteObj::Init();
	SetPos(Vector2f(FRAMEWORK->GetWindowSize()) * 0.25f);

	animator = new Animator();
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossIdleDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossIdleRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossIdleLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossIdleUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossBackHandDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossBackHandRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossBackHandLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossBackHandUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossForeHandDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossForeHandRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossForeHandLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossForeHandUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossSplitCastDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossSplitCastRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossSplitCastLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossSplitCastUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossDashDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossDashRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossDashLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossDashUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossSlideRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossSlideLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossSlideDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossSlideUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossGroundSlamDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossGroundSlamUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossGroundSlamDownEnd"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossGroundSlamUpEnd"));
	{
		vector<string> clipIds = { "FinalBossSlideRight", "FinalBossSlideLeft", "FinalBossSlideDown", "FinalBossSlideUp", "FinalBossGroundSlamDownEnd", "FinalBossGroundSlamUpEnd" };
		for (int i = 0; i < clipIds.size(); ++i)
		{
			AnimationEvent ev;
			ev.clipId = clipIds[i];
			ev.frame = RESOURCE_MGR->GetAnimationClip(ev.clipId)->GetFrameCount() - 1;
			ev.onEvent = bind(&FinalBoss::NextAction, this);
			animator->AddEvent(ev);
		}
	}
	{
		vector<string> clipIds = { "FinalBossBackHandRight", "FinalBossBackHandLeft", "FinalBossBackHandDown", "FinalBossBackHandUp", "FinalBossForeHandRight", "FinalBossForeHandLeft", "FinalBossForeHandDown", "FinalBossForeHandUp", "FinalBossSplitCastRight", "FinalBossSplitCastLeft", "FinalBossSplitCastDown", "FinalBossSplitCastUp", "FinalBossGroundSlamDown", "FinalBossGroundSlamUp" };
		for (int i = 0; i < clipIds.size(); ++i)
		{
			AnimationEvent ev;
			ev.clipId = clipIds[i];
			ev.frame = RESOURCE_MGR->GetAnimationClip(ev.clipId)->GetFrameCount() - 1;
			ev.onEvent = bind(&FinalBoss::FinishAction, this);
			animator->AddEvent(ev);
		}
	}
	animator->SetTarget(&sprite);
	SetState(States::Idle);

	for (int i = 0; i < 6; ++i)
	{
		Skill* newSkill = new Skill();
		newSkill->SetSubject(this, Skill::SubjectType::FinalBoss);
		normalSkills.push_back(newSkill);
	}
	for (int i = 0; i < 6; ++i)
	{
		Skill* newSkill = new Skill();
		newSkill->SetSubject(this, Skill::SubjectType::FinalBoss);
		chaosSkills.push_back(newSkill);
	}

	SetHitBox(FloatRect(0.f, 0.f, 10.f, 25.f));
	SetHitBoxOrigin(Origins::MC);

	SetMaxHp(825);
	SetCurHp(maxHp);
}


void FinalBoss::Release()
{
}

void FinalBoss::Reset()
{
}

void FinalBoss::Update(float dt)
{
}

void FinalBoss::Draw(RenderWindow& window)
{
	for (auto skill : normalSkills)
	{
		skill->Draw(window);
	}
	for (auto skill : chaosSkills)
	{
		skill->Draw(window);
	}
	SpriteObj::Draw(window);
}

void FinalBoss::UpdateIdle(float dt)
{
	superArmorTimer += dt;
	if (superArmorTimer >= 4.5f)
		superArmor = true;
	else if (superArmorTimer >= 3.f)
	{
		if(evasionCnt == 1)
			Dash(DashType::Evasion);
	}
	else if (superArmorTimer >= 1.5f)
	{
		if(evasionCnt == 0)
			Dash(DashType::Evasion);
	}
}

void FinalBoss::UpdateDash(float dt)
{
	dashTimer += dt;
	Translate(dashDir * speed * dt);
	if (dashType == DashType::Chase && Utils::Distance(position, player->GetPos()) < currSkill->GetSetting()->distance)
	{
		if (vecIdx < normalSkills.size())
		{
			normalSkills[vecIdx]->Do();
			currSkill = normalSkills[vecIdx];
			++vecIdx;
			evasionCntLim = Utils::RandomRange(0, 3);
		}
		else
		{
			random_device rd;
			mt19937 g(rd());
			shuffle(normalSkills.begin(), normalSkills.end(), g);
			vecIdx = 0;

			auto skillIdx = Utils::RandomRange(0, chaosSkills.size());
			chaosSkills[skillIdx]->Do();
			currSkill = chaosSkills[skillIdx];
			evasionCntLim = 2;
		}
		++attackCnt;
		evasionCnt = 0;
	}
	if (dashTimer >= dashDuration)
	{
		dashTimer = 0.f;
		FinishAction();
	}
}

void FinalBoss::UpdateWait(float dt)
{
	if (currSkill->GetDoing())
		return;
	if (currSkill->GetSetting()->playerAction == Player::SkillAction::GroundSlam)
		SetState(States::GroundSlamEnd);
}

void FinalBoss::Action(Skill* skill)
{
}

void FinalBoss::Dash(DashType type)
{
	auto playerDir = player->GetPos() - position;
	switch (type)
	{
	case FinalBoss::DashType::Evasion:
		{
			auto angle = Utils::Angle({ playerDir.x, -playerDir.y });
			direction = Utils::RandomRange(0, 2) == 0 ? Vector2f(cos(angle + 60.f), -sin(angle + 60.f)) : Vector2f(cos(angle - 60.f), -sin(angle - 60.f));
			++evasionCnt;
		}
		break;
	case FinalBoss::DashType::Chase:
		if (Utils::Distance(position, player->GetPos()) > attackRange)
			direction = Vector2f((player->GetPos() + Utils::RandContact() * attackRange) - position);
		else
			direction = player->GetPos() - position;
		break;
	default:
		break;
	}
	SetState(States::Dash);
}

void FinalBoss::FinishAction()
{
	if (currSkill != nullptr && currSkill->GetSetting()->stopMoving == Skill::StopMoving::Immovable)
	{
		SetState(States::Wait);
		return;
	}
	switch (currState)
	{
	case States::GroundSlam:
		SetState(States::GroundSlamEnd);
		break;
	default:
		NextAction();
		break;
	}
}

void FinalBoss::NextAction()
{
	if (attackCnt < normalSkills.size() + 1)
		SetState(States::Dash);
	else
	{
		if (currState == States::Dash)
			SetState(States::Slide);
		else
		{
			superArmor = false;
			attackCnt = 0;
			SetState(States::Idle);
		}
	}
}
