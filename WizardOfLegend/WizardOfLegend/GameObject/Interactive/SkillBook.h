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
		CloseReady,
		Close,
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
	virtual void Reset() override;
	virtual void Update(float dt) override;
	
	void SetState(States state);
};
