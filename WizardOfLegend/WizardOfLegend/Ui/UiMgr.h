#pragma once
#include "../GameObject/Object.h"
#include <list>

class Scene;
class UiMgr : public Object
{
protected:
	Scene* parentScene;

public:
	UiMgr(Scene* scene);
	virtual ~UiMgr();
};

