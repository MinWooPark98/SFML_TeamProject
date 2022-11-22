#include "FireBoss.h"
#include "Skill.h"

void FireBoss::Init()
{
	Release();
	Enemy::Init();
	SetScale({ 1, 1 });
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
	RandomPatternSet(AttackType::None);
	//attackType = AttackType::DragonAttack;

	for (int i = 0; i < 6; ++i)
	{
		Skill* newSkill = new Skill();
		newSkill->SetSubject(this, Skill::SubjectType::Enemy);
		skills.push_back(newSkill);
	}
	skills[0]->SetSkill("DragonArc");
	skills[1]->SetSkill("FireFull");

	lastPos = GetPos();
}

void FireBoss::Update(float dt)
{
	Enemy::Update(dt);

	if (InputMgr::GetKeyDown(Keyboard::Key::H))
	{
		SetCurHp(0);
	}

	if (curHp <= 0 && isAlive)
	{
		isAlive = false;
	}

	if (curBossState == BossStates::Move || attackType != AttackType::ThirdAttack)
	{
		float angle = Utils::Angle(player->GetPos(), GetPos());

		if ((angle >= -44 && angle <= 45) || (angle >= -180 && angle <= -130 || angle >= 130 && angle <= 180))
			moveType = MoveType::LeftAndRight;
		if ((angle > 45 && angle < 130) || (angle <= -45 && angle > -130))
			moveType = MoveType::TopAndBottom;

		if (curBossState == BossStates::Move && patternCount != 0)
		{
			BossMonsterMove(dt);
		}
	}

	if (attackType == AttackType::ThirdAttack && curBossState == BossStates::Attack && thirdAttackCount > 0 && patternDelay <= 0.f)
	{
		auto kick = Utils::Normalize(playerLastPos - lastPos);
		Translate({ (dt * speed * kick) / 5.f });
	}

	if (attackType == AttackType::ThrowingKnife && isThrowingKnife)
	{
		auto throwing = Utils::Normalize(playerLastPos - lastPos);
		Translate({ (dt * speed * throwing) / 4.f * -1.f });
	}
	if (isThrowingKnife && nextPatternDelay <= 1.3f)
		isThrowingKnife = false;


	if (curBossState == BossStates::Idle && patternDelay <= 0.f)
		curBossState = BossStates::Move;


	if (curBossState != BossStates::Idle)
		nextPatternDelay -= dt;

	patternDelay -= dt;
	animation.Update(dt);
	for (auto skill : skills)
	{
		skill->Update(dt);
	}
}

void FireBoss::Draw(RenderWindow& window)
{
	if (isAlive)
	{
		Enemy::Draw(window);
		window.draw(sprite);
		for (auto skill : skills)
		{
			skill->Draw(window);
		}
	}
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
				skills[1]->Do();

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
				isThrowingKnife = false;
			}
			else
			{
				switch (skills[0]->GetSetting()->attackShape)
				{
				case Projectile::AttackShape::Range:
					skills[0]->SetSkillDir(Utils::Normalize(playerLastPos - lastPos));
					break;
				case Projectile::AttackShape::Wave:
					skills[0]->SetSkillDir(Utils::Normalize(playerLastPos - lastPos));
					break;
				}
				skills[0]->Do();

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
				isThrowingKnife = true;
			}
			break;
		case FireBoss::AttackType::DragonAttack:
			//if (nextPatternDelay > 0.f)
			//{
			//	switch (lastMoveType)
			//	{
			//	case FireBoss::MoveType::LeftAndRight:
			//		direction.x < 0.f ? animation.Play("FireBossLeftRun") : animation.Play("FireBossRightRun");
			//		break;
			//	case FireBoss::MoveType::TopAndBottom:
			//		direction.y < 0.f ? animation.Play("FireBossUpRun") : animation.Play("FireBossDownRun");
			//		break;
			//	}
			//	SetPos({0, 0});
			//}
			//else
			//{
			//	animation.Play(playerLastPos.x < lastPos.x ? "FireBossLeftLand" : "FireBossRightLand");
			//	attackDelay = 1.f;
			//	nextPatternDelay = 1.5f;
			//}
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
			UpdateThirdAttack(dt);
			break;
		case FireBoss::AttackType::FireBall:
			UpdateFireball(dt);
			break;
		case FireBoss::AttackType::ThrowingKnife:
			UpdateThrowingKnife(dt);
			break;
		//case FireBoss::AttackType::DragonAttack:
		//	UpdateDragonAttack(dt);
		//	break;
		//case FireBoss::AttackType::Meteor:
		//	SetState(BossStates::Attack);
			//break;
		}

		if (patternCount == 0 && attackDelay <= 0.f)
		{
			cout << "pattern reset" << endl;
			SetState(BossStates::Idle);
			RandomPatternSet(AttackType::Meteor); // 처음부터 메테오 패턴 안 나오게
			thirdAttackCount = 3;
			patternCount = 3;
			nextPatternDelay = 1.5f;
			patternDelay = 4.f;
			isThrowingKnife = false;
		}
	}
}

