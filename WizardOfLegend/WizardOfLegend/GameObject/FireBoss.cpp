#include "FireBoss.h"

void FireBoss::Init()
{
	Enemy::Init();

	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossClear"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossDie"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossDownFireball"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossDownRun"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossDownThrowing"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftAttackReady"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftFireball"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftFly"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftHit"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftLand"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftRun"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftStern"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftStomp"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftThirdAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftThrowing"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightAttackReady"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightFireball"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightFly"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightHit"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightLand"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightRun"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightStern"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightStomp"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightThirdAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightThrowing"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossUpFireball"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossUpRun"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossUpThirdAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossUpThrowing"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossDownThirdAttack"));

	SetPos({200, 200});
	SetSpeed(700.f);
	SetMoveScale(10000.f);
	SetAttackScale(0.f);
	SetMonsterType(MonsterType::MiddleBoss);
	SetMaxHp(1);
	SetCurHp(GetMaxHp());
	//SetScale({1.5, 1.5});

	attackType = AttackType::ThirdAttack;
}

void FireBoss::Update(float dt)
{
	Enemy::Update(dt);

	if (curBossState == BossStates::Move)
	{
		float angle = Utils::Angle(player->GetPos(), GetPos());

		if ((angle >= -44 && angle <= 45) || (angle >= -180 && angle <= -130 || angle >= 130 && angle <= 180))
			moveType = MoveType::LeftAndRight;
		if ((angle > 45 && angle < 130) || (angle <= -45 && angle > -130))
			moveType = MoveType::TopAndBottom;

		BossMonsterMove(dt);
	}

	if (attackType == AttackType::ThirdAttack && curBossState == BossStates::Attack && thirdAttackCount > 0 && patternDelay <= 0.f)
	{
		auto kick = Utils::Normalize(playerLastPos - lastPos);
		Translate({ (dt * speed * kick) / 5.f });
	}

	if (InputMgr::GetKeyDown(Keyboard::Key::L))
	{
		SetState(BossStates::Idle);
	}
	if (InputMgr::GetKeyDown(Keyboard::Key::K))
	{
		SetState(BossStates::Move);
	}

	nextPatternDelay -= dt;
	patternDelay -= dt;
	animation.Update(dt);
}

void FireBoss::Draw(RenderWindow& window)
{
	Enemy::Draw(window);
	window.draw(sprite);
}

void FireBoss::SetState(BossStates newState)
{
	if (curBossState == newState)
		return;

	curBossState = newState;

	switch (curBossState)
	{
	case BossStates::Idle:
		animation.Play("FireBossIdle");
		break;
	case BossStates::Move:
		switch (moveType)
		{
		case FireBoss::MoveType::LeftAndRight:
			direction.x < 0.f ? animation.Play("FireBossLeftRun") : animation.Play("FireBossRightRun");
			break;
		case FireBoss::MoveType::TopAndBottom:
			direction.y < 0.f ? animation.Play("FireBossUpRun") : animation.Play("FireBossDownRun");
			break;
		}
		break;
	case BossStates::Attack:
		switch (attackType)
		{
		case FireBoss::AttackType::ThirdAttack:
			if (patternDelay > 0.f)
				playerLastPos.x < lastPos.x ? animation.Play("FireBossLeftAttackReady") : animation.Play("FireBossRightAttackReady");
			else
			{
				nextPatternDelay = 0.5f;
				attackDelay = 0.3f;

				switch (moveType)
				{
				case FireBoss::MoveType::LeftAndRight:
					playerLastPos.x < lastPos.x ? animation.Play("FireBossLeftThirdAttack") : animation.Play("FireBossRightThirdAttack");
					break;
				case FireBoss::MoveType::TopAndBottom:
					playerLastPos.y < lastPos.y ? animation.Play("FireBossUpThirdAttack") : animation.Play("FireBossDownThirdAttack");
					break;
				}
			}
			break;
		case FireBoss::AttackType::FireBall:
			break;
		case FireBoss::AttackType::ThrowingKnife:
			break;
		case FireBoss::AttackType::DragonAttack:
			break;
		case FireBoss::AttackType::Meteor:
			break;
		}
		break;
	case BossStates::Hit:
		direction.x < 0.f ? animation.Play("FireBossLeftHit") : animation.Play("FireBossRightHit");
		break;
	case BossStates::Die:
		animation.Play("FireBossDie");
		break;
	case BossStates::Clear:
		animation.Play("FireBossClear");
		break;
	}
}

void FireBoss::UpdateAttack(float dt)
{
	if (curHp <= 0)
		return;

	if (attackDelay <= 0.f)
	{
		switch (attackType)
		{
		case FireBoss::AttackType::ThirdAttack:
			if (thirdAttackCount != 0)
			{
				SetState(BossStates::None);
				SetState(BossStates::Attack);

				if (patternDelay <= 0.f)
				{
					if (thirdAttackCount > 0)
						attackType = AttackType::ThirdAttack;

					if (thirdAttackCount == 3 && patternCount > 0)
						patternCount--;

					thirdAttackCount--;
				}
			}
			break;
		case FireBoss::AttackType::FireBall:
			break;
		case FireBoss::AttackType::ThrowingKnife:
			break;
		case FireBoss::AttackType::DragonAttack:
			break;
		case FireBoss::AttackType::Meteor:
			break;
		}

		if (patternCount == 0 && nextPatternDelay <= 0.f)
		{
			cout << "pattern reset" << endl;
			SetState(BossStates::Idle);
			attackType = (AttackType)Utils::RandomRange(0, 5);
			thirdAttackCount = 3;
			patternCount = 3;
		}
	}
}

void FireBoss::UpdateMove(int attackDelay)
{
	if (attackType == AttackType::ThirdAttack)
		SetAttackScale(30.f);
	else
		SetAttackScale(10000.f);

	if (Utils::Distance(player->GetPos(), GetPos()) <= GetAttackScale())
	{
		SetState(BossStates::Attack);
		this->attackDelay = attackDelay;
		thirdAttackCount = 3;

		if (thirdAttackCount == 3)
		{
			playerLastPos = player->GetPos();
			lastPos = GetPos();
			patternDelay = 0.5f;
		}

		return;
	}
}
