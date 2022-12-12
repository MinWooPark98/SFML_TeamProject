#include "KeyButton.h"
#include "../GameObject/SpriteObj.h"
#include "../GameObject/TextObj.h"
#include "../Framework/ResourceMgr.h"

KeyButton::KeyButton()
	:highLightOn(false), option(nullptr), highLight(nullptr), buttonName(nullptr), textPlace(TextPlace::Down)
{
}

KeyButton::~KeyButton()
{
}

void KeyButton::Init()
{
	Object::Init();
	option = new SpriteObj();
	option->Init();
	option->SetScale({ 3.75f, 4.f });
	option->SetUI(true);
	highLight = new SpriteObj();
	highLight->Init();
	highLight->SetScale({ 3.75f, 4.f });
	highLight->SetUI(true);
	buttonName = new TextObj();
	buttonName->Init();
	buttonName->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	buttonName->SetSize(22);
	buttonName->SetFillColor(Color::White);
	buttonName->SetUI(true);
}

void KeyButton::Reset()
{
	highLightOn = false;
	HighLightOff();
}

void KeyButton::Release()
{
	if (option != nullptr)
		delete option;
	if (highLight != nullptr)
		delete highLight;
	if (buttonName != nullptr)
		delete buttonName;
}

void KeyButton::Update(float dt)
{
	Object::Update(dt);
}

void KeyButton::Draw(RenderWindow& window)
{
	Object::Draw(window);
	if (highLightOn && highLight != nullptr)
		highLight->Draw(window);
	if (option != nullptr)
		option->Draw(window);
	if (buttonName != nullptr)
		buttonName->Draw(window);
}

void KeyButton::SetDevMode(bool isDevMode)
{
	Object::SetDevMode(isDevMode);
	if (option != nullptr)
		option->SetDevMode(isDevMode);
	if (highLight != nullptr)
		highLight->SetDevMode(isDevMode);
	if (buttonName != nullptr)
		buttonName->SetDevMode(isDevMode);
}

void KeyButton::SetOption(const string& texDir)
{
	option->SetTexture(*RESOURCE_MGR->GetTexture(texDir));
	option->SetOrigin(Origins::MC);
}

void KeyButton::SetHighLight(const string& texDir)
{
	highLight->SetTexture(*RESOURCE_MGR->GetTexture(texDir));
	highLight->SetOrigin(Origins::MC);
}

void KeyButton::SetButtonName(const string& name)
{
	buttonName->SetString(name);
	buttonName->AsciiToUnicode();
	buttonName->SetOrigin(Origins::MC);
}

void KeyButton::HighLightOn()
{
	highLightOn = true;
	if (HighLightOnFunc != nullptr)
		HighLightOnFunc();
}

void KeyButton::HighLightOff()
{
	highLightOn = false;
	if (HighLightOffFunc != nullptr)
		HighLightOffFunc();
}

void KeyButton::Reposition()
{
	if (option != nullptr)
	{
		option->SetPos(position);
		option->GetHitBox().setPosition(position);
	}
	if (highLight != nullptr)
	{
		highLight->SetPos(position);
		highLight->SetPos(position);
	}
	if (buttonName != nullptr)
	{
		switch (textPlace)
		{
		case KeyButton::TextPlace::Aside:
			buttonName->SetPos(position + Vector2f(option->GetGlobalBounds().width * 0.5f + 100.f, 0.f));
			break;
		case KeyButton::TextPlace::Down:
			buttonName->SetPos(position + Vector2f(0.f, option->GetGlobalBounds().height * 0.5f + 35.f));
			break;
		default:
			break;
		}
	}
}

void KeyButton::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	Reposition();
}

void KeyButton::Translate(const Vector2f& pos)
{
	Object::Translate(pos);
	Reposition();
}
