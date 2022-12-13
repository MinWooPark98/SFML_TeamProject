#pragma once
#include "Effect.h"

class HitSpark : public Effect
{
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

