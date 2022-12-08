#pragma once
#include "../GameObject/Object.h"

class SkillBookButton;
class SpriteObj;
class TextObj;
class SkillSet;

class SkillBookCardInfo : public Object
{
protected:
	SpriteObj* frame;

	vector<pair<SpriteObj*, SpriteObj*>> cards;
	vector<pair<SpriteObj*, SpriteObj*>> drawingCards;
	vector<vector<pair<string, string>>> skillInfos;
	int element;
	int skillIdx;

	vector<SkillSet*> playerSkillSets;
	int currPlayerSkillSetIdx;

	bool isMoving;
	float moveSpeed;

public:
	SkillBookCardInfo();
	virtual ~SkillBookCardInfo();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Release() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void Reposition();
	virtual void SetPos(const Vector2f& pos) override;
	virtual void Translate(const Vector2f& delta) override;
	virtual void SetActive(bool active) override;
	function<void()> Deactivate;
	void Reappear();
	void Disappear();

	void SetDrawinCards();
	void ChangeSkill();
	function<void()> ChangeSkillBookUi;

	void SetCurrPlayerSkillSetIdx(int idx) { currPlayerSkillSetIdx = idx; }
};

