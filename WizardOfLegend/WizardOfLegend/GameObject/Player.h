#pragma once
#include "SpriteObj.h"

class Animator;
class SkillSet;
class Skill;

class Player : public SpriteObj
{
public:
	enum class States
	{
		None = -1,
		Idle,
		Run,
		Dash,
		Slide,
		Wait,
		NormalSpell,
		PBAoE,
		Jump,
		GroundSlam,
		GroundSlamEnd,
		Hit,
		Die,
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
	bool isBackHand;	// true일 시 Backhand, false일 시 Forehand

	vector<SkillSet*> skillSets;
	SkillSet* currSkillSet;

	bool skillToolMode;

	int maxHp;
	int curHp;
	float hitDuration;
	float hitTimer;
	FloatRect hitboxSize;

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
	void UpdateWait(float dt);
	void UpdateHit(float dt);

	void SetAtkDmg(int dmg) { attackDmg = dmg; }
	int GetAtkDmg() const { return attackDmg; }
	void SetSpeed(float speed);

	bool GetBackHand() const { return isBackHand; }

	void Action(Skill* skill);
	void FinishAction();
	void SetSkillSet(int idx, const string& skillSetName, bool isPlayScene = false);
	void SetCurrSkillSet(SkillSet* skillSet) { currSkillSet = skillSet; }
	SkillSet* GetCurrSkillSet() { return currSkillSet; }

	void SetSkillToolMode() { skillToolMode = true; }

	int GetMaxHp() { return maxHp; };
	void SetMaxHp(int hp) { maxHp = hp; };
	void OnHit(const Vector2f& atkDir, int dmg);

	int GetCurHp() { return curHp; };
	void SetCurHp(int hp) { curHp = hp; };

	vector<SkillSet*>& GetSkillSets() { return skillSets; }
};

