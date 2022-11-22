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
		Dash,
		Slide,
		Skill,
	};
	enum class SkillAction
	{
		NormalSpell,
		PBAoE,
		JumpSlash,
		Dash,
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

	Vector2f lastDir;
	bool isBackHand;	// true�� �� Backhand, false�� �� Forehand

	vector<Skill*> skills;
	Skill* currSkill;

	bool skillToolMode;

public:
	Player();
	virtual ~Player();

	void SetState(States state);

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void UpdateIdle(float dt);
	void UpdateRun(float dt);
	void UpdateDash(float dt);
	void UpdateSkill(float dt);

	void SetAtkDmg(int dmg) { attackDmg = dmg; }
	int GetAtkDmg() const { return attackDmg; }

	bool GetBackHand() const { return isBackHand; }

	void Action();
	void SetCurrSkill(Skill* skill) { currSkill = skill; }

	void SetSkillToolMode() { skillToolMode = true; }
	vector<Skill*>& GetSkills() { return skills; }
};

