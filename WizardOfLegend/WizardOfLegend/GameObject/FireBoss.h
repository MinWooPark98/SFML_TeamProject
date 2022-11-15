#pragma once
#include "Enemy.h"

class FireBoss : public Enemy
{
public:
	enum class AttackType
	{
		None = -1,
		ThirdAttack, // ��������
		FireBall, // ������ ���� ����
		ThrowingKnife, // ������ ���ÿ� �ڷ� ��½
		DragonAttack, // �ƹ� ���� �پ �÷��̾��� ������ �������� ���� (���� ����)
		Meteor, // �� ��� ����, ���� �� �� ���� ��ġ � ����
		// 5���� �÷��̾� ��ġ �޾Ƽ� ����(������ �ٷ�, ���� ������ 0.5��), �������� ������ġ
	};

protected:


public:
	FireBoss() {};
	virtual ~FireBoss() {};

	virtual void Release() override { Enemy::Release(); };
	virtual void Reset() override { Enemy::Reset(); };
	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void SetState(States newState) override;
	virtual void UpdateAttack(float dt) override;
};

