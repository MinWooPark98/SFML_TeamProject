#pragma once
#include "Interactive.h"

class SkillBook : public Interactive
{
public:
	enum class States
	{
		None = -1,
		Idle,
		OpenReady,
		Open,
		Turn,
		BigChomp,
		Count,
	};

protected:
	States currState;

	float idleActionDelay;
	float idleActionTimer;

public:
	SkillBook();
	virtual ~SkillBook();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	
	void SetState(States state);
};
