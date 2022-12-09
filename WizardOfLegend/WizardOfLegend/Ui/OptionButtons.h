#pragma once
#include "../GameObject/Object.h"

class Button2;

class OptionButtons : public Object
{
public:
	enum class Axis
	{
		Horizontal,
		Vertical,
	};

protected:
	vector<Button2*> buttons;
	Axis axis;

public:
	OptionButtons();
	virtual ~OptionButtons();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	virtual void SetPos(const Vector2f& pos) override;
	virtual void SetActive(bool active) override;

	void Clear();

	void SetAxis(Axis axis) { this->axis = axis; }
	void AddButton(Button2* button, const string& str, const FloatRect& buttonSize, Color textColor = Color::White, Color btnColor = Color(150, 150, 150, 255));
	vector<Button2*>& GetButtons() { return buttons; }
};

