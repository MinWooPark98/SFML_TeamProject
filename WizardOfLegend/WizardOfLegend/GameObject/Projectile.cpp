#include "Projectile.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"

Projectile::Projectile()
	:atkShape(AttackShape::None), animator(nullptr), isMoving(false), movingDuration(0.f), movingTimer(0.f), speed(0.f), moveType(MoveType::OneWay), isComingBack(false),  attackDmg(0), dmgType(DamageType::Once), isOnDelay(true), delay(0.f), timer(0.f), isOnAtkDelay(false), atkDelay(0.f), atkTimer(0.f), distance(0.f), angle(0.f), amplitude(0.f), frequency(0.f), reverse(false),vecIdx(0)
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
	isOnDelay = true;
	timer = 0.f;
	isOnAtkDelay = false;
	angle = 0.f;
	isComingBack = false;
	amplitude = 0.f;
	frequency = 0.f;
	reverse = false;
}

void Projectile::Update(float dt)
{
	if (!isMoving)
		return;

	if (isOnDelay)
	{
		timer += dt;
		if (timer >= delay)
		{
			isOnDelay = false;
			Fire();
		}
		else
			return;
	}

	animator->Update(dt);
	SpriteObj::Update(dt);

	movingTimer += dt;
	switch (atkShape)
	{
	case Projectile::AttackShape::Rotate:
		{
			angle += speed * dt;
			SetPos({ startPos.x + distance * (float)cos((angle) * (M_PI / 180.f)), startPos.y + distance * (float)sin((angle) * (M_PI / 180.f)) });
		}
		break;
	case Projectile::AttackShape::Wave:
		{
			if (moveType == MoveType::BackAndForth && !isComingBack && movingTimer >= movingDuration * 0.5f)
			{
				isComingBack = true;
				animator->Play(clipName[1 - vecIdx]);
				vecIdx = 1 - vecIdx;
			}

			if (!isComingBack)
				angle += speed * dt;
			else
				angle -= speed * dt;
			sprite.setRotation(Utils::Angle(direction));
			Transform transform;
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

	if (!isOnAtkDelay)
	{
		// �浹 �˻�, ����
	}
	else if (dmgType != DamageType::Once)
	{
		atkTimer += dt;
		if (atkTimer >= atkDelay)
		{
			isOnAtkDelay = false;
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
	if (atkShape == Projectile::AttackShape::Rotate)
		vecIdx = speed > 0.f ? 0 : 1;
	else
		vecIdx = direction.x < 0.f ? 0 : 1;
	animator->Play(clipName[vecIdx]);
}
