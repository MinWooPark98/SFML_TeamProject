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

	SetState(States::RightIdle);

	arrow = new SpriteObj();
	arrow->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Arrow.png"));
	arrow->SetHitBox((FloatRect)arrow->GetTextureRect());
	arrow->SetOrigin(Origins::MC);
	SetArrowSpeed(400.f);
	arrowDir.setFillColor(Color::Red);
	arrowDir.setSize({2, 1080});

	SetPaletteIndex(44);
	SetpaletteSize(9);
	SetColorTable("graphics/ArcherColorIndex.png");
	SetColor(3);

	SetSpeed(200.f);
	SetMoveScale(500.f);
	SetAttackScale(400.f);
	SetAttackStartDelay(1.f);
	weapon->SetOrigin(Origins::MC);
}

void Archer::Update(float dt)
{
	Enemy::Update(dt);

	if (Utils::Distance(player->GetPos(), GetPos()) <= GetMoveScale() + 1.f && curState != States::Attack)
		NormalMonsterMove(dt);

	attackDelay -= dt;

	switch (curState)
	{
	case Archer::States::LeftIdle: case Archer::States::RightIdle:
		UpdateIdle();
		break;
	case Archer::States::LeftMove: case Archer::States::RightMove:
		UpdateMove(2.f);
		break;
	case Archer::States::Attack:
		UpdateAttack(dt);
		break;
	case Archer::States::Hit:
		SetState(States::Hit);
		break;
	case Archer::States::Die:
		SetState(States::Die);
		break;
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
	if (curState == States::Attack)
	{
		window.draw(weapon->GetSprite(), &shader);

		if (attackDelay >= attackStart)
			window.draw(arrowDir);

		arrow->Draw(window);
	}
	
	Object::Draw(window);
	window.draw(sprite, &shader);
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
		if (player->GetPos().x >= GetPos().x)
		{
			attackDelay = 2.f;
			return;
		}
		if (player->GetPos().x < GetPos().x)
		{
			attackDelay = 2.f;
			return;
		}
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
	if (attackDelay >= attackStart)
	{
		weapon->GetSprite().setRotation(Utils::Angle(GetPos(), player->GetPos()));
		arrow->GetSprite().setRotation(Utils::Angle(GetPos(), player->GetPos()) + 90);
		arrowDir.setRotation(Utils::Angle(GetPos(), player->GetPos()) - 90);
		arrow->SetPos(GetPos());
		weapon->SetPos(GetPos());
		arrowDir.setPosition(GetPos());

		if (player->GetPos().x >= GetPos().x)
			animation.Play("ArcherRightAttack");
		if (player->GetPos().x < GetPos().x)
			animation.Play("ArcherLeftAttack");

		playerLastPos = player->GetPos();
	}
	else
	{
		auto shot = Utils::Normalize(playerLastPos - arrow->GetPos());
		arrow->Translate({ dt * arrowSpeed * shot * 2.f});
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
		SetState(States::Attack);

		if (Utils::Distance(player->GetPos(), GetPos()) > GetAttackScale() &&
			Utils::Distance(player->GetPos(), GetPos()) < GetMoveScale())
		{
			if (lastDir.x < 0.f)
				SetState(States::LeftMove);
			if (lastDir.x > 0.f)
				SetState(States::RightMove);
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