#pragma once
#include "SpriteObj.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "Player.h"

class Enemy : public SpriteObj
{
public:
	enum class MonsterType
	{
		None = -1,
		Normal,
		StageBoss,
		MiddleBoss,
		LastBoss,
	};

	enum class States
	{
		None = -1,
		LeftIdle,
		RightIdle,
		LeftMove,
		RightMove,
		Attack,
		MoveAttack,
		Hit,
		Die,
	};

protected:
	Animator animation;
	States curState;
	Vector2f lastDir;

	SpriteObj* weapon;

	float attackDelay;

	// shader
	Shader shader;
	Texture texColorTable;
	int paletteIndex;
	int paletteSize;

	float attackScale;
	float moveAttackScale;
	float escapeScale;
	float moveScale;
	float speed;
	float maxHp;
	float curHp;
	float damage;

	// Player
	Player* player;
	Vector2f playerLastPos;

	MonsterType type;

	float dieTimer;
	bool isAlive = true;

	// boss
	bool superArmor = false;

public:
	Enemy() : curState(States::None), lastDir(1.f, 0.f) {};
	virtual ~Enemy() {};

	virtual void Init() override;
	virtual void Release() override { SpriteObj::Release(); isAlive = true; };
	virtual void Reset() override { SpriteObj::Reset(); };
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override { SpriteObj::Draw(window); };
	virtual void SetState(States newState) = 0;

	float GetSpeed() { return speed; };
	void SetSpeed(float spd) { speed = spd; };

	float GetMaxHp() { return maxHp; };
	void SetMaxHp(float hp) { maxHp = hp; };

	float GetCurHp() { return curHp; };
	void SetCurHp(float hp) { curHp = hp; };

	float GetDamage() { return damage; };
	void SetDamage(float dmg) { damage = dmg; };

	void SetMoveScale(float scale) { moveScale = scale; };
	float GetMoveScale() const { return moveScale; };

	void SetAttackScale(float scale) { attackScale = scale; };
	float GetAttackScale() const { return attackScale; };

	void SetMoveAttackScale(float scale) { moveAttackScale = scale; };
	float GetMoveAttackScale() const { return moveAttackScale; };

	void SetEscapeScale(float scale) { escapeScale = scale; };
	float GetEscapeScale() const { return escapeScale; };

	void SetPaletteIndex(int index) { paletteIndex = index; };
	void SetpaletteSize(int size) { paletteSize = size; };
	void SetColorTable(string table) { texColorTable.loadFromFile(table); };
	void SetColor(int index);

	void SetWeaponImage(string w) { weapon->SetTexture(*RESOURCE_MGR->GetTexture(w)); };
	void SetPlayer(Player* p) { player = p; };

	void NormalMonsterMove(float dt);

	void UpdateIdle();
	virtual void UpdateMove(int attackDelay);
	virtual void UpdateAttack(float dt) = 0;
	
	void SetMonsterType(MonsterType t) { type = t; };

	void SetIsAlive(bool set) { isAlive = set; };
	bool GetIsAlive() const { return isAlive; };

	void SetSuperArmor(bool set) { superArmor = set; };
	bool GetSuperArmor() const { return superArmor; };

	void SetStat(float spd, float cHp, float mHp, float dmg)
	{
		speed = spd;
		curHp = cHp;
		maxHp = mHp;
		damage = dmg;
	};
};

