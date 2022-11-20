#pragma once
#include "../GameObject/UiObject.h"
#include "../GameObject/Skill.h"
#include "../GameObject/TextBox.h"

class Button2;

class SelectOption : public Object
{
public:
	enum class Options
	{
		SkillName,
		Element,
		AttackType,
		AttackCntLim,
		AttackInterval,
		Distance,
		AttackShape,
		Amplitude,
		Frequency,
		MoveType,
		PlayerAction,
		SkillDelay,
		SkillCoolDown,
		DmgRatio,
		DmgType,
		DmgDelay,
		Duration,
		Speed,
		AnimClipName1,
		AnimClipName2,
		Count,
	};

protected:
	vector<pair<Button2*, Button2*>> options;
	list<TextBox*> textBoxs;

	Skill::Set selectedSet;

public:
	SelectOption();
	virtual ~SelectOption();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void ApplyOption(Options opt, TextBox* box);
	void ConvertVal(string& str, int& opt);
	void ConvertVal(string& str, float& opt);
};

