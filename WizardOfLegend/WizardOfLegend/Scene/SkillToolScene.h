#pragma once
#include "Scene.h"

class SkillToolScene : public Scene
{
public:
	SkillToolScene();
	virtual ~SkillToolScene();

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Enter() override;
	virtual void Exit() override;
};

