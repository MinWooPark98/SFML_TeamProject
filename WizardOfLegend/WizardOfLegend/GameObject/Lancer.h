#pragma once
#include "Enemy.h"

class Lancer : public Enemy
{
protected:
	Animator spearAnimation;
	bool spearWait = false;
	int spearPos = 0;
	int attackPos;
	SpriteObj* lancerAttackEffect; // Attack Effect

public:
	Lancer() {};
	virtual ~Lancer() {};

	virtual void Init() override;
	virtual void Release() override { Enemy::Release(); };
	virtual void Reset() override { Enemy::Reset(); };
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void UpdateAttack(float dt) override;

	virtual void SetState(States newState) override;
};

