#pragma once
#include "Enemy.h"
class Summoner : public Enemy
{
protected:
	float attackTimer = 0.f;
	float attackStartTimer = 2.f;

	vector<Animator*> fireAnimations;
	vector<SpriteObj*> fires;

	float fireSpeed = 200.f;
	bool isAttack = false;

	bool fireSet = true;
	bool isShot = false;

public:
	Summoner();
	virtual ~Summoner();

	virtual void Init() override;
	virtual void Release() override { SpriteObj::Release(); };
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void SetState(States newState);

	virtual void UpdateAttack(float dt) override;

	void UpdateCollision(float dt);
};