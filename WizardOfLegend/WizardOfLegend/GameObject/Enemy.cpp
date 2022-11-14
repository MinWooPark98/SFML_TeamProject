#include "Enemy.h"

void Enemy::Init()
{
	animation.SetTarget(&sprite);
	position = { 0, 0 };
	shader.loadFromFile("shaders/palette.frag", Shader::Fragment);
	weapon = new SpriteObj();
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

	if (!Utils::EqualFloat(direction.x, 0.f))
	{
		auto move = Utils::Normalize(player->GetPos() - GetPos());
		Translate({ dt * speed * move });

		if (lastDir.x < 0.f)
			SetState(States::LeftMove);
		if (lastDir.x > 0.f)
			SetState(States::RightMove);
	}

	if (Utils::EqualFloat(direction.x, 0.f))
	{
		if (lastDir.x < 0.f)
			SetState(States::LeftIdle);
		if (lastDir.x > 0.f)
			SetState(States::RightIdle);
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
	if (Utils::Distance(player->GetPos(), GetPos()) <= GetAttackScale())
	{
		SetState(States::Attack);
		this->attackDelay = attackDelay;
	}
}