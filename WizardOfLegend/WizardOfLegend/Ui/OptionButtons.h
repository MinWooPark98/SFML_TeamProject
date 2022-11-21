#pragma once
#include "../GameObject/Object.h"

class Button2;

class OptionButtons : public Object
{
protected:
	vector<Button2*> buttons;

public:
	OptionButtons();
	virtual ~OptionButtons();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	virtual void SetPos(const Vector2f& pos) override;
	void AddButton(Button2* button, const string& str, const FloatRect& buttonSize, Color textColor = Color::White, Color btnColor = Color(150, 150, 150, 255));
	vector<Button2*> GetButtons() { return buttons; }
};
