#pragma once
#include "UiMgr.h"

class SkillBookUi;
class Player;

class DevUiMgr : public UiMgr
{
protected:
	map<int, vector<Object*>> uiObjList;

public:
	DevUiMgr();
	virtual ~DevUiMgr();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	virtual Object* FindUiObj(const string& name) override;
};

