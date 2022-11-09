#pragma once
#include "Scene.h"

class Lancer;
class PlayScene : public Scene
{
protected:
	Lancer* lancer;

public:
	PlayScene();
	virtual ~PlayScene();

	virtual void Init();
	virtual void Release() {};

	virtual void Reset() {};

	virtual void Enter() {};
	virtual void Exit() {};

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
};

