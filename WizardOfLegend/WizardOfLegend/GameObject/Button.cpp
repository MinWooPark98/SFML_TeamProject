#include "Button.h"
#include "../Scene/SceneMgr.h"
#include "../Framework/InputMgr.h"

Button::Button()
	:type(Type::None), isMouseOn(false), isSelected(false)
{
}

Button::~Button()
{
}

void Button::Init()
{
	Object::Init();
}

void Button::Reset()
{
	Object::Reset();
	isMouseOn = false;
	isSelected = false;
}

void Button::Update(float dt)
{
	Vector2f mousePos;
	switch (type)
	{
	case Button::Type::GameObj:
		mousePos = SCENE_MGR->GetCurrentScene()->GetObjMousePos();
		break;
	case Button::Type::Ui:
		mousePos = SCENE_MGR->GetCurrentScene()->GetUiMousePos();
		break;
	default:
		break;
	}
	if (GetHitBounds().contains(mousePos))
	{
		if (!isMouseOn)
			MouseOn();
		if (InputMgr::GetMouseButtonDown(Mouse::Left))
			Select();
	}
	else if (isMouseOn)
		MouseAway();
}

void Button::Select()
{
	isSelected = true;
	if (Selected != nullptr)
		Selected();
}

void Button::Deselet()
{
	isSelected = false;
	if (Deselected != nullptr)
		Deselected();
}

void Button::MouseOn()
{
	isMouseOn = true;
}

void Button::MouseAway()
{
	isMouseOn = false;
}
