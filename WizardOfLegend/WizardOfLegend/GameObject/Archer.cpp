#include "Archer.h"

Archer::Archer()
	: bowDir(0, 0)
{
}

void Archer::Init()
{
	Enemy::Init();

	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherLeftRun"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherRightRun"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherLeftAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherRightAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherLeftDie"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherRightDie"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherLeftHit"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherRightHit"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherLeftIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherRightIdle"));


	SetWeaponImage("graphics/ArcherAttackRelease3.png");
	bowAnimation.SetTarget(&weapon->GetSprite());
	bowAnimation.AddClip(*RESOURCE_MGR->GetAnimationClip("BowPull"));
	bowAnimation.AddClip(*RESOURCE_MGR->GetAnimationClip("BowShoot"));

	arrow = new SpriteObj();
	arrow->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Arrow.png"));
	arrow->SetHitBox((FloatRect)arrow->GetTextureRect());
	arrow->SetOrigin(Origins::MC);
	SetArrowSpeed(400.f);
	arrowDir.setFillColor(Color::Red);
	arrowDir.setOrigin({});
	arrowDir.setSize({2, 1080});

	SetPaletteIndex(44);
	SetpaletteSize(9);
	SetColorTable("graphics/ArcherColorIndex.png");

	SetSpeed(200.f);
	SetMoveScale(500.f);
	SetAttackScale(400.f);
	SetAttackStartDelay(1.f);
	SetMonsterType(MonsterType::Normal);
	SetMaxHp(1);
	SetCurHp(GetMaxHp());
	weapon->SetOrigin(Origins::MC);
}

void Archer::Update(float dt)
{
	Enemy::Update(dt);

	if (Utils::Distance(player->GetPos(), GetPos()) <= GetMoveScale() + 1.f && curState != States::Attack && curState != States::MoveAttack)
		NormalMonsterMove(dt);

	if (InputMgr::GetKeyDown(Keyboard::Key::L))
		SetCurHp(0);

	if (curHp <= 0 && isAlive)
	{
		dieTimer = 1.f;
		SetState(States::Die);
		isAlive = false;
	}

	if (!Utils::EqualFloat(direction.x, 0.f))
	{
		lastDir = direction;
	}

	animation.Update(dt);
	bowAnimation.Update(dt);
}

void Archer::Draw(RenderWindow& window)
{
	if (curState == States::Attack || curState == States::MoveAttack)
	{
		window.draw(weapon->GetSprite(), &shader);

		if (attackDelay >= attackStart)
			window.draw(arrowDir);

		if (curState == States::Attack)
			arrow->Draw(window);
	}

	Object::Draw(window);

	if (isAlive)
		window.draw(sprite, &shader);
	else
	{
		if (dieTimer >= 0.f)
			window.draw(sprite, &shader);
	}
}

void Archer::SetState(States newState)
{
	if (curState == newState)
		return;

	curState = newState;

	switch (curState)
	{
	case Archer::States::LeftIdle:
		animation.Play("ArcherLeftIdle");
		break;
	case Archer::States::RightIdle:
		animation.Play("ArcherRightIdle");
		break;
	case Archer::States::LeftMove:
		animation.Play("ArcherLeftRun");
		break;
	case Archer::States::RightMove:
		animation.Play("ArcherRightRun");
		break;
	case Archer::States::Attack:
		attackDelay = 2.f;
		break;
	case Archer::States::Hit:
		lastDir.x < 0.f ? animation.Play("ArcherLeftHit") : animation.Play("ArcherRightHit");
		break;
	case Archer::States::Die:
		lastDir.x < 0.f ? animation.Play("ArcherLeftDie") : animation.Play("ArcherRightDie");
		break;
	}
}

void Archer::UpdateAttack(float dt)
{
	if (curHp <= 0)
		return;

	if (attackDelay >= attackStart)
	{
		weapon->GetSprite().setRotation(Utils::Angle(GetPos(), player->GetPos()));
		weapon->SetPos(GetPos());
		arrowDir.setPosition(GetPos());
		arrowDir.setRotation(Utils::Angle(GetPos(), player->GetPos()) - 90);

		if (curState == States::Attack)
		{
			arrow->GetSprite().setRotation(Utils::Angle(GetPos(), player->GetPos()) + 90);
			arrow->SetPos(GetPos());
		}

		if (player->GetPos().x >= GetPos().x)
			animation.Play("ArcherRightAttack");
		if (player->GetPos().x < GetPos().x)
			animation.Play("ArcherLeftAttack");

		playerLastPos = player->GetPos();
	}
	else
	{
		if (curState == States::Attack)
		{
			auto shot = Utils::Normalize(playerLastPos - GetPos());
			arrow->Translate({ dt * arrowSpeed * shot * 2.f });
			// 히트박스 구현되면 화살 멈추게 해야함
		}
	}


	if (attackDelay <= attackStart && bowWait)
	{
		bowAnimation.Play("BowShoot");
		bowWait = false;
	}
	else if (attackDelay >= attackStart && !bowWait)
	{
		bowAnimation.Play("BowPull");
		bowWait = true;
	}


	if (attackDelay <= 0.f)
	{
		SetState(States::None);

		if (type == MonsterType::Normal)
			SetState(States::Attack);

		if (Utils::Distance(player->GetPos(), GetPos()) > GetAttackScale() &&
			Utils::Distance(player->GetPos(), GetPos()) < GetMoveScale())
		{
			if (lastDir.x < 0.f)
			{
				SetState(States::LeftMove);
				return;
			}
			if (lastDir.x > 0.f)
			{
				SetState(States::RightMove);
				return;
			}
		}

		if (Utils::Distance(player->GetPos(), GetPos()) >= GetMoveScale())
		{
			direction.x = 0;

			if (lastDir.x < 0.f)
				SetState(States::LeftIdle);
			if (lastDir.x > 0.f)
				SetState(States::RightIdle);

			return;
		}
	}
}