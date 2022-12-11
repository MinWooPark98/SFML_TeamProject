#pragma once
#include "Interactive.h"

class ItemBox : public Interactive
{
public:
	enum class States
	{
		None = -1,
		Idle,
		Open,
		Close,
		Count,
	};

protected:
	States currState;

	float idleActionDelay;
	float idleActionTimer;

public:
	ItemBox();
	virtual ~ItemBox();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	void SetState(States state);
};

