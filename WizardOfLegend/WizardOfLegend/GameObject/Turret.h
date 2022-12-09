#pragma once
#include "Enemy.h"

class Skill;
class Turret : public Enemy
{
protected:
	bool isAttack;
	float attackStartTimer = 2.f;
	float attackTimer = 0.f;
	Skill* newSkill;

	SpriteObj* turretEye; // ÈòÀÚ
	SpriteObj* turretIris; // °ËÀºÀÚ
	SpriteObj* turretEyeWary; // ´«»ì

	float eyeMoveSpeed = 2.f;

public:
	Turret();
	virtual ~Turret();

	virtual void Init() override;
	virtual void Release() override { SpriteObj::Release(); };
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void SetState(States newState) override;

	void EyePos(const Vector2f& pos);
	virtual void UpdateAttack(float dt);

	void EyeMove(float dt);
	void EyeReset(float dt);
};

