#pragma once
#include "SpriteObj.h"

class Animator;
class Skill;
class Player;

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
		GroundSlamEnd,
	};
	enum class SkillAction
	{
		NormalSpell,
		SplitCast,
		GroundSlam,
	};
	enum class DashType
	{
		Evasion,
		Chase,
	};

private:
	States currState;

	Animator* animator;

	int attackDmg;
	int attackCnt;
	float attackRange;

	float speed;
	float dashDuration;
	float dashTimer;
	int evasionCntLim;
	int evasionCnt;
	DashType dashType;

	Vector2f lastDir;
	Vector2f dashDir;
	bool isBackHand;

	vector<Skill*> normalSkills;
	int vecIdx;
	vector<Skill*> chaosSkills;
	Skill* currSkill;

	int maxHp;
	int curHp;

	bool superArmor;
	float superArmorDelay;
	float superArmorTimer;

	Player* player;

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

	void Dash(DashType type);

	void SetAtkDmg(int dmg) { attackDmg = dmg; }
	int GetAtkDmg() const { return attackDmg; }
	void SetAttackRange(float range) { attackRange = range; }
	
	void NextAction();

	bool GetBackHand() const { return isBackHand; }

	void Action(Skill* skill);
	void FinishAction();
	void SetCurrSkill(Skill* skill) { currSkill = skill; }
	Skill* GetCurrSkill() { return currSkill; }

	int GetMaxHp() { return maxHp; };
	void SetMaxHp(int hp) { maxHp = hp; };

	int GetCurHp() { return curHp; };
	void SetCurHp(int hp) { curHp = hp; };

	void SetPlayer(Player* player) { this->player = player; }

	vector<Skill*>& GetNormalSkills() { return normalSkills; }
	vector<Skill*>& GetChaosSkills() { return chaosSkills; }
};

