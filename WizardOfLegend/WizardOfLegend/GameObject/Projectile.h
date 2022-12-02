#pragma once
#include "SpriteObj.h"
#include "Skill.h"

class Animator;

class Projectile : public SpriteObj
{
protected:
	Skill::AttackShape atkShape;

	Animator* animator;

	bool isMoving;
	float movingDuration;
	float movingTimer;
	float speed;
	Skill::WaveType waveType;
	float fallingHeight;
	float cumulativeFallingHeight;
	Skill::RangeType rangeType;
	bool isComingBack;
	int attackDmg;

	Skill::DamageType dmgType;

	bool isOnDelay;
	float delay;
	float timer;

	bool isOnAtkDelay;
	float atkDelay;
	float atkTimer;

	float distance;		// 주체로부터의 거리

	float angle;		// wave 이동 혹은 원운동 시 사용
	float frequency;	// wave 이동 시 진동수(비율)
	bool reverse;
	vector<string> clipName;
	int vecIdx;

	Vector2f startPos;
	float amplitude;

	Skill::SubjectType subType;

public:
	Projectile();
	virtual ~Projectile();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	void SetAtkShape(Skill::AttackShape shape) { atkShape = shape; }
	Skill::AttackShape GetAtkShape() { return atkShape; }
	void SetAnimClip(const vector<string>& clipName);
	void Fire();
	void SetMoving(bool moving) { isMoving = moving; }
	bool GetMoving() const { return isMoving; }
	void SetMovingDuration(float duration) { movingDuration = duration; }
	void SetSpeed(float speed) { this->speed = speed; }
	void SetWaveType(Skill::WaveType type) { waveType = type; }
	void SetFallingHeight(float height) { fallingHeight = height; }
	void SetRangeType(Skill::RangeType type) { rangeType = type; }
	void SetAtkDmg(int atkDmg) { attackDmg = atkDmg; }
	void SetDmgType(Skill::DamageType dmgtype) { dmgType = dmgtype; }
	void SetDelay(float delay) { this->delay = delay; }
	void SetAtkDelay(float atkDelay) { this->atkDelay = atkDelay; }
	void SetDistance(float distance) { this->distance = distance; }
	void SetAngle(float angle) { this->angle = angle; }
	float GetAngle() const { return angle; }
	void SetStartPos(const Vector2f& pos) { startPos = pos; }
	void SetAmplitude(float amplitude) { this->amplitude = amplitude; }
	void SetReverse(bool reverse) { this->reverse = reverse; }
	void SetFrequency(float f) { frequency = f; }
	void SetSubjectType(Skill::SubjectType type) { subType = type; }
};

