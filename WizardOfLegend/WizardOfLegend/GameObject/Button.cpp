#include "Button.h"
#include "../GameObject/SpriteObj.h"
#include "../GameObject/TextObj.h"
#include <iostream>

using namespace std;

Button::Button(UiMgr* mgr)
	: sprite(nullptr), text(nullptr), uimgr(mgr), bndType(BoundType::None), isClickColor(false)
{
}

Button::~Button()
{
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
	if (text != nullptr)
	{
		delete text;
		text = nullptr;
	}
}

void Button::Init()
{
	UiObject::Init();
}

void Button::Reset()
{
	UiObject::Reset();
}

void Button::ColorClear()
{
	UiObject::ColorClear();
	if (isClickColor)
	{
		if (sprite != nullptr && sprite->GetSprite().getTexture() != nullptr)
		{
			sprite->SetColor(Color::White);
		}
		if (text != nullptr && text->GetText().getFont() != nullptr)
		{
			text->SetFillColor(Color::White);
		}
	}
}

void Button::Update(float dt)
{
	if (!enabled)
		return;
	UiObject::Update(dt);
	if (IsDown())
	{
		if (isClickColor)
		{
			if (sprite != nullptr && sprite->GetSprite().getTexture() != nullptr)
			{
				sprite->SetColor(Color(200, 200, 200, 255));
			}
			if (text != nullptr && text->GetText().getFont() != nullptr)
			{
				text->SetFillColor(Color(200, 200, 200, 255));
			}
		}
	}
	if (IsUp() || IsExit())
	{
		if (isClickColor)
		{
			if (sprite != nullptr && sprite->GetSprite().getTexture() != nullptr)
			{
				sprite->SetColor(Color::White);
			}
			if (text != nullptr && text->GetText().getFont() != nullptr)
			{
				text->SetFillColor(Color::White);
			}
		}
	}
	if (IsDownRight())
	{
		if (isClickColor)
		{
			if (sprite != nullptr && sprite->GetSprite().getTexture() != nullptr)
			{
				sprite->SetColor(Color(200, 200, 200, 255));
			}
			if (text != nullptr && text->GetText().getFont() != nullptr)
			{
				text->SetFillColor(Color(200, 200, 200, 255));
			}
		}
	}
	if (IsUpRight() || IsExit())
	{
		if (isClickColor)
		{
			if (sprite != nullptr && sprite->GetSprite().getTexture() != nullptr)
			{
				sprite->SetColor(Color::White);
			}
			if (text != nullptr && text->GetText().getFont() != nullptr)
			{
				text->SetFillColor(Color::White);
			}
		}
	}
}

void Button::Draw(RenderWindow& window)
{
	if (!enabled)
		return;
	UiObject::Draw(window);
	if (sprite != nullptr && sprite->GetSprite().getTexture() != nullptr)
	{
		sprite->Draw(window);
	}
	if (text != nullptr && text->GetText().getFont() != nullptr)
	{
		text->Draw(window);
	}
}

void Button::SetPos(Vector2f pos)
{
	UiObject::SetPos(pos);
	if(sprite != nullptr)
		sprite->SetPos(pos);
	if(text != nullptr)
		text->SetPos(pos);
	ReBound();
}

void Button::Release()
{
	Object::Release();
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
	if (text != nullptr)
	{
		delete text;
		text = nullptr;
	}
}

void Button::UseSprite()
{
	sprite = new SpriteObj();
	sprite->Init();
	sprite->SetUI(true);
	SetBoundType(BoundType::SPRITE);
}

void Button::UseText()
{
	text = new TextObj();
	text->Init();
	SetBoundType(BoundType::TEXT);
}

void Button::SetTexture(Texture& t, bool isBound)
{
	sprite->SetTexture(t);
	if (isBound)
	{
		SetBound(sprite->GetGlobalBounds());
		bndType = BoundType::SPRITE;
	}
}

void Button::SetText(Font& font, int size, Color color, string str, bool isBound)
{
	text->SetText(font, size, color, str);
	if (isBound)
	{
		SetBound(text->GetGlobalBounds());
		bndType = BoundType::TEXT;
	}
}

void Button::SetOrigin(Origins origin)
{
	if (sprite != nullptr && sprite->GetSprite().getTexture() != nullptr)
	{
		sprite->SetOrigin(origin);
	}
	if (text != nullptr && text->GetText().getFont() != nullptr)
	{
		text->SetOrigin(origin);
	}
	ReBound();
}

void Button::ReBound()
{
	switch (bndType)
	{
	case Button::BoundType::None:
		break;
	case Button::BoundType::SPRITE:
		SetBound(sprite->GetGlobalBounds());
		break;
	case Button::BoundType::TEXT:
		SetBound(text->GetGlobalBounds());
		break;
	default:
		break;
	}

}

std::ofstream& operator<<(std::ofstream& ofs, const Button& cur)
{
	switch (cur.GetState())
	{
	case UiState::None:
		cout << "None";
		break;
	case UiState::Enter:
		cout << "Enter";
		break;
	case UiState::Stay:
		cout << "Stay";
		break;
	case UiState::Down:
		cout << "Down";
		break;
	case UiState::Up:
		cout << "Up";
		break;
	case UiState::Exit:
		cout << "Exit";
		break;
	default:
		break;
	}
	return ofs;
}
