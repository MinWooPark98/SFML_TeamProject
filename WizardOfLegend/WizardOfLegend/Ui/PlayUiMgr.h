#pragma once
#include "UiMgr.h"

class SpriteObj;
class PlayUiMgr : public UiMgr
{
protected:
	map<int, vector<Object*>> uiObjList;
	Vector2i windowSize;

	SpriteObj* HpBarFill;
	SpriteObj* HpBarHurt;

public:
	PlayUiMgr();
	virtual ~PlayUiMgr();

	virtual void Init() override;
	virtual void Release() override;
	virtual void SetPos(const Vector2f& pos) override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
};

