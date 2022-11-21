#pragma once
#include "Scene.h"

class TitleScene : public Scene
{
public:
	TitleScene();
	virtual ~TitleScene();

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	virtual void Enter() override;
	virtual void Exit() override;
};
