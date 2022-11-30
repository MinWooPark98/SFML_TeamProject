#pragma once
#include "Enemy.h"

class FinalBoss : public Enemy
{
public:
	enum class Phase
	{
		Phase0,
		Phase1,
		Phase2,
		End,
	};
	enum class States
	{
		None = -1,
		Idle,
		Dash,
		Slide,
		Wait,
		NormalSpell,
		SplitCast,
		GroundSlam,
	};

public:
	FinalBoss();
	virtual ~FinalBoss();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
};

