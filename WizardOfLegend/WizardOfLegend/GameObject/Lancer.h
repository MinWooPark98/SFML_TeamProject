#pragma once
#include "SpriteObj.h"
#include "../Framework/Animator.h"

class Player;
class Lancer : public SpriteObj
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

	// Spear
	Animator spearAnimation;
	SpriteObj* spear;
	bool spearWait = false;
	int spearPos;
	int attackPos = 0;
	float attackDelay = 2.f;
	SpriteObj* lancerAttackEffect; // Attack Effect

	// shader
	Shader shader;
	Texture texColorTable;
	int paletteIndex = 59;
	int paletteSize = 9;

	// player
	Player* player;
	Vector2f playerLastPos;

public:
	Lancer();
	virtual ~Lancer() {};

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
};

