#pragma once
#include "Scene.h"

class Projectile;

class PlayScene : public Scene
{
public:
	PlayScene();
	virtual ~PlayScene();

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Enter() override;
	virtual void Exit() override;
};

