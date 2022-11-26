#pragma once
#include "SpriteObj.h"

class Animator;

class CastingCircle : public SpriteObj
{
protected:
	Animator* animator;

	float duration;
	float timer;

	int attackDmg;
	bool isOnAtkDelay;
	float atkDelay;
	float atkTimer;

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
};
