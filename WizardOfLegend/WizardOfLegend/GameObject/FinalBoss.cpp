#include "FinalBoss.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/Framework.h"
#include "../Scene/SceneMgr.h"
#include "Skill.h"
#include "Player.h"

FinalBoss::FinalBoss()
	:currState(States::None), animator(nullptr), attackDmg(0), attackCnt(0), attackRange(0.f), speed(400.f), dashDuration(0.5f), dashTimer(0.f), evasionCntLim(3), evasionCnt(0), dashType(DashType::Evasion), lastDir(0.f, 1.f), dashDir(0.f, 1.f), isBackHand(false), vecIdx(0), currSkill(nullptr), maxHp(825), curHp(825), hitDuration(0.3f), hitTimer(0.f), player(nullptr), superArmor(true), superArmorDelay(6.f), superArmorTimer(0.f)
{
	direction = {0.f, 1.f};
}

FinalBoss::~FinalBoss()
{
}

void FinalBoss::SetState(States state)
{
	if (state != States::NormalSpell && state != States::Dash && currState == state)
		return;
	switch (state)
	{
	case States::Idle:
		lastDir.x < 0.f ? animator->Play("FinalBossIdleLeft") : animator->Play("FinalBossIdleRight");
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
		animator->Play("FinalBossGroundSlamDown");
		break;
	case States::Hit:
		lastDir.x < 0.f ? animator->Play("FinalBossHurtLeft") : animator->Play("FinalBossHurtRight");
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
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossIdleRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossIdleLeft"));
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
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossHurtRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FinalBossHurtLeft"));
	{
		vector<string> clipIds = { "FinalBossSlideRight", "FinalBossSlideLeft", "FinalBossSlideDown", "FinalBossSlideUp" };
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
		vector<string> clipIds = { "FinalBossBackHandRight", "FinalBossBackHandLeft", "FinalBossBackHandDown", "FinalBossBackHandUp", "FinalBossForeHandRight", "FinalBossForeHandLeft", "FinalBossForeHandDown", "FinalBossForeHandUp", "FinalBossSplitCastRight", "FinalBossSplitCastLeft", "FinalBossSplitCastDown", "FinalBossSplitCastUp", "FinalBossGroundSlamDown" };
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

	vector<string> skills = { "DragonArc", "FireFull", "FireBall", "FlameWolf", "WideAreaMeteor" };
	for (int i = 0; i < skills.size(); ++i)
	{
		Skill* skill1 = new Skill();
		skill1->SetSkill(skills[i]);
		skill1->SetSubject(this, Skill::SubjectType::FinalBoss);
		normalSkills.push_back(skill1);
	}
	Skill* skill2 = new Skill();
	skill2->SetSkill("FireFull");
	skill2->SetSubject(this, Skill::SubjectType::FinalBoss);
	chaosSkills.push_back(skill2);
	currSkill = normalSkills[0];

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
	SpriteObj::Reset();
}

void FinalBoss::Update(float dt)
{
	SetLastPosition(GetPos());

	SpriteObj::Update(dt);
	animator->Update(dt);

	switch (currState)
	{
	case FinalBoss::States::Idle:
		UpdateIdle(dt);
		break;
	case FinalBoss::States::Dash:
		UpdateDash(dt);
		break;
	case FinalBoss::States::Wait:
		UpdateWait(dt);
		break;
	case FinalBoss::States::Hit:
		UpdateHit(dt);
		break;
	default:
		break;
	}

	for (auto skill : normalSkills)
	{
		skill->Update(dt);
	}
	for (auto skill : chaosSkills)
	{
		skill->Update(dt);
	}
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
	if (superArmorTimer >= superArmorDelay)
	{
		superArmor = true;
		superArmorTimer = 0.f;
		evasionCnt = 0;
		Dash(DashType::Evasion);
	}
	else if (superArmorTimer >= superArmorDelay * 2.f / 3.f)
	{
		if(evasionCnt == 1)
			Dash(DashType::Evasion);
	}
	else if (superArmorTimer >= superArmorDelay / 3.f)
	{
		if(evasionCnt == 0)
			Dash(DashType::Evasion);
	}
}

void FinalBoss::UpdateDash(float dt)
{
	dashTimer += dt;
	Translate(dashDir * speed * dt);
	if (dashType == DashType::Chase)
	{
		auto activateDistance = currSkill->GetSetting()->attackShape == Skill::AttackShape::Wave ? 100.f : currSkill->GetSetting()->distance;
		if(Utils::Distance(position, player->GetPos()) < activateDistance)
		{
			currSkill->Do();
			cout << currSkill->GetSetting()->skillName << endl;
			if (vecIdx < normalSkills.size())
			{
				++vecIdx;
				if (vecIdx == normalSkills.size())
				{
					currSkill = chaosSkills[Utils::RandomRange(0, chaosSkills.size())];
				}
				else
					currSkill = normalSkills[vecIdx];
				evasionCntLim = Utils::RandomRange(1, 3);
			}
			else
			{
				random_device rd;
				mt19937 g(rd());
				shuffle(normalSkills.begin(), normalSkills.end(), g);

				vecIdx = 0;
				currSkill = normalSkills[vecIdx];

				evasionCntLim = 2;
			}
			++attackCnt;
			evasionCnt = 0;
			dashTimer = 0.f;
		}
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
	NextAction();
}

void FinalBoss::Action(Skill* skill)
{
	switch (currSkill->GetSetting()->playerAction)
	{
	case Player::SkillAction::NormalSpell:
		SetState(States::NormalSpell);
		break;
	case Player::SkillAction::GroundSlam:
		SetState(States::GroundSlam);
		break;
	default:
		SetState(States::SplitCast);
		break;
	}
	direction = Utils::Normalize(player->GetPos() - position);
	if (!Utils::EqualFloat(direction.x, 0.f) || !Utils::EqualFloat(direction.y, 0.f))
	{
		lastDir = direction;
		dashDir = lastDir;
	}
	else if (!Utils::EqualFloat(lastDir.x, 0.f))
		dashDir = Utils::Normalize({ lastDir.x, 0.f });
}

void FinalBoss::Dash(DashType type)
{
	dashType = type;
	auto playerDir = player->GetPos() - position;
	switch (type)
	{
	case FinalBoss::DashType::Evasion:
		{
			auto angle = Utils::Angle({ playerDir.x, -playerDir.y });
			angle += Utils::RandomRange(60.f, 80.f) * (Utils::RandomRange(0, 2) == 0 ? 1.f : -1.f);
			auto angleRad = angle * M_PI / 180.f;
			direction = Vector2f(cos(angleRad), -sin(angleRad));
			++evasionCnt;
		}
		break;
	case FinalBoss::DashType::Chase:
		if (Utils::Distance(position, player->GetPos()) > attackRange)
			direction = Utils::Normalize(Vector2f((player->GetPos() + Utils::RandContact() * attackRange) - position));
		else
			direction = Utils::Normalize(player->GetPos() - position);
		break;
	default:
		break;
	}
	if (!Utils::EqualFloat(direction.x, 0.f) || !Utils::EqualFloat(direction.y, 0.f))
	{
		lastDir = direction;
		dashDir = lastDir;
	}
	else if (!Utils::EqualFloat(lastDir.x, 0.f))
		dashDir = Utils::Normalize({ lastDir.x, 0.f });
	SetState(States::Dash);
}

void FinalBoss::FinishAction()
{
	if (currSkill != nullptr && currSkill->GetSetting()->stopMoving == Skill::StopMoving::Immovable)
	{
		SetState(States::Wait);
		return;
	}
	NextAction();
}

void FinalBoss::UpdateHit(float dt)
{
	hitTimer += dt;
	superArmorTimer += dt;
	Translate(-lastDir * speed * 0.5f * dt);
	if (hitTimer >= hitDuration)
	{
		hitTimer = 0.f;
		SetState(States::Idle);
	}
}

void FinalBoss::NextAction()
{
	if (superArmor)
	{
		if (attackCnt < normalSkills.size() + 1)
		{
			evasionCnt < evasionCntLim ? Dash(DashType::Evasion) : Dash(DashType::Chase);
		}
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
	else
		SetState(States::Idle);
}

void FinalBoss::OnHit(const Vector2f& atkDir, int dmg)
{
	curHp -= dmg;
	if (superArmor)
		return;
	direction = -atkDir;
	lastDir = direction;
	dashDir = direction;
	SetState(States::Hit);
}
