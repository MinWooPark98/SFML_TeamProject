#pragma once
#include "Enemy.h"

class Player;
class Lancer : public Enemy
{
protected:
	// Spear
	Animator spearAnimation;
	bool spearWait = false;
	int spearPos;
	int attackPos = 0;
	SpriteObj* lancerAttackEffect; // Attack Effect

public:
	Lancer() {};
	virtual ~Lancer() {};

	virtual void Init() override;
	virtual void Release() override { Enemy::Release(); };
	virtual void Reset() override { Enemy::Reset(); };
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void SetState(States newState);

	virtual void UpdateAttack(float dt) override;
};

