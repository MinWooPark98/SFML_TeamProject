#pragma once
#include "UiMgr.h"

class SpriteObj;
class TextObj;
class Button2;
class Player;
class FireBoss;
class HeavyBombingArcher;
class FinalBoss;
class PlaySceneSkillOptions;
class SkillCoolDownUi;
class MessageUi;
class GlassTube;
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

	// 메뉴창
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

	// 보스 임시 생존여부 설정
	bool isStart = false;
	bool isAlive = true;
	float dieTimer = 1.f;

	int monsterDamage = 0;

	PlaySceneSkillOptions* options;

	TextObj* fps;

	vector<SpriteObj*> skillSelects;
	vector<SpriteObj*> skillKeys;
	vector<SpriteObj*> skills;
	vector<SkillCoolDownUi*> coolDownUis;

	SpriteObj* gold;
	TextObj* goldText;
	SpriteObj* platinum;
	TextObj* platinumText;

	bool isTutorial;
	vector<SpriteObj*> moveKeyboard;
	float keyboardEnabledTimer = 5.f;
	int keyboardBright = 255;

	MessageUi* msgUi;
	vector<GlassTube*> glassTubes;

	int messageIndex = 0;

public:
	PlayUiMgr();
	virtual ~PlayUiMgr();

	virtual void Init() override;
	virtual void Release() override;
	virtual void SetPos(const Vector2f& pos) override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	bool IsOption() { return isOption; }

	void HpBarSizeControl(float dt);
	void BossHpBraSizeControl(float dt);
	void OverdriveBarControl(float dt);

	void SetMonsterDamage(int damage) { monsterDamage = damage; };
	void SetBossCurHp(int hp) { bossCurHp = hp; };
	void SetBossMaxHp(int hp) { bossMaxHp = hp; };

	void SetPlayer(Player* player);
	void SetFireBoss(FireBoss* boss) { this->fireBoss = boss; }
	void SetHeavyBombingArcher(HeavyBombingArcher* boss) { this->heavyBombingArcher = boss; }
	void SetFinalBoss(FinalBoss* boss) { this->finalBoss = boss; }
	void SetBossType(BossType type) { bossType = type; };

	int GetBossCurHp() { return bossCurHp; };

	void SetBossName(string name);

	void SetSkillIcon(int idx, const string& texture);

	bool GetIsStart() { return isStart; };

	void TuturialMoveKeyboardUiControl(float dt);

	virtual Object* FindUiObj(const string& name) override;

	void NewGlassTubes(GlassTube* tube);
	void GlassTubeSet(int index, bool set);
};