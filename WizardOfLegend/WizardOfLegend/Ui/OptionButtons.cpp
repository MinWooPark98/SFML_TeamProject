#include "OptionButtons.h"
#include "../GameObject/Button2.h"
#include "../GameObject/TextObj.h"

OptionButtons::OptionButtons()
	:axis(Axis::Vertical)
{
}

OptionButtons::~OptionButtons()
{
}

void OptionButtons::Init()
{
	Object::Init();
}

void OptionButtons::Reset()
{
	Object::Reset();
	for (auto button : buttons)
	{
		button->Reset();
	}
}

void OptionButtons::Update(float dt)
{
	Object::Update(dt);
	for (auto button : buttons)
	{
		button->Update(dt);
	}
}

void OptionButtons::Draw(RenderWindow& window)
{
	Object::Draw(window);
	for (auto button : buttons)
	{
		button->Draw(window);
	}
}

void OptionButtons::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	for (int i = 0; i < buttons.size(); ++i)
	{
		if (i == 0)
		{
			buttons[i]->SetPos(pos);
			continue;
		}
		switch (axis)
		{
		case OptionButtons::Axis::Horizontal:
			buttons[i]->SetPos(pos + Vector2f(buttons[i - 1]->GetHitBounds().width, 0.f));
			break;
		case OptionButtons::Axis::Vertical:
			buttons[i]->SetPos(pos + Vector2f(0.f, buttons[i - 1]->GetHitBounds().height));
			break;
		default:
			break;
		}
	}
}

void OptionButtons::AddButton(Button2* button, const string& str, const FloatRect& buttonSize, Color textColor, Color btnColor)
{
	if(button == nullptr)
		button = new Button2();
	button->Init();
	button->UseText();
	button->SetText("fonts/NotoSansKR-Bold.otf", 20, textColor, str);
	button->SetHitBox(buttonSize, btnColor);
	button->GetHitBox().setOutlineThickness(2.f);
	button->SetDevMode(true);
	if (!buttons.empty())
	{
		Button2* lastButton = buttons.back();
		switch (axis)
		{
		case OptionButtons::Axis::Horizontal:
			button->SetPos(lastButton->GetPos() + Vector2f(lastButton->GetHitBounds().width, 0.f));
			break;
		case OptionButtons::Axis::Vertical:
			button->SetPos(lastButton->GetPos() + Vector2f(0.f, lastButton->GetHitBounds().height));
			break;
		default:
			break;
		}
	}
	else
		button->SetPos(position);
	button->SetOrigin(Origins::TL);
	button->SetUI(true);
	buttons.push_back(button);
}
