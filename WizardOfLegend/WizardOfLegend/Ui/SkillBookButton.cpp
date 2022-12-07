#include "SkillBookButton.h"
#include "../GameObject/SpriteObj.h"
#include "../GameObject/TextObj.h"
#include "../Framework/ResourceMgr.h"

SkillBookButton::SkillBookButton()
	:highLightOn(false), option(nullptr), highLight(nullptr), buttonName(nullptr), textPlace(TextPlace::Down)
{
}

SkillBookButton::~SkillBookButton()
{
}

void SkillBookButton::Init()
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

void SkillBookButton::Reset()
{
	highLightOn = false;
	HighLightOff();
}

void SkillBookButton::Release()
{
	if (option != nullptr)
		delete option;
	if (highLight != nullptr)
		delete highLight;
	if (buttonName != nullptr)
		delete buttonName;
}

void SkillBookButton::Update(float dt)
{
	Object::Update(dt);
}

void SkillBookButton::Draw(RenderWindow& window)
{
	Object::Draw(window);
	if (highLightOn && highLight != nullptr)
		highLight->Draw(window);
	if (option != nullptr)
		option->Draw(window);
	if (buttonName != nullptr)
		buttonName->Draw(window);
}

void SkillBookButton::SetOption(const string& texDir)
{
	option->SetTexture(*RESOURCE_MGR->GetTexture(texDir));
	option->SetOrigin(Origins::MC);
}

void SkillBookButton::SetHighLight(const string& texDir)
{
	highLight->SetTexture(*RESOURCE_MGR->GetTexture(texDir));
	highLight->SetOrigin(Origins::MC);
}

void SkillBookButton::SetButtonName(const string& name)
{
	buttonName->SetString(name);
	buttonName->AsciiToUnicode();
	buttonName->SetOrigin(Origins::MC);
}

void SkillBookButton::HighLightOn()
{
	highLightOn = true;
	if (HighLightOnFunc != nullptr)
		HighLightOnFunc();
}

void SkillBookButton::HighLightOff()
{
	highLightOn = false;
	if (HighLightOffFunc != nullptr)
		HighLightOffFunc();
}

void SkillBookButton::Reposition()
{
	option->SetPos(position);
	highLight->SetPos(position);
	switch (textPlace)
	{
	case SkillBookButton::TextPlace::Aside:
		buttonName->SetPos(position + Vector2f(option->GetGlobalBounds().width * 0.5f + 100.f, 0.f));
		break;
	case SkillBookButton::TextPlace::Down:
		buttonName->SetPos(position + Vector2f(0.f, option->GetGlobalBounds().height * 0.5f + 35.f));
		break;
	default:
		break;
	}
}

void SkillBookButton::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	Reposition();
}

void SkillBookButton::Translate(const Vector2f& pos)
{
	Object::Translate(pos);
	Reposition();
}
