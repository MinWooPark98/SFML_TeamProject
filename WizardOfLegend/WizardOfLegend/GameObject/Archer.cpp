#include "Archer.h"
#include "../Scene/SceneMgr.h"
#include "../Scene/PlayScene.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/StatTable.h"

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
	arrow->SetHitBox({ 20, 20, 5, 30 }, Color::Red);
	arrow->SetOrigin(Origins::MC);
	SetArrowSpeed(400.f);
	arrowDir.setFillColor(Color::Red);
	arrowDir.setSize({1, 300});


	archerAttackArm = new SpriteObj();
	archerAttackArm->SetTexture(*RESOURCE_MGR->GetTexture("graphics/ArcherAttackArm.png"));
	archerAttackArm->SetOrigin(Origins::MC);

	archerPullArm = new SpriteObj();
	archerPullArm->SetTexture(*RESOURCE_MGR->GetTexture("graphics/ArcherAttackPullArm0.png"));
	archerPullArm->SetOrigin(Origins::MC);

	archerPullArmAnimation.SetTarget(&archerPullArm->GetSprite());
	archerPullArmAnimation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherArm"));


	SetPaletteIndex(44);
	SetpaletteSize(9);
	SetColorTable("graphics/ArcherColorIndex.png");

	SetMoveScale(200.f);
	SetAttackScale(100.f);
	SetAttackStartDelay(1.f);
	SetMonsterType(MonsterType::Normal);
	weapon->SetOrigin(Origins::MC);
	spawn->SetPos(GetPos());
	SetCardColor(2);
	SetHitBox({ 20.f, 20.f, 10.f, 30.f }, Color::Red);
	hitbox.setOrigin(GetHitBox().getSize().x * 0.5f, GetHitBox().getSize().y * 0.5f);
	SetLowHitBox({ 20.f, 20.f, 10.f, 5.f }, Color::White);
	SetLowHitBoxOrigin(Origins::MC);

	auto statTable = DATATABLE_MGR->Get<StatTable>(DataTable::Types::Stat);
	auto& stat = statTable->Get("Archer");
	SetDamage(stat.attackDmg);
	SetMaxHp(stat.maxHp);
	SetSpeed(stat.speed);
	SetCurHp(maxHp);
}

void Archer::Update(float dt)
{
	Enemy::Update(dt);

	if (!isSpawn)
	{
		spawnAnimation.Play("MonsterSpawnCard");
		animation.Play("ArcherRightIdle");
		isSpawn = true;
	}
	else if (isActionStart)
	{
		if (Utils::Distance(player->GetPos(), GetPos()) <= GetMoveScale() + 1.f && curState != States::Attack && curState != States::MoveAttack)
		{
			NormalMonsterMove(dt);

			moveSoundTimer -= dt;
			if (moveSoundTimer <= 0.f)
			{
				SOUND_MGR->Play("sounds/LightFootstep.wav");
				moveSoundTimer = 0.4f;
			}
		}
		else
			moveSoundTimer = 0.f;

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

		if (attackDelay <= attackStart + 0.2f && attackDelay >= attackStart + 0.05f)
		{
			if ((int)((attackDelay - attackStart) / 0.03f) % 2 == 0)
				arrowDir.setFillColor({180, 180, 180});
			else
				arrowDir.setFillColor(Color::Red);
		}
		else
			arrowDir.setFillColor(Color::Red);

		arrow->GetHitBox().setPosition(arrow->GetPos());
		animation.Update(dt);
		archerPullArmAnimation.Update(dt);
		bowAnimation.Update(dt);
	}
}

void Archer::Draw(RenderWindow& window)
{
	Enemy::Draw(window);

	if (curState == States::Attack || curState == States::MoveAttack)
	{
		window.draw(archerAttackArm->GetSprite(), &shader);
	}

	if (isAlive)
		window.draw(sprite, &shader);
	else
	{
		if (dieTimer >= 0.f)
			window.draw(sprite, &shader);
	}

	if (curState == States::Attack || curState == States::MoveAttack)
	{
		window.draw(weapon->GetSprite(), &shader);

		if (attackDelay >= attackStart)
			window.draw(arrowDir);

		if (curState == States::Attack)
		{
			if (isDevMode)
				window.draw(arrow->GetHitBox());

			if (arrow->GetActive())
				arrow->Draw(window);
		}

		window.draw(archerPullArm->GetSprite(), &shader);
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

	if (attackDelay >= attackStart + 0.2f)
	{
		arrow->SetActive(true);

		weapon->GetSprite().setRotation(Utils::Angle(GetPos(), player->GetPos()));
		weapon->SetPos(GetPos() - Utils::Normalize((player->GetPos() - GetPos())) * -3.f);
		arrowDir.setPosition(GetPos());
		arrowDir.setRotation(Utils::Angle(GetPos(), player->GetPos()) - 90);

		if (curState == States::Attack)
		{
			arrow->GetSprite().setRotation(Utils::Angle(GetPos(), player->GetPos()) + 90);
			arrow->SetPos(weapon->GetPos() + Utils::Normalize((playerLastPos - GetPos())) * 8.f);
			arrow->GetHitBox().setRotation(arrow->GetSprite().getRotation());
		}

		if (player->GetPos().x >= GetPos().x)
			animation.Play("ArcherRightAttack");
		if (player->GetPos().x < GetPos().x)
			animation.Play("ArcherLeftAttack");

		if (type == MonsterType::Normal)
		{
			archerAttackArm->SetPos(weapon->GetPos());
			archerPullArm->SetPos(weapon->GetPos() + Utils::Normalize((playerLastPos - GetPos())) * -8.f);
		}

		archerAttackArm->GetSprite().setRotation(Utils::Angle(GetPos(), player->GetPos()));
		archerPullArm->GetSprite().setRotation(Utils::Angle(GetPos(), player->GetPos()));
		archerPullArmAnimation.Play("ArcherArm");
		playerLastPos = player->GetPos();

		isAttack = true;
	}
	else if (attackDelay <= attackStart)
	{
		if (curState == States::Attack)
		{
			auto shot = Utils::Normalize(playerLastPos - GetPos());
			arrow->Translate({ dt * arrowSpeed * shot * 2.f });
		}

		if (Utils::OBB(player->GetHitBox(), arrow->GetHitBox()))
		{
			if (isAttack)
			{
				player->SetCurHp(player->GetCurHp() - GetDamage());
				arrow->SetActive(false);
				isAttack = false;
			}
		}
	}


	if (attackDelay <= attackStart && bowWait)
	{
		bowAnimation.Play("BowShoot");
		SOUND_MGR->Play("sounds/ArcherAttackRelease.wav");
		SOUND_MGR->Play("sounds/ArcherArrow.wav");
		bowWait = false;
	}
	else if (attackDelay >= attackStart && !bowWait)
	{
		bowAnimation.Play("BowPull");
		SOUND_MGR->Play("sounds/ArcherAttackWindup.wav");
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

	auto& collisionList = ((PlayScene*)SCENE_MGR->GetCurrentScene())->GetCollisionList();
	
	for (int i = 0; i < collisionList.size(); i++)
	{
		if (collisionList[i][Object::ObjTypes::Player].empty())
			continue;

		for (auto& coll : collisionList[i][Object::ObjTypes::Wall])
		{
			if (arrow->GetHitBounds().intersects(coll->GetHitBounds()))
			{
				arrow->SetActive(false);
			}
		}
	}
}