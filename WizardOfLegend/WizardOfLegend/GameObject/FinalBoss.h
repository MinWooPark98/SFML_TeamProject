#pragma once
#include "SpriteObj.h"

class Animator;
class SkillSet;
class Skill;

class FinalBoss : public SpriteObj
{
public:
	enum class Phase
	{
		Phase0,
		Phase1,
		Phase2,
		End,
	};
	enum class States
	{
		None = -1,
		Idle,
		Dash,
		Slide,
		Wait,
		NormalSpell,
		SplitCast,
		GroundSlam,
	};
	enum class SkillAction
	{
		NormalSpell,
		SplitCast,
		GroundSlam,
	};

private:
	States currState;

	Animator* animator;

	float speed;
	float dashDuration;
	float dashTimer;

	Vector2f lastDir;
	Vector2f dashDir;
	bool isBackHand;

	vector<SkillSet*> skillSets;
	SkillSet* currSkillSet;

	int maxHp;
	int curHp;

public:
	FinalBoss();
	virtual ~FinalBoss();

	void SetState(States state);
	States GetState() const { return currState; }

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void UpdateIdle(float dt);
	void UpdateDash(float dt);
	void UpdateWait(float dt);
};

