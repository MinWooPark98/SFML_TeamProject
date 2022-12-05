#pragma once
#include "Object.h"
#include "../Framework/Utils.h"
#include <functional>

class SpriteObj;
class TextObj;

class Button2 : public Object
{
protected:
	SpriteObj* sprite;
	TextObj* text;

	bool activated;

	bool isOtherView;
	Vector2f mousePos;
	bool isMouseOn;
	bool mouseOnSound;
	bool isClicked;

	Origins origin;
	Color textInitColor;
	Color boxInitColor;
	FloatRect btnBound;

public:
	Button2();
	virtual ~Button2();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void SetButtonBox(const FloatRect& rect, Color fillColor = Color::White, Color outLineColor = Color::White, float outLineThickness = 2.f);

	void UseSprite();
	void SetSprite(const string& texName);
	void UseText();
	void SetText(const string& fontName, int charSize = 20, Color textColor = Color::White, String string = "");

	SpriteObj* GetSprite() { return sprite; }
	TextObj* GetText() { return text; }

	virtual void SetPos(const Vector2f& pos) override;
	virtual void SetOrigin(Origins origin) override;
	void Reposition();

	void SetMouseOnSound(bool soundOn) { mouseOnSound = soundOn; }
	void DefaultMouseOn();
	void DefaultMouseOff();
	void FillBoxComplementaryColor();
	void FillBoxInitColor();
	void SetBoxInitColor(Color color) { boxInitColor = color; }

	void SetActivated(bool activate);
	bool GetActivated() { return activated; }

	void IsOtherView(bool isOtherView) { this->isOtherView = isOtherView; }
	void SetMousePos(const Vector2f pos) { mousePos = pos; }

	function<void()> MousePointerOn;
	function<void()> MousePointerOff;
	void MouseOn();
	void MouseOff();
	function<void()> ClickOn;
	function<void()> ClickOff;
	void Clicked();
	void UnClicked();
};

