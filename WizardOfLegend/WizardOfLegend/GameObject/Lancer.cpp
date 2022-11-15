#include "Lancer.h"


void Lancer::Init()
{
	Enemy::Init();

	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerLeftMove"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerRightMove"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerLeftAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerRightAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerLeftDie"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerRightDie"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerDownAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerUpAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerLeftHit"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerRightHit"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerLeftIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerRightIdle"));


	SetWeaponImage("graphics/LancerSpear.png");
	SpriteObj tempSpearImage;
	tempSpearImage.SetTexture(*RESOURCE_MGR->GetTexture("graphics/LancerAttackEffect2.png"));
	lancerAttackEffect = new SpriteObj();
	lancerAttackEffect->SetHitBox((FloatRect)tempSpearImage.GetTextureRect());
	lancerAttackEffect->SetOrigin(Origins::MC);
	spearAnimation.SetTarget(&lancerAttackEffect->GetSprite());
	spearAnimation.AddClip(*RESOURCE_MGR->GetAnimationClip("SpearMotion"));
	

	SetPaletteIndex(59);
	SetpaletteSize(9);
	SetColorTable("graphics/LancerColorIndex.png");
	SetColor(3);
	SetSpeed(200.f);
	SetMoveScale(500.f);
	SetAttackScale(150.f);
	SetMonsterType(MonsterType::Normal);
	SetMaxHp(1);
	SetCurHp(GetMaxHp());
	weapon->SetOrigin(Origins::MC);
}

void Lancer::Update(float dt)
{
	Enemy::Update(dt);
	
	if (Utils::Distance(player->GetPos(), GetPos()) <= GetMoveScale() + 1.f && curState != States::Attack)
		NormalMonsterMove(dt);

	if (InputMgr::GetKeyDown(Keyboard::Key::K))
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
	spearAnimation.Update(dt);
}

void Lancer::Draw(RenderWindow& window)
{
	if (curState == States::Attack)
	{
		window.draw(weapon->GetSprite(), &shader);

		if (attackDelay <= 1.f)
			window.draw(lancerAttackEffect->GetSprite(), &shader);
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

void Lancer::SetState(States newState)
{
	if (curState == newState)
		return;

	curState = newState;

	switch (curState)
	{
	case States::LeftIdle:
		animation.Play("LancerLeftIdle");
		break;
	case States::RightIdle:
		animation.Play("LancerRightIdle");
		break;
	case States::LeftMove:
		animation.Play("LancerLeftMove");
		break;
	case States::RightMove:
		animation.Play("LancerRightMove");
		break;
	case States::Attack:
		weapon->GetSprite().setRotation(Utils::Angle(GetPos(), player->GetPos()) + 90);
		lancerAttackEffect->GetSprite().setRotation(Utils::Angle(GetPos(), player->GetPos()) + 90);

		if (Utils::Angle(player->GetPos(), GetPos()) >= -180 &&
			Utils::Angle(player->GetPos(), GetPos()) <= -130 ||
			Utils::Angle(player->GetPos(), GetPos()) >= 130 &&
			Utils::Angle(player->GetPos(), GetPos()) <= 180)
		{
			animation.Play("LancerRightAttack");
			spearPos = 1;
			attackDelay = 2.f;
			return;
		}
		if (Utils::Angle(player->GetPos(), GetPos()) <= -45 &&
			Utils::Angle(player->GetPos(), GetPos()) >= -129)
		{
			animation.Play("LancerDownAttack");
			spearPos = 4;
			attackDelay = 2.f;
			return;
		}
		if (Utils::Angle(player->GetPos(), GetPos()) >= -44 &&
			Utils::Angle(player->GetPos(), GetPos()) <= 45)
		{
			animation.Play("LancerLeftAttack");
			spearPos = 2;
			attackDelay = 2.f;
			return;
		}
		if (Utils::Angle(player->GetPos(), GetPos()) >= 46 &&
			Utils::Angle(player->GetPos(), GetPos()) <= 129)
		{
			animation.Play("LancerUpAttack");
			spearPos = 3;
			attackDelay = 2.f;
			return;
		}
		break;
	case States::Hit:
		lastDir.x < 0.f ? animation.Play("LancerLeftHit") : animation.Play("LancerRightHit");
		break;
	case States::Die:
		lastDir.x < 0.f ? animation.Play("LancerLeftDie") : animation.Play("LancerRightDie");
		break;
	}
}

void Lancer::UpdateAttack(float dt)
{
	if (curHp <= 0)
		return;

	if (attackDelay <= 1.f && spearWait)
	{
		weapon->SetTexture(*RESOURCE_MGR->GetTexture("graphics/LancerSpearWithArm.png"));
		weapon->SetPos(GetPos() + Utils::Normalize((playerLastPos - GetPos())) * 20.f);
		switch (spearPos)
		{
		case 1: case 2:
			weapon->SetPos({ weapon->GetPos().x, weapon->GetPos().y - 5.f}); // �¿�
			break;
		case 3: case 4:
			weapon->SetPos({ weapon->GetPos().x + 5.f, weapon->GetPos().y - 5.f }); // ����
			break;
		}

		lancerAttackEffect->SetPos(weapon->GetPos() + Utils::Normalize((playerLastPos - GetPos())) * 100.f);
		spearAnimation.Play("SpearMotion");
		spearWait = false;
	}
	else if (attackDelay >= 1.f && !spearWait)
	{
		weapon->SetTexture(*RESOURCE_MGR->GetTexture("graphics/LancerSpear.png"));
		weapon->SetPos({ GetPos().x, GetPos().y });
		if (spearPos == 3 || spearPos == 4)
			weapon->SetPos({ weapon->GetPos().x + 7.f, weapon->GetPos().y });
		playerLastPos = player->GetPos();
		spearWait = true;
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