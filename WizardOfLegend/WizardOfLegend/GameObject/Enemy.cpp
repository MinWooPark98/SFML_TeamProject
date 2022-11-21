#include "Enemy.h"

void Enemy::Init()
{
	Release();
	animation.SetTarget(&sprite);
	position = { 0, 0 };
	shader.loadFromFile("shaders/palette.frag", Shader::Fragment);
	weapon = new SpriteObj();
	type = MonsterType::None;
	curState = States::RightIdle;
}

void Enemy::Update(float dt)
{ 
	SpriteObj::Update(dt);

	if (type == MonsterType::Normal || type == MonsterType::StageBoss)
	{
		switch (curState)
		{
		case States::LeftIdle: case States::RightIdle:
			UpdateIdle();
			break;
		case States::LeftMove: case States::RightMove:
			UpdateMove(2.f);
			break;
		case States::Attack: case States::MoveAttack:
			UpdateAttack(dt);
			break;
		case States::Hit:
			SetState(States::Hit);
			break;
		case States::Die:
			SetState(States::Die);
			break;
		}
	}
	else if (type == MonsterType::MiddleBoss)
	{
		switch (curBossState)
		{
		case Enemy::BossStates::Idle:
			break;
		case Enemy::BossStates::Move:
			UpdateMove(0.5f);
			break;
		case Enemy::BossStates::Attack:
			UpdateAttack(dt);
			break;
		case Enemy::BossStates::Hit:
			SetState(BossStates::Hit);
			break;
		case Enemy::BossStates::Die:
			SetState(BossStates::Die);
			break;
		case Enemy::BossStates::Clear:
			SetState(BossStates::Clear);
			break;
		}
	}


	if (curState == States::Attack || curState == States::MoveAttack || curBossState == BossStates::Attack)
		attackDelay -= dt;

	if (curState == States::Die && dieTimer >= 0.f)
		dieTimer -= dt;
}

void Enemy::SetColor(int index)
{
	paletteIndex = (paletteIndex - index) % paletteSize;
	shader.setUniform("colorTable", texColorTable);
	shader.setUniform("paletteIndex", (float)paletteIndex / paletteSize);
}

void Enemy::NormalMonsterMove(float dt)
{
	if (Utils::Distance(player->GetPos(), GetPos()) <= GetMoveScale())
	{
		player->GetPos().x > GetPos().x ? direction.x = 1 : direction.x = -1;
		player->GetPos().y > GetPos().y ? direction.y = 1 : direction.y = -1;
	}
	else
		direction.x = 0;

	if (curState == States::Die)
		return;

	if (type == MonsterType::Normal || type == MonsterType::StageBoss)
	{
		if (!Utils::EqualFloat(direction.x, 0.f))
		{
			auto move = Utils::Normalize(player->GetPos() - GetPos());
			Translate({ dt * speed * move });

			if (lastDir.x < 0.f)
				SetState(States::LeftMove);
			if (lastDir.x > 0.f)
				SetState(States::RightMove);

			return;
		}

		if (Utils::EqualFloat(direction.x, 0.f))
		{
			if (lastDir.x < 0.f)
				SetState(States::LeftIdle);
			if (lastDir.x > 0.f)
				SetState(States::RightIdle);

			return;
		}
	}
}

void Enemy::BossMonsterMove(float dt)
{
	player->GetPos().x > GetPos().x ? direction.x = 1 : direction.x = -1;
	player->GetPos().y > GetPos().y ? direction.y = 1 : direction.y = -1;

	if (curBossState == BossStates::Die || curBossState == BossStates::Clear)
		return;

	if (!Utils::EqualFloat(direction.x, 0.f))
	{
		auto move = Utils::Normalize(player->GetPos() - GetPos());
		Translate({ dt * speed * move });
		SetState(BossStates::None);
		SetState(BossStates::Move);
		return;
	}
}

void Enemy::UpdateIdle()
{
	if (!Utils::EqualFloat(direction.x, 0.f))
	{
		if (lastDir.x > 0.f)
			SetState(States::LeftMove);
		if (lastDir.x < 0.f)
			SetState(States::RightMove);
		return;
	}
}

void Enemy::UpdateMove(int attackDelay)
{
	if (Utils::Distance(player->GetPos(), GetPos()) <= GetAttackScale() && type == MonsterType::Normal)
	{
		SetState(States::Attack);
		this->attackDelay = attackDelay;
		return;
	}

	if (type == MonsterType::StageBoss)	
	{
		if (normalAttackCount <= 0)
		{
			if (Utils::Distance(player->GetPos(), GetPos()) <= GetEscapeScale())
				SetState(States::MoveAttack);

			else if (Utils::Distance(player->GetPos(), GetPos()) <= GetMoveAttackScale())
				SetState(States::MoveAttack);

			else if (Utils::Distance(player->GetPos(), GetPos()) <= GetAttackScale())
				SetState(States::Attack);
		}
		else
		{
			SetState(States::Attack);
		}

		this->attackDelay = attackDelay;
		return;
	}
}