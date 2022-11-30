#pragma once
#include "SpriteObj.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/SoundMgr.h"
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

	enum class BossStates
	{
		None = -1,
		Idle,
		Move,
		Attack,
		Hit,
		Die,
		Clear,
	};

protected:
	Animator spawnAnimation;
	SpriteObj* spawn;
	float spawnTimer = 2.f;
	float deleteTimer = 0.3f;
	bool isSpawn = false;
	bool isActionStart = false;
	// card shader
	Shader cardShader;
	Texture cardTexColorTable;
	int cardPaletteIndex;
	int cardPaletteSize;
	bool isShader = true;


	Animator animation;
	States curState;
	BossStates curBossState;
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
	int maxHp;
	int curHp;
	int damage;
	int normalAttackCount;

	// Player
	Player* player;
	Vector2f playerLastPos;

	MonsterType type;

	float dieTimer;
	bool isAlive = true;

	// boss
	bool superArmor = false;

	float moveSoundTimer = 0.f;

	Vector2f monsterLastPosition;

public:
	Enemy() : curState(States::None), lastDir(1.f, 0.f) {};
	virtual ~Enemy() { Release(); };

	virtual void Init() override;
	virtual void Release() override { SpriteObj::Release(); };
	virtual void Reset() override { SpriteObj::Reset(); };
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void SetState(States newState) = 0;
	virtual void SetState(BossStates newState) {};

	float GetSpeed() { return speed; };
	void SetSpeed(float spd) { speed = spd; };

	int GetMaxHp() { return maxHp; };
	void SetMaxHp(int hp) { maxHp = hp; };

	int GetCurHp() { return curHp; };
	void SetCurHp(int hp) { curHp = hp; };

	int GetDamage() { return damage; };
	void SetDamage(int dmg) { damage = dmg; };

	void SetMoveScale(float scale) { moveScale = scale; };
	float GetMoveScale() const { return moveScale; };

	void SetAttackScale(float scale) { attackScale = scale; };
	float GetAttackScale() const { return attackScale; };

	void SetMoveAttackScale(float scale) { moveAttackScale = scale; };
	float GetMoveAttackScale() const { return moveAttackScale; };

	void SetEscapeScale(float scale) { escapeScale = scale; };
	float GetEscapeScale() const { return escapeScale; };

	void SetNormalAttackCount(int count) { normalAttackCount = count; };
	int GetNormalAttackCount() const { return normalAttackCount; };

	void SetPaletteIndex(int index) { paletteIndex = index; };
	void SetpaletteSize(int size) { paletteSize = size; };
	void SetColorTable(string table) { texColorTable.loadFromFile(table); };
	void SetColor(int index);

	void SetCardPaletteIndex(int index) { cardPaletteIndex = index; };
	void SetCardPaletteSize(int size) { cardPaletteSize = size; };
	void SetCardColorTable(string table) { cardTexColorTable.loadFromFile(table); };
	void SetCardColor(int index)
	{
		cardPaletteIndex = (cardPaletteIndex - index) % cardPaletteSize;
		cardShader.setUniform("colorTable", cardTexColorTable);
		cardShader.setUniform("paletteIndex", (float)cardPaletteIndex / cardPaletteSize);
	}
	void SetCardPalette(int index, int size, string table)
	{
		cardPaletteIndex = index;
		cardPaletteSize = size;
		cardTexColorTable.loadFromFile(table);
	};

	void SetWeaponImage(string w) { weapon->SetTexture(*RESOURCE_MGR->GetTexture(w)); };
	void SetPlayer(Player* p) { player = p; };

	void NormalMonsterMove(float dt);
	void BossMonsterMove(float dt);

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

	void SetSpawn(bool set) { isSpawn = set; };
	bool GetIsSpawn() const { return isSpawn; };
	void SpawnScale(float scale, float dt);

	void SetCardPos(Vector2f pos) { spawn->SetPos(pos); };

	Vector2f GetLastPosition() const { return monsterLastPosition; };
};