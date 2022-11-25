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
		NormalSpell,
		PBAoE,
		JumpSlash,
		GroundSlam,
	};
	enum class SkillAction
	{
		NormalSpell,
		Dash,
		PBAoE,
		JumpSlash,
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
	bool isBackHand;	// true일 시 Backhand, false일 시 Forehand

	vector<Skill*> skills;
	Skill* currSkill;

	bool skillToolMode;

	int maxHp = 525;
	int curHp;

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
	void UpdateJumpSlash(float dt);

	void SetAtkDmg(int dmg) { attackDmg = dmg; }
	int GetAtkDmg() const { return attackDmg; }

	bool GetBackHand() const { return isBackHand; }

	void Action();
	void SetCurrSkill(Skill* skill) { currSkill = skill; }

	void SetSkillToolMode() { skillToolMode = true; }
	vector<Skill*>& GetSkills() { return skills; }

	int GetMaxHp() { return maxHp; };
	void SetMaxHp(int hp) { maxHp = hp; };

	int GetCurHp() { return curHp; };
	void SetCurHp(int hp) { curHp = hp; };
};

