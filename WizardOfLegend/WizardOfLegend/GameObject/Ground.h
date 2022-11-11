#pragma once
#include "Stable.h"

class Ground : Stable
{
protected:

public:
	Ground();
	virtual ~Ground();

	virtual void Init() override;
	virtual bool Collision(SpriteObj* otherObj) override;

};

