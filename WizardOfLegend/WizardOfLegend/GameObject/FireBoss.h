#pragma once
#include "Enemy.h"

class Skill;
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

	bool isThrowingKnife = false;

	float patternDelay = 0.5f;
	float nextPatternDelay = 0.5f;
	float angles = 0.f;

	MoveType lastMoveType;

	vector<Skill*> skills;

	SpriteObj* fireWing;
	Animator fireWingAnimation;

	SpriteObj* firebossKick;
	Animator kickAnimation;

public:
	FireBoss() : attackType(AttackType::None), moveType(MoveType::None) {};
	virtual ~FireBoss() { Release(); };

	virtual void Release() override 
	{ 
		Enemy::Release(); 
		for (auto& it : skills)
		{
			delete it;
		}
		skills.clear();
	};
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
	void UpdateMeteor(float dt);

	void SetPlayerLastPos(Vector2f playerPos) { playerLastPos = playerPos; };

	RectangleShape GetFireBossKickHitBox() const { return firebossKick->GetHitBox(); }; // OBB �浹�� ���� ����
};

