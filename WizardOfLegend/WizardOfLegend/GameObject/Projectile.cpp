#include "Projectile.h"

Projectile::Projectile()
	:isMoving(false), movingDuration(0.f), movingTimer(0.f), speed(0.f)
{
}

Projectile::~Projectile()
{
}

void Projectile::Reset()
{
	SpriteObj::Init();
	movingTimer = 0.f;
}

void Projectile::Update(float dt)
{
	SpriteObj::Update(dt);
	if (isMoving)
	{
		Translate(direction * speed * dt);
		movingTimer += dt;
		if (movingTimer >= movingDuration)
			isMoving = false;
	}
}
