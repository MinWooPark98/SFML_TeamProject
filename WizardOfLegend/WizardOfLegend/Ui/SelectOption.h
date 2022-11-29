#pragma once
#include "../GameObject/Object.h"
#include "../GameObject/Skill.h"
#include "../GameObject/TextBox.h"
#include "OptionButtons.h"

class DataTableList;

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
		WaveType,
		FallingHeight,
		RangeType,
		PlayerAction,
		SkillDelay,
		SkillCoolDown,
		DmgType,
		DmgRatio,
		DmgDelay,
		Duration,
		Speed,
		AnimClipName1,
		AnimClipName2,
		SoundName1,
		SoundName2,
		Count,
	};

protected:
	pair<OptionButtons*, OptionButtons*> options;
	map<Options, OptionButtons*> optButtons;
	map<Options, TextBox*> textBoxs;
	vector<OptionButtons*> menuButtons;

	Skill::Set selectedSet;

	DataTableList* skillList;
	DataTableList* skillSetList;

public:
	SelectOption();
	virtual ~SelectOption();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void ActivateAll();
	void ActivateBasedOnAtkShape();
	void ActivateOption(Options option);
	void DeactivateOption(Options option);
	void ApplyText(Options opt, TextBox* box);
	void ApplyTextStr(Options opt, string str);
	void ApplyOptBtn(Options opt, OptionButtons* opts, Button2* btn);
	void ApplyOptBtnIdx(Options opt, int vecIdx);
	void ConvertVal(string& str, int& opt);
	void ConvertVal(string& str, float& opt);

	void SaveSetToCSV();
	void SetPlayer1stSkill();

	void Load(const string& skillName);
	void LoadSkillSet(const string& skillSetName);

	Skill::Set& GetSet() { return selectedSet; }
};

