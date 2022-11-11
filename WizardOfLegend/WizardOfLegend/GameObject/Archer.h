#pragma once
#include "SpriteObj.h"
#include "../Framework/Animator.h"

class Player;
class Archer : public SpriteObj
{
public:
	enum class States
	{
		None = -1,
		LeftIdle,
		RightIdle,
		LeftMove,
		RightMove,
		Attack,
		Hit,
		Die,
	};

protected:
	Animator animation;
	States curState;
	Vector2f lastDir;
	Vector2f bowDir;

	float speed;
	float maxHp;
	float curHp;
	float damage;

	int attackPos = 0;


	Shader shader;
	Texture texColorTable;
	int paletteIndex = 44;
	int paletteSize = 9;


	SpriteObj* bow;
	float attackDelay = 2.f;
	bool bowWait = false;
	Vector2f playerLastPos;
	int bowPos;
	//Animator bowAnimation;
	SpriteObj* AttackImage;

	Player* player;

public:
	Archer();
	virtual ~Archer() {};

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void SetState(States newState);
	void Move(float dt);

	void SetColor(int index);

	void UpdateIdle();
	void UpdateMove();
	void UpdateAttack();

	void SetPlayer(Player* player);

	float GetSpeed() { return speed; };
	void SetSpeed(float spd) { speed = spd; };

	float GetMaxHp() { return maxHp; };
	void SetMaxHp(float hp) { maxHp = hp; };

	float GetCurHp() { return curHp; };
	void SetCurHp(float hp) { curHp = hp; };

	float GetDamage() { return damage; };
	void SetDamage(float dmg) { damage = dmg; };
};