#pragma once
#include "../GameObject/UiObject.h"

class Button2;
class UiMgr;

class SelectOption : public Object
{
protected:
	vector<pair<Button2*, Button2*>> options;
	UiMgr* uiMgr;

public:
	SelectOption(UiMgr* mgr);
	virtual ~SelectOption();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
};

