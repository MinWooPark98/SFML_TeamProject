#pragma once
#include "Scene.h"
#include "../GameObject/Button.h"

class FireBoss;
class Player;


class PlayScene : public Scene
{
protected:
	FireBoss* fireBoss;
	Player* player;
	IntRect mapSize;

public:
	PlayScene();
	virtual ~PlayScene();

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	virtual void Enter() override;
	virtual void Exit() override;
};

