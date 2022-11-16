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
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossDownFireballIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossUpFireballIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftFireballIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightFireballIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightThrowingIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftThrowingIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossDownThrowingIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossUpThrowingIdle"));

	SetPos({200, 200});
	SetSpeed(700.f);
	SetMoveScale(10000.f);
	SetAttackScale(0.f);
	SetMonsterType(MonsterType::MiddleBoss);
	SetMaxHp(1);
	SetCurHp(GetMaxHp());
	//SetScale({1.5, 1.5});

	//attackType = AttackType::FireBall;
	attackType = AttackType::ThirdAttack;
}

void FireBoss::Update(float dt)
{
	Enemy::Update(dt);

	if (curBossState == BossStates::Move || attackType != AttackType::ThirdAttack)
	{
		float angle = Utils::Angle(player->GetPos(), GetPos());

		if ((angle >= -44 && angle <= 45) || (angle >= -180 && angle <= -130 || angle >= 130 && angle <= 180))
			moveType = MoveType::LeftAndRight;
		if ((angle > 45 && angle < 130) || (angle <= -45 && angle > -130))
			moveType = MoveType::TopAndBottom;

		if (curBossState == BossStates::Move)
			BossMonsterMove(dt);
	}

	if (attackType == AttackType::ThirdAttack && curBossState == BossStates::Attack && thirdAttackCount > 0 && patternDelay <= 0.f)
	{
		auto kick = Utils::Normalize(playerLastPos - lastPos);
		Translate({ (dt * speed * kick) / 5.f });
	}

	if (attackType == AttackType::ThrowingKnife && patternDelay < 0.3f && patternDelay > 0.f)
	{
		auto kick = Utils::Normalize(playerLastPos - lastPos);
		Translate({ (dt * speed * kick) / 4.f * -1.f });
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
				animation.Play(playerLastPos.x < lastPos.x ? "FireBossLeftLand" : "FireBossRightLand");
			else
			{
				attackDelay = 0.3f;
				nextPatternDelay = 1.5f;
				switch (moveType)
				{
				case FireBoss::MoveType::LeftAndRight:
					animation.Play(playerLastPos.x < lastPos.x ? "FireBossLeftThirdAttack" : "FireBossRightThirdAttack");
					break;
				case FireBoss::MoveType::TopAndBottom:
					animation.Play(playerLastPos.y < lastPos.y ? "FireBossUpThirdAttack" : "FireBossDownThirdAttack");
					break;
				}
			}
			break;
		case FireBoss::AttackType::FireBall:
			if (nextPatternDelay > 0.f)
			{
				switch (lastMoveType)
				{
				case FireBoss::MoveType::LeftAndRight:
					animation.Play(playerLastPos.x < lastPos.x ? "FireBossLeftFireballIdle" : "FireBossRightFireballIdle");
					break;
				case FireBoss::MoveType::TopAndBottom:
					animation.Play(playerLastPos.y < lastPos.y ? "FireBossUpFireballIdle" : "FireBossDownFireballIdle");
					break;
				}
			}
			else
			{
				cout << playerLastPos.x << " " << lastPos.x << endl;

				attackDelay = 1.f;
				nextPatternDelay = 1.5f;
				switch (lastMoveType)
				{
				case FireBoss::MoveType::LeftAndRight:
					animation.Play(playerLastPos.x < lastPos.x ? "FireBossLeftFireball" : "FireBossRightFireball");
					break;
				case FireBoss::MoveType::TopAndBottom:
					animation.Play(playerLastPos.y < lastPos.y ? "FireBossUpFireball" : "FireBossDownFireball");
					break;
				}
			}
			break;
		case FireBoss::AttackType::ThrowingKnife:
			if (nextPatternDelay > 0.f)
			{
				switch (lastMoveType)
				{
				case FireBoss::MoveType::LeftAndRight:
					animation.Play(playerLastPos.x < lastPos.x ? "FireBossLeftThrowingIdle" : "FireBossRightThrowingIdle");
					break;
				case FireBoss::MoveType::TopAndBottom:
					animation.Play(playerLastPos.y < lastPos.y ? "FireBossUpThrowingIdle" : "FireBossDownThrowingIdle");
					break;
				}
			}
			else
			{
				attackDelay = 1.f;
				nextPatternDelay = 1.5f;
				switch (lastMoveType)
				{
				case FireBoss::MoveType::LeftAndRight:
					animation.Play(playerLastPos.x < lastPos.x ? "FireBossLeftThrowing" : "FireBossRightThrowing");
					break;
				case FireBoss::MoveType::TopAndBottom:
					animation.Play(playerLastPos.y < lastPos.y ? "FireBossUpThrowing" : "FireBossDownThrowing");
					break;
				}
			}
			break;
		case FireBoss::AttackType::DragonAttack:
			switch (moveType)
			{
			case FireBoss::MoveType::LeftAndRight:
				animation.Play(playerLastPos.x < lastPos.x ? "FireBossLeftThrowing" : "FireBossRightThrowing");
				break;
			case FireBoss::MoveType::TopAndBottom:
				animation.Play(playerLastPos.y < lastPos.y ? "FireBossUpThrowing" : "FireBossDownThrowing");
				break;
			}
			break;
		case FireBoss::AttackType::Meteor:
			switch (moveType)
			{
			case FireBoss::MoveType::LeftAndRight:
				animation.Play(player->GetPos().x < GetPos().x ? "FireBossLeftFireball" : "FireBossRightFireball");
				break;
			case FireBoss::MoveType::TopAndBottom:
				animation.Play(player->GetPos().y < GetPos().y ? "FireBossUpFireball" : "FireBossDownFireball");
				break;
			}
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
					if (thirdAttackCount == 3 && patternCount > 0)
						patternCount--;

					thirdAttackCount--;
				}
			}

			if (thirdAttackCount > 0)
				attackType = AttackType::ThirdAttack;
			else if (thirdAttackCount == 0 && patternDelay <= 0.f && attackDelay <= 0.f)
			{
				attackType = (AttackType)Utils::RandomRange(1, 3);

				if (attackType == AttackType::FireBall || attackType == AttackType::ThrowingKnife)
				{
					playerLastPos = player->GetPos();
					lastPos = GetPos();
					lastMoveType = moveType;
				}

				SetState(BossStates::Move);
			}

			break;
		case FireBoss::AttackType::FireBall:
			if (nextPatternDelay <= 0.f)
			{
				SetState(BossStates::None);
				SetState(BossStates::Attack);
				if (patternDelay <= 0.f)
				{
					attackType = (AttackType)Utils::RandomRange(1, 3);

					if (attackType == AttackType::ThirdAttack)
					{
						thirdAttackCount = 3;
						patternDelay = 0.5f;
						SetAttackScale(30.f);
					}
					else if (attackType == AttackType::ThrowingKnife)
					{
						playerLastPos = player->GetPos();
						lastPos = GetPos();
						lastMoveType = moveType;
					}

					SetState(BossStates::Move);
				}
			}
			break;
		case FireBoss::AttackType::ThrowingKnife:
			if (nextPatternDelay <= 0.f)
			{
				SetState(BossStates::None);
				SetState(BossStates::Attack);

				if (patternDelay <= 0.f)
				{
					attackType = AttackType::ThirdAttack;

					if (attackType == AttackType::ThirdAttack)
					{
						thirdAttackCount = 3;
						patternDelay = 0.5f;
						SetAttackScale(30.f);
						SetState(BossStates::Move);
					}
				}
			}
			break;
		case FireBoss::AttackType::DragonAttack:
			SetState(BossStates::Attack);
			break;
		case FireBoss::AttackType::Meteor:
			SetState(BossStates::Attack);
			break;
		}

		if (patternCount == 0 && nextPatternDelay <= 0.f)
		{
			cout << "pattern reset" << endl;
			SetState(BossStates::Idle);
			//attackType = (AttackType)Utils::RandomRange(0, 5);
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

		if (thirdAttackCount == 3 && attackType == AttackType::ThirdAttack)
		{
			playerLastPos = player->GetPos();
			lastPos = GetPos();
			patternDelay = 0.5f;
		}
		else
			patternDelay = 1.5f;
		//if (attackType == AttackType::FireBall)
		//{
		//	patternDelay = 1.5f;
		//}
		//if (attackType == AttackType::DragonAttack)
		//{
		//	patternDelay = 1.5f; // 5
		//}
		//if (attackType == AttackType::ThrowingKnife)
		//{
		//	patternDelay = 1.5f;
		//}

		return;
	}

	if (Utils::Distance(player->GetPos(), GetPos()) >= GetMoveScale())
	{
		direction.x = 0;
		SetState(BossStates::Move);

		return;
	}
}
