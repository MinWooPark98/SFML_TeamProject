#pragma once
#include "Scene.h"
#include "../Framework/ObjectPool.h"

class Projectile;

class PlayScene : public Scene
{
protected:
	ObjectPool<Projectile> projectiles;

public:
	PlayScene();
	virtual ~PlayScene();

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Enter() override;
	virtual void Exit() override;
};

