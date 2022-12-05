#pragma once
#include "../GameObject/TextObj.h"

class ShowDamage : public TextObj
{
protected:
	int damage;
	float speed;
	Vector2f startPos;
	float duration;
	Vector2f direction;

public:
	ShowDamage();
	virtual ~ShowDamage();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	virtual void SetPos(Vector2f pos);
	void SetDamage(int dmg);
	void ShowDamageFire();
};

