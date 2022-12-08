#pragma once
#include "NPC.h"

class SkillBook : public NPC
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

public:
	SkillBook();
	virtual ~SkillBook();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	
	void SetState(States state);
};
