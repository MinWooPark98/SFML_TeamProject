#include "Lancer.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/StatTable.h"
#include "../Scene/PlayScene.h"
#include "../Scene/SceneMgr.h"
#include "Gold.h"
#include "ChaosFragments.h"
#include "../DataTable/PropertyTable.h"

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
	SetMoveScale(700.f);
	SetAttackScale(100.f);
	SetMonsterType(MonsterType::Normal);
	weapon->SetOrigin(Origins::MC);
	weapon->SetHitBox({ 20, 20, 5, 35 }, Color::Red);
	weapon->GetHitBox().setOrigin(weapon->GetHitBox().getSize().x * 0.5f, weapon->GetHitBox().getSize().y * 0.5f);
	spawn->SetPos(GetPos());
	SetCardColor(2);

	auto statTable = DATATABLE_MGR->Get<StatTable>(DataTable::Types::Stat);
	auto& stat = statTable->Get("Lancer");
	SetDamage(stat.attackDmg);
	SetMaxHp(stat.maxHp);
	SetSpeed(stat.speed);
	SetCurHp(GetMaxHp());

	SetHitBox({ 20.f, 20.f, 20.f, 30.f }, Color::Red);
	hitbox.setOrigin(GetHitBox().getSize().x * 0.5f, GetHitBox().getSize().y * 0.5f);
	SetLowHitBox({ 20.f, 20.f, 15.f, 5.f }, Color::White);
	SetLowHitBoxOrigin(Origins::MC);

	auto propertyTable = DATATABLE_MGR->Get<PropertyTable>(DataTable::Types::MonsterProperty);
	auto& p = propertyTable->Get("Lancer");
	SetProperty(p.goldProbability, p.goldDropNumber, p.minGold, p.maxGold, p.platinumProbability, p.platinumDropNumber);
}

void Lancer::Update(float dt)
{
	Enemy::Update(dt);
	if (isAlive)
	{
		if (!isSpawn)
		{
			spawnAnimation.Play("MonsterSpawnCard");
			animation.Play("LancerRightIdle");
			isSpawn = true;
		}
		else if (isActionStart)
		{
			if (curState != States::Hit)
			{
				if (Utils::Distance(player->GetPos(), GetPos()) <= GetMoveScale() + 1.f && curState != States::Attack)
				{
					NormalMonsterMove(dt);

					moveSoundTimer -= dt;
					if (moveSoundTimer <= 0.f)
					{
						SOUND_MGR->Play("sounds/MetalFootstep.wav");
						moveSoundTimer = 0.4f;
					}
				}

				spearAnimation.Update(dt);
				if (spearAnimation.GetCurrentFrame() == 6)
					lancerAttackEffect->SetActive(false);
			}
			else
			{
				attackDelay = 2.f;
				spearWait = false;
			}

			if (curHp <= 0 && isAlive)
			{
				dieTimer = 1.f;
				SetState(States::Die);

				PlayScene* playScene = (PlayScene*)SCENE_MGR->GetCurrentScene();
				Drop(playScene);

				isAlive = false;
			}

			if (!Utils::EqualFloat(direction.x, 0.f))
			{
				lastDir = direction;
			}
		}
	}
	animation.Update(dt);

	Scene* currScene = SCENE_MGR->GetCurrentScene();
	if (currScene->GetType() != Scenes::Play || curState == States::Fall || curState == States::Hit)
		return;
	vector<map<Object::ObjTypes, list<Object*>>>& collisionList = ((PlayScene*)currScene)->GetCollisionList();
	for (int i = 0; i < collisionList.size(); ++i)
	{
		if (collisionList[i][Object::ObjTypes::Player].empty())
			continue;
		for (auto& cliff : collisionList[i][Object::ObjTypes::Cliff])
		{
			if (cliff->GetHitBounds().intersects(GetLowHitBounds()))
			{
				SetPos(lastPosition);
			}
		}
	}
}

