#pragma once
#include "SpriteObj.h"
#include "../Framework/Animator.h"

class HitSpark : public SpriteObj
{
protected:
	Animator* animation;
	bool showing=false;

public:
	HitSpark();
	virtual ~HitSpark();

	virtual void Init() override;

	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void EnemyHitSparkFire(Vector2f hitpoint);
	void PlayerHitSparkFire(Vector2f hitpoint);
};

