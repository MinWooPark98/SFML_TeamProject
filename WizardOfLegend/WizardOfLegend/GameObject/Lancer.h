#pragma once
#include "SpriteObj.h"
#include "../Framework/Animator.h"

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
	Animator animation;
	States curState;
	Vector2f lastDir;
	Vector2f SpireDir;

	float speed;
	float maxHp;
	float curHp;
	float damage;

	SpriteObj* spear;

	int attackPos = 0;

	Shader shader;
	Texture texColorTable;
	int paletteIndex = 59;
	int paletteSize = 9;

	// dev
	Object* player;
	Vector2f playerDir;
	RectangleShape playerRec;
	float playerSpeed = 500.f;

	float attackDelay = 2.f;
	bool spearWait = false;
	Vector2f playerLastPos;
	int spearPos;

public:
	Lancer();
	virtual ~Lancer() {};

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;


	void Die();
	void SetState(States newState);
	void Move(float dt, Object* player);

	void SetColor(int index);
	void DevPlayerMove(float dt);

	void UpdateIdle();
	void UpdateMove();
	void UpdateAttack();
};