void FireBoss::UpdateMove(int attackDelay)
{
	if (attackType == AttackType::ThirdAttack)
		SetAttackScale(30.f);
	//else if (attackType == AttackType::DragonAttack)
	//	SetAttackScale(30.f);
	else
		SetAttackScale(10000.f);

	if (patternCount == 0)
		SetAttackScale(10000.f); // 3번공격 패턴에서 끝났을 때 근접해야 Idle로 바뀌던 오류 때문에 해둠

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
		{
			patternDelay = 1.5f;
		}

		return;
	}

	if (Utils::Distance(player->GetPos(), GetPos()) >= GetMoveScale())
	{
		direction.x = 0;
		SetState(BossStates::Move);

		return;
	}
}

void FireBoss::RandomPatternSet(AttackType type)
{
	int randomPattern = 0;

	do
	{
		randomPattern = Utils::RandomRange(0, 3); // 여기 변경
	} while (randomPattern == (int)type);

	attackType = (AttackType)randomPattern;
}

void FireBoss::PatternOptionSet()
{
	if (attackType == AttackType::ThirdAttack)
	{
		thirdAttackCount = 3;
		patternDelay = 0.5f;
		SetAttackScale(30.f);
	}

	if (attackType == AttackType::FireBall || attackType == AttackType::ThrowingKnife)
	{
		playerLastPos = player->GetPos();
		lastPos = GetPos();
		lastMoveType = moveType;
	}
}

void FireBoss::UpdateThirdAttack(float dt)
{
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
		RandomPatternSet(AttackType::ThirdAttack);
		PatternOptionSet();

		SetState(BossStates::Move);
	}
}

void FireBoss::UpdateFireball(float dt)
{
	if (nextPatternDelay <= 0.f)
	{
		SetState(BossStates::None);
		SetState(BossStates::Attack);
		if (patternDelay <= 0.f)
		{
			RandomPatternSet(AttackType::FireBall);
			PatternOptionSet();

			if (patternCount > 0)
				patternCount--;

			SetState(BossStates::Move);
		}
	}
}

void FireBoss::UpdateThrowingKnife(float dt)
{
	if (nextPatternDelay <= 0.f)
	{
		SetState(BossStates::None);
		SetState(BossStates::Attack);

		if (patternDelay <= 0.f)
		{
			RandomPatternSet(AttackType::ThrowingKnife);
			PatternOptionSet();

			if (patternCount > 0)
				patternCount--;
			
			SetState(BossStates::Move);
		}
	}
}

//void FireBoss::UpdateDragonAttack(float dt)
//{
//	if (nextPatternDelay <= 0.f)
//	{
//		SetState(BossStates::None);
//		SetState(BossStates::Attack);
//
//		if (patternDelay <= 0.f)
//		{
//			RandomPatternSet(AttackType::DragonAttack);
//			PatternOptionSet();
//
//			patternCount--;
//			SetState(BossStates::Move);
//		}
//	}
//}
//
//void FireBoss::UpdateMeteor(float dt)
//{
//	if (nextPatternDelay <= 0.f)
//	{
//		SetState(BossStates::None);
//		SetState(BossStates::Attack);
//
//		if (patternDelay <= 0.f)
//		{
//			RandomPatternSet(AttackType::Meteor);
//			PatternOptionSet();
//
//			patternCount--;
//			SetState(BossStates::Move);
//		}
//	}
//}
