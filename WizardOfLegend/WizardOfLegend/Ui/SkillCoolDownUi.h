#pragma once
#include "../GameObject/Object.h"

class SkillSet;
class TextObj;

class SkillCoolDownUi : public Object
{
protected:
	ConvexShape shape;
	TextObj* text;
	string str;
	const SkillSet* skillSet;

public:
	SkillCoolDownUi();
	virtual ~SkillCoolDownUi();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	virtual void SetPos(const Vector2f& pos) override;

	void SetSkillSet(const SkillSet* set) { skillSet = set; }
};
