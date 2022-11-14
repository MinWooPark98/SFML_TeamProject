#include "Enemy.h"

void Enemy::Init()
{
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

	if (curState == States::Attack || curState == States::MoveAttack)
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