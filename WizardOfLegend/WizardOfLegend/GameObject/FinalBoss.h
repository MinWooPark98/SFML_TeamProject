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
		GroundSlam,

	};

protected:
	

public:
	FinalBoss();
	virtual ~FinalBoss();
};

