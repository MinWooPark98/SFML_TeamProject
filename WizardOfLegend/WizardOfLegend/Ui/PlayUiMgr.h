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

	float maxHpBarSize = 62 * 4;
	float hpBarSize = 62 * 4;
	float hpBarHurtSize = 62.f * 4.f;

	bool testOverdrive = false;
	bool testHp = false;

	// 플레이어한테 받아와야 함
	TextObj* hpText;
	int playerCurHp = 525;
	int playerMaxHp = 525;

	// 메뉴창
	SpriteObj* menu;
	RectangleShape* menuRec;
	bool isOption = false;

public:
	PlayUiMgr();
	virtual ~PlayUiMgr();

	virtual void Init() override;
	virtual void Release() override;
	virtual void SetPos(const Vector2f& pos) override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void HpBarSizeControl(float dt);
};

