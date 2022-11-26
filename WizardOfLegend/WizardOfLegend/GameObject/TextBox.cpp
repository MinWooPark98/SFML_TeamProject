#include "TextBox.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "../Scene/SceneMgr.h"

TextBox::TextBox()
{
}

TextBox::~TextBox()
{
}

void TextBox::Init()
{
	TextObj::Init();
	SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	SetSize(20);
	SetFillColor(Color::Black);
	GetHitBox().setOutlineThickness(2.f);
	SetDevMode(true);
}

void TextBox::Reset()
{
	enabled = false;
	str.clear();
	text.setString("");
}

void TextBox::Update(float dt)
{
	TextObj::Update(dt);
	auto downList = InputMgr::GetKeyDownList();
	auto ingList = InputMgr::GetKeyIngList();
	char addChar = NULL;
	for (auto key : downList)
	{
		if (key >= Keyboard::A && key <= Keyboard::Z)
		{
			addChar = 'a' + ((char)key - (char)Keyboard::A);
			for (auto ing : ingList)
			{
				if (ing == Keyboard::LShift || ing == Keyboard::RShift)
				{
					addChar = 'A' + ((char)key - (char)Keyboard::A);
					break;
				}
			}
			break;
		}
		else if (key >= Keyboard::Num0 && key <= Keyboard::Num9)
		{
			addChar = '0' + ((char)key - (char)Keyboard::Num0);
			break;
		}
		else if (key >= Keyboard::Numpad0 && key <= Keyboard::Numpad9)
		{
			addChar = '0' + ((char)key - (char)Keyboard::Numpad0);
			break;
		}
		else if (key == Keyboard::Slash)
		{
			addChar = '/';
			break;
		}
		else if (key == Keyboard::Period)
		{
			addChar = '.';
			break;
		}
		else if (key == Keyboard::Space)
		{
			addChar = ' ';
			break;
		}
		else if (key == Keyboard::Backspace && !str.empty())
		{
			for (auto ing : ingList)
			{
				if (ing == Keyboard::LShift || ing == Keyboard::RShift)
				{
					str.clear();
					break;
				}
			}
			if(!str.empty())
				str.pop_back();
			SetString(str);
			break;
		}
	}
	if (addChar != NULL)
		SetString(str + addChar);

	auto& mousePos = SCENE_MGR->GetCurrentScene()->GetUiMousePos();
	if (InputMgr::GetKeyDown(Keyboard::Return) || (InputMgr::GetMouseButtonDown(Mouse::Left) && !GetHitBounds().contains(mousePos)))
		FinishWrite();
}

void TextBox::SetString(string str)
{
	this->str = str;
	TextObj::SetString(str);
}

string TextBox::GetString()
{
	return str;
}

void TextBox::FinishWrite()
{
	SetActive(false);
	if (SendWritten != nullptr)
		SendWritten();
}
