#pragma once
#include "Archer.h"

class HeavyBombingArcher : public Archer
{
public:
	enum class Pattern
	{
		None = -1,
		EscapeAttack,
		MovingAttack,
	};

protected:
	vector<SpriteObj*> smollArrow;
	int smollArrowCount = 5;
	float smollArrowDelay = 0.1f;
	int count = 0;
	Pattern pattern;
	Vector2f move;
	Vector2f lastPos;

public:
	HeavyBombingArcher() {};
	virtual ~HeavyBombingArcher() { Release(); };

	virtual void Init() override;
	virtual void Release() override { Archer::Release(); };
	virtual void Reset() override { Archer::Reset(); };
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void UpdateAttack(float dt) override;
	virtual void SetState(States newState) override;
};

