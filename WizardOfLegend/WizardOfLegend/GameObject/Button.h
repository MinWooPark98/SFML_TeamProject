#pragma once
#include "Object.h"
#include <functional>

class Button : public Object
{
public:
	enum class Type
	{
		None = -1,
		GameObj,
		Ui,
	};
protected:
	Type type;

	bool isMouseOn;
	bool isSelected;

public:
	Button();
	virtual ~Button();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	RectangleShape& GetButtonBox() { return hitbox; }

	function<void()> Selected;
	function<void()> Deselected;
	virtual void Select();
	virtual void Deselet();

	virtual void MouseOn();
	virtual void MouseAway();
};

