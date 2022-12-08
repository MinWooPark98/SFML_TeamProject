#pragma once
#include "../GameObject/Object.h"

class SpriteObj;
class TextObj;

class SkillBookButton : public Object
{
public:
	enum class TextPlace
	{
		Aside,
		Down,
	};

protected:
	bool highLightOn;

	SpriteObj* option;
	SpriteObj* highLight;
	TextObj* buttonName;

	TextPlace textPlace;

public:
	SkillBookButton();
	~SkillBookButton();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Release() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	bool GetHighLightOn() { return highLightOn; }

	void SetOption(const string& texDir);
	SpriteObj* GetOption() { return option; }
	void SetHighLight(const string& texDir);
	void SetButtonName(const string& name);

	void HighLightOn();
	void HighLightOff();
	function<void()> HighLightOnFunc;
	function<void()> HighLightOffFunc;

	void SetTextPlace(TextPlace place) { textPlace = place; }
	void Reposition();
	virtual void SetPos(const Vector2f& pos);
	virtual void Translate(const Vector2f& pos);
};

