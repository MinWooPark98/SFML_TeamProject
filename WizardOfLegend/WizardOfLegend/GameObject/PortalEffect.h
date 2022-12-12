#pragma once
#include "Effect.h"

class PortalEffect : public Effect
{
protected:
	bool showing = false;

public:
	PortalEffect();
	virtual ~PortalEffect();

	virtual void Init() override;

	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void ShowPortalEffect(Vector2f playerPos);
};

