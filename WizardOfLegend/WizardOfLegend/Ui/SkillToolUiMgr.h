#pragma once
#include "UiMgr.h"

class SkillToolUiMgr : public UiMgr
{
protected:
	map<int, vector<Object*>> uiObjList;

public:
	SkillToolUiMgr();
	virtual ~SkillToolUiMgr();

	virtual void Init() override;
	virtual void Release() override;
	virtual void SetPos(const Vector2f& pos) override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
};
