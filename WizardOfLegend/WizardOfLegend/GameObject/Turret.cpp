#include "Turret.h"
#include "Skill.h"
#include "../Framework/Framework.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/StatTable.h"
#include "../Scene/PlayScene.h"
#include "../Scene/SceneMgr.h"
#include "../Ui/Gold.h"

Turret::Turret()
{
}

Turret::~Turret()
{
}

void Turret::Init()
{
	Enemy::Init();
	SetTexture(*RESOURCE_MGR->GetTexture("graphics/TurretBase.png"));
	SetOrigin(Origins::MC);

	turretEye = new SpriteObj();
	turretEye->SetTexture(*RESOURCE_MGR->GetTexture("graphics/TurretEyeOpenInner.png"));
	turretEye->SetOrigin(Origins::MC);

	turretIris = new SpriteObj();
	turretIris->SetTexture(*RESOURCE_MGR->GetTexture("graphics/TurretIris.png"));
	turretIris->SetOrigin(Origins::MC);

	turretEyeWary = new SpriteObj();
	turretEyeWary->SetTexture(*RESOURCE_MGR->GetTexture("graphics/TurretEyeHalfClosed.png"));
	turretEyeWary->SetOrigin(Origins::MC);

	auto statTable = DATATABLE_MGR->Get<StatTable>(DataTable::Types::Stat);
	auto& stat = statTable->Get("Turret");
	SetDamage(stat.attackDmg);
	SetMaxHp(stat.maxHp);
	SetSpeed(stat.speed);
	SetCurHp(maxHp);

	newSkill = new Skill();
	newSkill->SetSubject(this, Skill::SubjectType::Enemy);
	newSkill->SetSkill("TurretAttack");

	SetHitBox({ 20.f, 20.f, 10.f, 20.f }, Color::Red);
	hitbox.setOrigin(GetHitBox().getSize().x * 0.5f, GetHitBox().getSize().y * 0.5f);
	SetLowHitBox({ 20.f, 20.f, 10.f, 5.f }, Color::White);
	SetLowHitBoxOrigin(Origins::MC);

	SetAttackScale(200.f);
	SetMoveScale(200.f);

	SetState(States::LeftIdle);
	SetMonsterType(MonsterType::Normal);
	isActionStart = true;
	sprite.setScale({1, 1});
}

void Turret::Reset()
{
	Enemy::Reset();
}

void Turret::Update(float dt)
{
	Enemy::Update(dt);

	if (Utils::Distance(player->GetPos(), GetPos()) <= GetMoveScale() + 1.f && curState != States::Attack)
	{
		NormalMonsterMove(dt);
	}

	if (curHp <= 0 && isAlive)
	{
		dieTimer = 1.f;
		SetState(States::Die);

		PlayScene* playScene = (PlayScene*)SCENE_MGR->GetCurrentScene();
		auto gold = playScene->GetGold()->Get();
		gold->SetGoldPos(GetPos());

		SOUND_MGR->Play("sounds/GoldSpawn.wav");

		isAlive = false;
	}

	if (Utils::Distance(player->GetPos(), GetPos()) <= GetAttackScale())
		EyeMove(dt);
	else
		EyeReset(dt);

	newSkill->Update(dt);
}

void Turret::Draw(RenderWindow& window)
{
	Enemy::Draw(window);

	if (isAlive)
	{
		window.draw(sprite);
		turretEye->Draw(window);
		turretIris->Draw(window);
		turretEyeWary->Draw(window);
		newSkill->Draw(window);
	}
}

void Turret::SetState(States newState)
{
	if (curState == newState)
		return;

	curState = newState;
}

void Turret::UpdateAttack(float dt)
{
	if (Utils::Distance(player->GetPos(), GetPos()) <= GetAttackScale())
	{
		attackTimer += dt;

		if (attackTimer >= attackStartTimer)
		{
			newSkill->SetSkillDir(Utils::Normalize(playerLastPos - GetPos()));
			newSkill->Reprepare();
			newSkill->Do();
			attackTimer = 0.f;
		}
		else
		{
			playerLastPos = player->GetPos();
		}
	}
	else
	{
		SetState(States::LeftMove);
		attackTimer = 0.f;
	}
}

void Turret::EyePos(const Vector2f& pos)
{
	turretEye->SetPos({ pos.x, pos.y - 8.f });
	turretIris->SetPos({ pos.x, pos.y - 8.f });
	turretEyeWary->SetPos({ pos.x, pos.y - 8.f });
}

void Turret::EyeMove(float dt)
{
	if (Utils::Distance(turretIris->GetPos(), turretEye->GetPos()) < 2.f)
	{
		auto move = Utils::Normalize(player->GetPos() - turretIris->GetPos());
		turretIris->Translate(dt * move * eyeMoveSpeed);
	}
	else
	{
		auto move = Utils::Normalize(turretEye->GetPos() - turretIris->GetPos());
		turretIris->Translate((dt * move * eyeMoveSpeed));
	}
}

void Turret::EyeReset(float dt)
{
	auto move = Utils::Normalize(turretEye->GetPos() - turretIris->GetPos());
	turretIris->Translate(dt * move * eyeMoveSpeed);
}