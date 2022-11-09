#pragma once
#include "SpriteObj.h"

class Animator;
class Skill;

class Player : public SpriteObj
{
public:
	enum class States
	{
		None,
		Idle,
		Run,
		Slide,
		Skill,
	};

protected:
	States currState;

	Animator* animator;
	Shader playerShader;
	int paletteIdx;
	int paletteSize;
	
	float walkingSpeed;
	float runningSpeed;
	float accelTime;
	float accelTimer;

	Vector2f lastDir;
	bool isBackHand;	// true일 시 Backhand, false일 시 Forehand

	vector<Skill*> skills;

public:
	Player();
	virtual ~Player();

	void SetState(States state);

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void UpdateIdle(float dt);
	void UpdateRun(float dt);
	void UpdateSkill(float dt);
};

