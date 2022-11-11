#include "Projectile.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"

Projectile::Projectile()
	:atkShape(AttackShape::None), animator(nullptr), isMoving(false), movingDuration(0.f), movingTimer(0.f), speed(0.f), moveType(MoveType::OneWay), attackDmg(0), dmgType(DamageType::Once), isOnDelay(false), atkDelay(0.f), atkTimer(0.f), angle(0.f), amplitude(0.f), frequency(0.f), reverse(false)
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
	angle = 0.f;
	amplitude = 0.f;
	frequency = 0.f;
	reverse = false;
}

void Projectile::Update(float dt)
{
	if (!isMoving)
		return;
	SpriteObj::Update(dt);

	if (moveType == MoveType::BackAndForth && movingTimer >= movingDuration * 0.5f)
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
			angle += speed * dt;
			sprite.setRotation(Utils::Angle(direction));
			Transform transform;
			frequency = 2.f;
			transform.rotate(Utils::Angle(direction)).translate({ angle, amplitude * (float)(sin((angle) * (M_PI / 180.f) * frequency)) * (reverse ? -1.f : 1.f) });
			sprite.setRotation(Utils::Angle(direction) + (90.f - fabs(((angle - (((int)(angle / (360.f / frequency))) * (360.f / frequency))) - (180.f / frequency)) * frequency)) * atan2(amplitude, 90.f) * (reverse ? 1.f : - 1.f) + (direction.x > 0.f ? 0.f : 180.f));
			position = transform.transformPoint({ 0.f, 0.f });
			SetPos(position + startPos);
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

void Projectile::SetAnimClip(const vector<string>& clipName)
{
	for (auto& name : clipName)
	{
		animator->AddClip(*RESOURCE_MGR->GetAnimationClip(name));
	}
	this->clipName = clipName;
}

void Projectile::Fire()
{
	isMoving = true;
	switch (atkShape)
	{
	case Projectile::AttackShape::Range:
		break;
	case Projectile::AttackShape::Rotate:
		break;
	case Projectile::AttackShape::Wave:
		if(direction.x < 0.f)
			animator->Play(clipName[0]);
		else
			animator->Play(clipName[1]);
		break;
	}
}
