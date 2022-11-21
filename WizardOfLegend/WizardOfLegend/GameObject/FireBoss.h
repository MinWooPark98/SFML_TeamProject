#pragma once
#include "Enemy.h"

class Skill;
class FireBoss : public Enemy
{
public:
	enum class AttackType
	{
		None = -1,
		ThirdAttack, // 근접공격
		FireBall, // 가만히 서서 날림
		ThrowingKnife, // 날리는 동시에 뒤로 슬쩍
		DragonAttack, // 아무 벽에 붙어서 플레이어쪽 벽으로 개빠르게 돌진 (궤적 나옴)
		Meteor, // 맵 가운데 점프, 착지 후 맵 랜덤 위치 운석 낙하
		// 5개는 플레이어 위치 받아서 낙하(착지시 바로, 다음 딜레이 0.5초), 나머지는 랜덤위치
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
	void UpdateMeteor(float dt);

	void SetPlayerLastPos(Vector2f playerPos) { playerLastPos = playerPos; };
};

