#pragma once
#include "SpriteObj.h"
#include "../Framework/Animator.h"

class Lancer : public SpriteObj
{
public:
	enum class States
	{
		None = -1,
		Idle,
		Move,
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
	bool setSpear = false;

	int attackPos = 0;

	Shader shader;
	Texture texColorTable;
	int paletteIndex = 59;
	int paletteSize = 9;

public:
	Lancer();
	virtual ~Lancer() {};

	virtual void Init();
	virtual void Release();
	virtual void Reset();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);


	void Die();
	void SetState(States newState);
	void Move(float dt); // 플레이어 매개변수로 받기

	void SpearPos(const Vector2f& lancerPos);
	void SetColor(int index);
};

