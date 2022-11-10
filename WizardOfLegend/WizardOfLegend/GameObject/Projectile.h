#pragma once
#include "SpriteObj.h"

class Animator;

class Projectile : public SpriteObj
{
public:
	enum class AttackShape
	{
		None = -1,
		Range,
		Rotate,
		Wave,
	};
	enum class DamageType
	{
		Once,
		Periodic,
	};

protected:
	AttackShape atkShape;

	Animator* animator;

	bool isMoving;
	float movingDuration;
	float movingTimer;
	float speed;
	bool skillReturn;
	int attackDmg;

	DamageType dmgType;
	bool isOnDelay;
	float atkDelay;
	float atkTimer;

	float angle;	// wave 이동 혹은 원운동 시 사용
	string clipName;

	Transform transform;
	Vector2f startPos;
	float amplitude;

public:
	Projectile();
	virtual ~Projectile();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	void SetAtkShape(AttackShape shape) { atkShape = shape; }
	void SetAnimClip(const string& clipName);
	void Fire();
	void SetMoving(bool moving) { isMoving = moving; }
	bool GetMoving() const { return isMoving; }
	void SetMovingDuration(float duration) { movingDuration = duration; }
	void SetSpeed(float speed) { this->speed = speed; }
	void SetAtkDmg(int atkDmg) { attackDmg = atkDmg; }
	void SetAtkOnce(DamageType dmgtype) { dmgType = dmgtype; }
	void SetAtkDelay(float delay) { atkDelay = delay; }
	void SetAngle(float angle) { this->angle = angle; }
	float GetAngle() const { return angle; }
	void SetTransform(const Transform& transform) { this->transform = transform; }
	void SetStartPos(const Vector2f& pos) { startPos = pos; }
	void SetAmplitude(float amplitude) { this->amplitude = amplitude; }
};

