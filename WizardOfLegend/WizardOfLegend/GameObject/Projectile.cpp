#include "Projectile.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"

Projectile::Projectile()
	:atkShape(AttackShape::None), animator(nullptr), isMoving(false), movingDuration(0.f), movingTimer(0.f), speed(0.f), skillReturn(false), attackDmg(0), dmgType(DamageType::Once), isOnDelay(false), atkDelay(0.f), atkTimer(0.f), angle(0.f), amplitude(0.f)
{
}

Projectile::~Projectile()
{
}

void Projectile::Init()
{
	SpriteObj::Init();
	animator = new Animator();
	animator->SetTarget(&sprite);
}

void Projectile::Reset()
{
	SpriteObj::Reset();
	isMoving = false;
	movingTimer = 0.f;
	atkTimer = 0.f;
	isOnDelay = false;
}

void Projectile::Update(float dt)
{
	if (!isMoving)
		return;
	SpriteObj::Update(dt);

	if (skillReturn)
		direction = -direction;

	movingTimer += dt;
	switch (atkShape)
	{
	case Projectile::AttackShape::Range:
		break;
	case Projectile::AttackShape::Rotate:
		break;
	case Projectile::AttackShape::Wave:
		{
			position = startPos + direction * speed * dt;
			Transform translate;
			translate.translate({ 0.f, amplitude * sin(angle) });
			Transform rotation;
			rotation.rotate(Utils::Angle(direction) + 90.f);
			transform = translate * rotation;
			transform.transformPoint(position);
			SetPos(position);
		}
		break;
	default:
		break;
	}

	if (movingTimer >= movingDuration)
	{
		isMoving = false;
		movingTimer = 0.f;
	}

	if (!isOnDelay)
	{
		// 충돌 검사, 공격
	}
	else if (dmgType != DamageType::Once)
	{
		atkTimer += dt;
		if (atkTimer >= atkDelay)
		{
			isOnDelay = false;
			atkTimer = 0.f;
		}
	}
}

void Projectile::SetAnimClip(const string& clipName)
{
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip(clipName));
	this->clipName = clipName;
}

void Projectile::Fire()
{
	isMoving = true;
	animator->Play(clipName);
}