void Lancer::Draw(RenderWindow& window)
{
	if (curState == States::Attack)
	{
		window.draw(weapon->GetSprite(), &shader);

		if (lancerAttackEffect->GetActive())
		{
			if (attackDelay <= 1.f)
				window.draw(lancerAttackEffect->GetSprite(), &shader);
		}

		if (isDevMode && lancerAttackEffect->GetActive())
		{
			window.draw(lancerAttackEffect->GetHitBox());
			window.draw(weapon->GetHitBox());
		}
	}

	Enemy::Draw(window);

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

	float angle = 0.f;

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
		weapon->GetHitBox().setRotation(Utils::Angle(GetPos(), player->GetPos()) + 90);
		lancerAttackEffect->GetSprite().setRotation(Utils::Angle(GetPos(), player->GetPos()) + 90);
		lancerAttackEffect->GetHitBox().setRotation(lancerAttackEffect->GetSprite().getRotation());

		angle = Utils::Angle(player->GetPos(), GetPos());
		attackDelay = 2.f;

		if (angle >= -180 && angle <= -130 || angle >= 130 && angle <= 180)
		{
			animation.Play("LancerRightAttack");
			spearPos = 1;
		}
		if (angle <= -45 && angle >= -129)
		{
			animation.Play("LancerDownAttack");
			spearPos = 4;
		}
		if (angle >= -44 && angle <= 45)
		{
			animation.Play("LancerLeftAttack");
			spearPos = 2;
		}
		if (angle >= 46 && angle <= 129)
		{
			animation.Play("LancerUpAttack");
			spearPos = 3;
		}
		break;
	case States::Hit:
		lastDir.x < 0.f ? animation.Play("LancerLeftHit") : animation.Play("LancerRightHit");
		break;
	case States::Die:
		lastDir.x < 0.f ? animation.Play("LancerLeftDie") : animation.Play("LancerRightDie");
		SOUND_MGR->Play("sounds/EnemyDead.wav");
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
		weapon->GetHitBox().setPosition(weapon->GetPos());
		switch (spearPos)
		{
		case 1: case 2:
			weapon->SetPos({ weapon->GetPos().x, weapon->GetPos().y - 5.f }); // �¿�
			weapon->GetHitBox().setPosition(weapon->GetPos());
			break;
		case 3: case 4:
			weapon->SetPos({ weapon->GetPos().x + 5.f, weapon->GetPos().y - 5.f }); // ����
			weapon->GetHitBox().setPosition(weapon->GetPos());
			break;
		}

		auto lanceDir = Utils::Normalize(playerLastPos - GetPos());
		lancerAttackEffect->SetPos(weapon->GetPos() + lanceDir * 50.f);
		lancerAttackEffect->GetHitBox().setPosition(lancerAttackEffect->GetPos());
		lancerAttackEffect->SetActive(true);
		spearAnimation.Play("SpearMotion");
		SOUND_MGR->Play("sounds/KnightAttack.wav");

		auto& collisionList = ((PlayScene*)SCENE_MGR->GetCurrentScene())->GetCollisionList();
		for (int i = 0; i < collisionList.size(); i++)
		{
			if (collisionList[i][Object::ObjTypes::Player].empty())
				continue;

			for (auto& coll : collisionList[i][Object::ObjTypes::Wall])
			{
				if (Utils::OBB(weapon->GetHitBox(), coll->GetHitBox()))
				{
					lancerAttackEffect->SetActive(false);
				}
			}
		}

		if (lancerAttackEffect->GetActive())
		{
			if (Utils::OBB(player->GetHitBox(), lancerAttackEffect->GetHitBox()) ||
				Utils::OBB(player->GetHitBox(), weapon->GetHitBox()))
			{
				player->OnHit(lanceDir, GetDamage());
			}
		}

		spearWait = false;
	}
	else if (attackDelay >= 1.f && !spearWait)
	{
		lancerAttackEffect->SetActive(false);
		weapon->SetTexture(*RESOURCE_MGR->GetTexture("graphics/LancerSpear.png"));
		weapon->SetPos({ GetPos().x, GetPos().y });
		weapon->GetHitBox().setPosition(weapon->GetPos());
		if (spearPos == 3 || spearPos == 4)
			weapon->SetPos({ weapon->GetPos().x + 7.f, weapon->GetPos().y });
		playerLastPos = player->GetPos();
		SOUND_MGR->Play("sounds/LancerAttackWindup.wav");
		spearWait = true;
		lancerAttackEffect->SetActive(true);
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

void Lancer::Reset()
{
	Enemy::Reset();
	spearWait = false;
	spearPos = 0;
	auto statTable = DATATABLE_MGR->Get<StatTable>(DataTable::Types::Stat);
	auto& stat = statTable->Get("Lancer");
	SetDamage(stat.attackDmg);
	SetMaxHp(stat.maxHp);
	SetSpeed(stat.speed);
	SetCurHp(GetMaxHp());
}