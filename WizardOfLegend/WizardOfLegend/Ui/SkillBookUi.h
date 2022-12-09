#pragma once
#include "../GameObject/Object.h"

class SkillBookButton;
class SpriteObj;
class TextObj;
class SkillSet;
class SkillBookCardInfo;

class SkillBookUi : public Object
{
public:
	enum class States
	{
		SkillOption,
		Collection,
	};
protected:
	vector<SpriteObj*> panels;
	vector<pair<SkillBookButton*, SpriteObj*>> options;
	SkillBookCardInfo* skillInfo;
	vector<SkillSet*> playerSkillSets;
	SkillBookButton* collection;
	vector<pair<SpriteObj*, TextObj*>> infos;
	int skillVecIdx;

	bool isMoving;
	float moveSpeed;

	States state;

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
	void Reappear();
	void Disappear();
	void OptionHighLightOn(int idx);
	void OptionHighLightOff(int idx);

	void ResetCurrOptionIcon();
};

