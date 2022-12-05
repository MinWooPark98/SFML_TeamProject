#pragma once
#include "../GameObject/TextObj.h"

class ShowDamage : public TextObj
{
protected:
	float speed;
	float duration;
	float timer = 0.f;

public:
	ShowDamage();
	virtual ~ShowDamage();

	virtual void Init() override;

	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void ShowDamageFire(Vector2f objPos, int dmg);
};

