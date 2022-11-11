#pragma once
#include "UiMgr.h"
#include "../GameObject/SpriteObj.h"


class MapToolUiMgr : public UiMgr
{
protected:
	SpriteObj tilePalette;
public:
	MapToolUiMgr();
	virtual ~MapToolUiMgr();

	virtual void Init() override;

	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

};

