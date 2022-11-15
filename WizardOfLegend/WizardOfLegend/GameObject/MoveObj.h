#pragma once
#include "Button.h"

class MoveObj : public Button
{


public:
	MoveObj(UiMgr* scene);
	virtual ~MoveObj();
	virtual void Update(float dt);
	virtual void Init();
	virtual void Reset();
};

