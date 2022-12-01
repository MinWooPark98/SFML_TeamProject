#pragma once
#include "SpriteObj.h"
#include "Skill.h"

class Animator;

class CastingCircle : public SpriteObj
{
protected:
	Animator* animator;

	float duration;
	float timer;

	Skill::DamageType dmgType;
	int attackDmg;
	bool isOnAtkDelay;
	float atkDelay;
	float atkTimer;

	Skill::SubjectType subType;

public:
	CastingCircle();
	virtual ~CastingCircle();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	void Do();
	void SetDuration(float duration) { this->duration = duration; }
	void SetTimer(float timer) { this->timer = timer; }
	void SetDmgType(Skill::DamageType type) { dmgType = type; }
	void SetAtkDmg(int dmg) { attackDmg = dmg; }
	void SetAtkDelay(float delay) { atkDelay = delay; }
	void SetSubjectType(Skill::SubjectType type);
};
