#pragma once
#include "UiMgr.h"

class MapToolUi : public UiMgr
{
protected:
	RectangleShape tilePalette;
public:
	MapToolUi(Scene* scene);
	virtual ~MapToolUi();

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

};

