#pragma once
#include "Interactive.h"

class Wardrobe : public Interactive
{
public:
	enum class States
	{
		None = -1,
		Idle,
		Open,
		Close,
		Blink,
		Mustache,
		Count,
	};

protected:
	States currState;

	float idleActionDelay;
	float idleActionTimer;

public:
	Wardrobe();
	virtual ~Wardrobe();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	void SetState(States state);
};

