#pragma once
#include "Scene.h"

class DevScene : public Scene
{
public:
	DevScene();
	virtual ~DevScene();

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	virtual void Enter() override;
	virtual void Exit() override;
};

