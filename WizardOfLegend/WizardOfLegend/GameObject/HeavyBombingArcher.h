#pragma once
#include "Archer.h"

class HeavyBombingArcher : public Archer
{
protected:
	vector<SpriteObj*> smollArrow;
	int smollArrowCount = 5;
	float smollArrowDelay = 0.1f;
	int count = 0;
	int normalAttackCount = 2;

public:
	HeavyBombingArcher() {};
	virtual ~HeavyBombingArcher() {};

	virtual void Init() override;
	virtual void Release() override { Archer::Release(); };
	virtual void Reset() override { Archer::Reset(); };
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void UpdateAttack(float dt) override;
	virtual void SetState(States newState) override;
};

