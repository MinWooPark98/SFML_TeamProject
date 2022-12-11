#pragma once
#include "SpriteObj.h"

class Animator;
class SkillSet;
class Skill;
class ItemMgr;

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
		Fall,
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
	float fallDuration;
	float fallTimer;

	Vector2f lastDir;
	Vector2f dashDir;
	bool isBackHand;	// true일 시 Backhand, false일 시 Forehand

	Vector2f lastStandingPos;
	Vector2f fallingScale;

	vector<SkillSet*> skillSets;
	SkillSet* currSkillSet;

	CircleShape shadow;

	bool skillToolMode;

	int maxHp;
	int curHp;
	float hitDuration;
	float hitTimer;
	FloatRect hitboxSize;
	float damageTake;
	float evasionRate;
	float criticalRate;
	float criticalRatio;

	float moveSoundTimer = 0.f;
	bool isRunning = true;

	ItemMgr* itemMgr;

	int gold;
	int platinum;

public:
	Player();
	virtual ~Player();

	void SetState(States state);
	States GetState() const { return currState; }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void UpdateIdle(float dt);
	void UpdateRun(float dt);
	void UpdateDash(float dt);
	void UpdateJump(float dt);
	void UpdateWait(float dt);
	void UpdateHit(float dt);
	void UpdateFall(float dt);

	void SetSpeed(float speed);
	void SetAtkDmg(int dmg) { attackDmg = dmg; }
	int GetAtkDmg() const { return attackDmg; }
	void SetDamageTake(float take) { damageTake = take; }
	float GetDamageTake() { return damageTake; }
	void SetEvasionRate(float ratio) { evasionRate = ratio; }
	float GetEvasionRate() { return evasionRate; }
	void SetCriticalRate(float ratio) { criticalRate = ratio; }
	float GetCriticalRate() { return criticalRate; }
	void SetCriticalRatio(float dmg) { criticalRatio = dmg; }
	float GetCriticalRatio() { return criticalRatio; }

	bool GetBackHand() const { return isBackHand; }

	void SetLastStandingPos(const Vector2f& pos) { lastStandingPos = pos; }
	bool IsStanding();

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

	ItemMgr* GetItemMgr() { return itemMgr; }

	int GetCurGold() { return gold; };
	void AddGold(int gold) { this->gold += gold; };
	void SubGold(int gold) { this->gold -= gold; };
	void GoldReset() { gold = 0; };

	int GetCurPlatinum() { return platinum; };
	void SetPlatinum(int platinum) { this->platinum = platinum; };
	void AddPlatinum(int platinum) { this->platinum += platinum; };
	void SubPlatinum(int platinum) { this->platinum -= platinum; };
	void SavePlatinum(int platinum);
	void LoadPlatinum();
};

