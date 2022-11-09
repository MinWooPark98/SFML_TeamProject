#pragma once
#include "SpriteObj.h"

class Projectile : public SpriteObj
{
protected:
	bool isMoving;
	float movingDuration;
	float movingTimer;
	float speed;

public:
	Projectile();
	virtual ~Projectile();

	virtual void Reset() override;
	virtual void Update(float dt) override;

	void SetMoving(bool moving) { isMoving = moving; }

};

