#pragma once
#include "SpriteObj.h"
#include "../Framework/Animator.h"

class Effect : public SpriteObj
{
protected:
	Animator* animation;

public:
	Effect();
	virtual ~Effect();

	virtual void Init() override;

	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
};

