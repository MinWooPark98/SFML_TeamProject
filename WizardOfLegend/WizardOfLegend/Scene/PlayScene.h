#pragma once
#include "Scene.h"
#include "../GameObject/Button.h"

class PlayScene : public Scene
{
protected:
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

