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
	enum class MoveType
	{
		OneWay,
		BackAndForth,
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
	MoveType moveType;
	int attackDmg;

	DamageType dmgType;
	bool isOnDelay;
	float atkDelay;
	float atkTimer;

	float angle;		// wave 이동 혹은 원운동 시 사용
	float frequency;	// wave 이동 시 진동수(비율)
	bool reverse;
	vector<string> clipName;

	Vector2f startPos;
	float amplitude;

public:
	Projectile();
	virtual ~Projectile();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	void SetAtkShape(AttackShape shape) { atkShape = shape; }
	void SetAnimClip(const vector<string>& clipName);
	void Fire();
	void SetMoving(bool moving) { isMoving = moving; }
	bool GetMoving() const { return isMoving; }
	void SetMovingDuration(float duration) { movingDuration = duration; }
	void SetSpeed(float speed) { this->speed = speed; }
	void SetMoveType(MoveType type) { moveType = type; }
	void SetAtkDmg(int atkDmg) { attackDmg = atkDmg; }
	void SetDmgType(DamageType dmgtype) { dmgType = dmgtype; }
	void SetAtkDelay(float delay) { atkDelay = delay; }
	void SetAngle(float angle) { this->angle = angle; }
	float GetAngle() const { return angle; }
	void SetStartPos(const Vector2f& pos) { startPos = pos; }
	void SetAmplitude(float amplitude) { this->amplitude = amplitude; }
	void SetReverse(bool reverse) { this->reverse = reverse; }
};

