#pragma once
#include "Enemy.h"

class Archer : public Enemy
{
protected:
	// Bow, Arrow
	Animator bowAnimation;
	Vector2f bowDir;
	SpriteObj* arrow;
	RectangleShape arrowDir;
	int bowPos;
	float arrowSpeed;
	float attackStart;
	bool bowWait = false;

public:
	Archer();
	virtual ~Archer() {};

	virtual void Init() override;
	virtual void Release() override { Enemy::Release(); };
	virtual void Reset() override { Enemy::Reset(); };
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	virtual void SetState(States newState) override;

	virtual void UpdateAttack(float dt) override;

	float GetArrowSpeed() { return arrowSpeed; };
	void SetArrowSpeed(float spd) { arrowSpeed = spd; };

	void SetAttackStartDelay(float delay) { attackStart = delay; };
	float GetAttackStartDelay() { return attackStart; };
};