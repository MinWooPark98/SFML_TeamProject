#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class SpriteObj;

class EndingScene : public Scene
{
protected:
	int startAlpha = 0;
	int currentAlpha = 0;
	int endAlpha = 255;
	float targetTime = 15.f;
	float currentTime = 0.f;

	SpriteObj* thankYou;
public:
	EndingScene();
	virtual ~EndingScene();

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	virtual void Enter() override;
	virtual void Exit() override;

	static bool endingBgm;
	static void endingBgmStart() { endingBgm = true; };

	void FadeIn(SpriteObj* obj, float dt);
};

