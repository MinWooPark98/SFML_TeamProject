#pragma once
#include "../GameObject/Interactive/Interactive.h"

class Scene;
class SpriteObj;
class MessageUi;
class GlassTube : public Interactive
{
protected:
	bool isPlayerAdjacent;

	MessageUi* msgUi;

public:
	GlassTube();
	virtual ~GlassTube();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	virtual void Release() override;
};

