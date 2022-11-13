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
	// Monster
	Animator animation;
	States curState;
	Vector2f lastDir;

	// Bow, Arrow
	SpriteObj* bow;
	Animator bowAnimation;
	Vector2f bowDir;
	SpriteObj* arrow;
	RectangleShape arrowDir;
	int bowPos;
	float arrowSpeed;
	float attackDelay = 2.f;
	float attackStart;
	bool bowWait = false;

	// Shader
	Shader shader;
	Texture texColorTable;
	int paletteIndex = 44;
	int paletteSize = 9;

	// Player
	Player* player;
	Vector2f playerLastPos;

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
	void UpdateAttack(float dt);

	void SetPlayer(Player* player);

	float GetArrowSpeed() { return arrowSpeed; };
	void SetArrowSpeed(float spd) { arrowSpeed = spd; };

	void SetAttackStartDelay(float delay) { attackStart = delay; };
	float GetAttackStartDelay() { return attackStart; };
};