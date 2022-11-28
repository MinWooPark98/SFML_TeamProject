#pragma once
#include "UiMgr.h"

class SpriteObj;
class TextObj;
class Button2;
class PlayUiMgr : public UiMgr
{
protected:
	map<int, vector<Object*>> uiObjList;
	Vector2i windowSize;

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

	// 플레이어한테 받아와야 함
	TextObj* hpText;
	int playerCurHp = 525;
	int playerMaxHp = 525;

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
	float bossHpBarHurtSize = 82.f * 4.f;

	// 보스마다 받아와야함
	int bossCurHp = 2000;
	int bossMaxHp = 2000;

	// 보스 임시 생존여부 설정
	bool isAlive = true;

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
};