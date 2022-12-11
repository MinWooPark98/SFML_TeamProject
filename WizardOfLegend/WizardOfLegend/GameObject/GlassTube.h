#pragma once
#include "SpriteObj.h"

class Scene;
class Player;
class SpriteObj;

class GlassTube : public SpriteObj
{
protected:
	bool isPlayerAdjacent;

	Scene* currScene;
	Player* player;

	SpriteObj* FKey;

public:
	GlassTube();
	virtual ~GlassTube();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void SetPlayer(Player* player) { this->player = player; };

	void SetIsPlayerAdjacent(bool ad);
};

