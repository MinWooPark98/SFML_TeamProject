#pragma once
#include "../Framework/Animator.h"
#include "SpriteObj.h"

class Dummy : public SpriteObj
{
public:
	enum class State
	{
		Normal,
		Hit,
	};

protected:
	float speed;
	float resetTimer;
	Vector2f lastDir;
	Vector2f initPos;
	State curState;

public:
	Dummy();
	virtual ~Dummy() {};

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void OnHit(const Vector2f& atkDir, int dmg);
	void UpdateHit(float dt);

	void SetInitPos(const Vector2f& pos) { initPos = pos; };

	void SetState(State s);
};

