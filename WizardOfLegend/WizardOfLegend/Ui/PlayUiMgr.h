#pragma once
#include "UiMgr.h"

class SpriteObj;
class TextObj;
class Button2;
class Player;
class FireBoss;
class HeavyBombingArcher;
class FinalBoss;
class PlayUiMgr : public UiMgr
{
public:
	enum class BossType
	{
		Archer,
		FireBoss,
		FinalBoss,
	};

protected:
	map<int, vector<Object*>> uiObjList;
	Vector2i windowSize;

	Player* player;
	SpriteObj* HpBarFill;
	SpriteObj* HpBarHurt;
	SpriteObj* OverdriveActiveBar;
	SpriteObj* playerStatusBarPortrait;

	float maxOverdriveBarSize = 48.f * 4.f;
	float overdriveBarSize = 0.f;

	float maxHpBarSize = 62.f * 4.f;
	float hpBarSize = 62.f * 4.f;
	float hpBarHurtSize = 62.f * 4.f;

	bool testOverdrive = false;

	TextObj* hpText;

	// �޴�â
	SpriteObj* menu;
	RectangleShape* menuRec;
	bool isOption = false;

	SpriteObj* playerMarker;
	SpriteObj* playerMarkerGlow;
	SpriteObj* playerMarkerOutLine;

	// bossHp
	TextObj* bossName;
	SpriteObj* bossHpBarFill;
	SpriteObj* bossHpBarHurt;
	SpriteObj* bossHpBarBG;

	float bossMaxHpBarSize = 89.f * 4.f;
	float bossHpBarSize = 89.f * 4.f;
	float bossHpBarHurtSize = 89.f * 4.f;

	BossType bossType;
	FireBoss* fireBoss;
	FinalBoss* finalBoss;
	HeavyBombingArcher* heavyBombingArcher;
	int bossCurHp;
	int bossMaxHp;

	// ���� �ӽ� �������� ����
	bool isStart = false;
	bool isAlive = true;
	float dieTimer = 1.f;

	int monsterDamage = 0;

	TextObj* fps;

	vector<SpriteObj*> skillSelects;
	vector<SpriteObj*> skillKeys;

public:
	PlayUiMgr();
	virtual ~PlayUiMgr();

	virtual void Init() override;
	virtual void Release() override;
	virtual void SetPos(const Vector2f& pos) override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void HpBarSizeControl(float dt);
	void BossHpBraSizeControl(float dt);
	void OverdriveBarControl(float dt);

	void SetMonsterDamage(int damage) { monsterDamage = damage; };
	void SetBossCurHp(int hp) { bossCurHp = hp; };
	void SetBossMaxHp(int hp) { bossMaxHp = hp; };

	void SetPlayer(Player* player) { this->player = player; }
	void SetFireBoss(FireBoss* boss) { this->fireBoss = boss; }

	void SetBossType(BossType type) { bossType = type; };

	int GetBossCurHp() { return bossCurHp; };

	void SetBossName(string name);
};