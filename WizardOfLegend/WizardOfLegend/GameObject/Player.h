#pragma once
#include "SpriteObj.h"

class Animator;
class SkillSet;

class Player : public SpriteObj
{
public:
	enum class States
	{
		None,
		Idle,
		Run,
		Dash,
		Slide,
		NormalSpell,
		PBAoE,
		Jump,
		GroundSlam,
	};
	enum class SkillAction
	{
		NormalSpell,
		Dash,
		PBAoE,
		Jump,
		GroundSlam,
	};

protected:
	States currState;

	Animator* animator;
	Shader playerShader;
	int paletteIdx;
	int paletteSize;
	
	int attackDmg;

	float walkingSpeed;
	float runningSpeed;
	float accelTime;
	float accelTimer;
	float dashDuration;
	float dashTimer;
	float jumpDuration;
	float jumpTimer;
	float jumpDistance;
	float jumpOriginY;

	Vector2f lastDir;
	Vector2f dashDir;
	bool isBackHand;	// true�� �� Backhand, false�� �� Forehand

	vector<SkillSet*> skillSets;
	SkillSet* currSkillSet;

	bool skillToolMode;

public:
	Player();
	virtual ~Player();

	void SetState(States state);
	States GetState() const { return currState; }

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void UpdateIdle(float dt);
	void UpdateRun(float dt);
	void UpdateDash(float dt);
	void UpdateJump(float dt);

	void SetAtkDmg(int dmg) { attackDmg = dmg; }
	int GetAtkDmg() const { return attackDmg; }

	bool GetBackHand() const { return isBackHand; }

	void Action();
	void FinishAction();
	void SetCurrSkillSet(SkillSet* skillSet) { currSkillSet = skillSet; }
	SkillSet* GetCurrSkillSet() { return currSkillSet; }

	void SetSkillToolMode() { skillToolMode = true; }
	vector<SkillSet*>& GetSkillSets() { return skillSets; }
};

