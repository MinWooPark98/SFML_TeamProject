#pragma once
#include "UiMgr.h"

class SpriteObj;
class DataTableList;

class TitleUiMgr : public UiMgr
{
protected:
	map<int, vector<Object*>> uiObjList;
	Vector2i windowSize;

	RectangleShape* backgrondShadow;
	float backgroundShadowValue = 0.f;

	SpriteObj* titleLogo;
	bool logoMove; 
	
	DataTableList* mapList;

	float startTextActiveTimer = 0.f;

public:
	TitleUiMgr();
	virtual ~TitleUiMgr();

	virtual void Init() override;
	virtual void Release() override;
	virtual void SetPos(const Vector2f& pos) override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void LoadPlayScene(const string& mapName);
};
