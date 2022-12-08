#pragma once
#include "../SpriteObj.h"

class Animator;

class NPC : public SpriteObj
{
protected:
	Animator* animator;

public:
	NPC();
	virtual ~NPC();

	virtual void Update(float dt) override;

	function<void()> Interact;
};
