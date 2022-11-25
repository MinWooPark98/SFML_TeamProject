#pragma once
#include "UiMgr.h"

class SpriteObj;
class TextObj;
class PlayUiMgr : public UiMgr
{
protected:
	map<int, vector<Object*>> uiObjList;
	Vector2i windowSize;

	SpriteObj* HpBarFill;
	SpriteObj* HpBarHurt;
	SpriteObj* OverdriveActiveBar;
	SpriteObj* playerStatusBarPortrait;

	float maxOverdriveBarSize = 48.f;
	float overdriveBarSize = 0.f;

	float maxHpBarSize = 62.f;
	float hpBarSize = 62.f;
	float hpBarHurtSize = 62.f;

	bool testOverdrive = false;
	bool testHp = false;

	// 플레이어한테 받아와야 함
	TextObj* hpText;
	float playerCurHp = 525.f;
	float playerMaxHp = 525.f;

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

