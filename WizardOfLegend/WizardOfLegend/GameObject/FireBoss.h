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

	enum class MoveType
	{
		None = -1,
		LeftAndRight,
		TopAndBottom,
	};

protected:
	AttackType attackType;
	MoveType moveType;
	Vector2f lastPos;
	int thirdAttackCount = 3;
	int patternCount = 3;

	//bool isThirdAttack = false;
	//bool isFireBall = false;
	bool isThrowingKnife = false;
	//bool isDragonAttack = false;
	//bool isMeteor = false;

	float patternDelay = 0.5f;
	float nextPatternDelay = 0.5f;

	MoveType lastMoveType;

public:
	FireBoss() : attackType(AttackType::None), moveType(MoveType::None) {};
	virtual ~FireBoss() {};

	virtual void Release() override { Enemy::Release(); };
	virtual void Reset() override { Enemy::Reset(); };
	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void SetState(States newState) override {};
	virtual void SetState(BossStates newState);
	virtual void UpdateAttack(float dt) override;
	virtual void UpdateMove(int attackDelay) override;

	void RandomPatternSet(AttackType type);
	void PatternOptionSet();

	void UpdateThirdAttack(float dt);
	void UpdateFireball(float dt);
	void UpdateThrowingKnife(float dt);
	void UpdateDragonAttack(float dt);
};

