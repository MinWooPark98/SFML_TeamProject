#include "Projectile.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"

Projectile::Projectile()
	:atkShape(Skill::AttackShape::None), animator(nullptr), isMoving(false), movingDuration(0.f), movingTimer(0.f), speed(0.f), waveType(Skill::WaveType::None), fallingHeight(0.f), cumulativeFallingHeight(0.f), rangeType(Skill::RangeType::None), isComingBack(false),  attackDmg(0), dmgType(Skill::DamageType::Once), isOnDelay(true), delay(0.f), timer(0.f), isOnAtkDelay(false), atkDelay(0.f), atkTimer(0.f), distance(0.f), angle(0.f), amplitude(0.f), frequency(0.f), reverse(false),vecIdx(0), subType(Skill::SubjectType::None)
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
	if (animator != nullptr)
		animator->ClearClips();
	clipName.clear();
	isMoving = false;
	movingTimer = 0.f;
	atkTimer = 0.f;
	isOnDelay = true;
	timer = 0.f;
	isOnAtkDelay = false;
	cumulativeFallingHeight = 0.f;
	angle = 0.f;
	isComingBack = false;
	amplitude = 0.f;
	frequency = 0.f;
	reverse = false;
	subType = Skill::SubjectType::None;
	sprite.setRotation(0.f);
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

	auto spriteBnd = sprite.getGlobalBounds();
	SetHitBox(spriteBnd);

	movingTimer += dt;
	switch (atkShape)
	{
	case Skill::AttackShape::Range:
		{
			cumulativeFallingHeight += fallingHeight * (dt / movingDuration);
			auto& currOrigin = animator->GetFrame().origin;
			sprite.setOrigin({ currOrigin.x, currOrigin.y + (fallingHeight - cumulativeFallingHeight) });
			switch (rangeType)
			{
			case Skill::RangeType::FromAbovePlayer:
				{
					auto rotation = -atan2f(distance * direction.x, fallingHeight) * 180.f / M_PI;
					sprite.setRotation(rotation);
				}
				break;
			default:
				break;
			}
		}
		break;
	case Skill::AttackShape::Rotate:
		{
			angle += speed * dt;
			SetPos({ startPos.x + distance * (float)cos((angle) * (M_PI / 180.f)), startPos.y + distance * (float)sin((angle) * (M_PI / 180.f)) });
		}
		break;
	case Skill::AttackShape::Wave:
		{
			if (waveType == Skill::WaveType::BackAndForth && !isComingBack && movingTimer >= movingDuration * 0.5f)
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

	if (dmgType != Skill::DamageType::NoDamage)
	{
		if (!isOnAtkDelay)
		{
			// 충돌 검사 및 데미지
		}
		else if (dmgType == Skill::DamageType::Periodic)
		{
			atkTimer += dt;
			if (atkTimer >= atkDelay)
			{
				isOnAtkDelay = false;
				atkTimer = 0.f;
			}
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
	if (atkShape == Skill::AttackShape::Rotate)
		vecIdx = speed > 0.f ? 0 : 1;
	else
		vecIdx = direction.x < 0.f ? 0 : 1;
	animator->Play(clipName[vecIdx]);
}
