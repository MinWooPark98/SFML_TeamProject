#pragma once
#include "../GameObject/Object.h"

class SkillBookButton;
class SpriteObj;
class TextObj;

class SkillBookUi : public Object
{
protected:
	vector<SpriteObj*> panels;
	vector<SkillBookButton*> options;
	SkillBookButton* collection;
	vector<pair<SpriteObj*, TextObj*>> infos;
	int skillVecIdx;

	bool isMoving;
	float moveSpeed;

public:
	SkillBookUi();
	virtual ~SkillBookUi();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Release() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void Reposition();
	virtual void SetPos(const Vector2f& pos) override;
	virtual void Translate(const Vector2f& delta) override;
	virtual void SetActive(bool active) override;
};

