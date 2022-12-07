#include "SkillBookButton.h"
#include "../GameObject/SpriteObj.h"
#include "../GameObject/TextObj.h"
#include "../Framework/ResourceMgr.h"

SkillBookButton::SkillBookButton()
	:highLightOn(false), option(nullptr), highLight(nullptr), name(nullptr)
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
	option->SetScale({ 3.5f, 4.f });
	option->SetUI(true);
	highLight = new SpriteObj();
	highLight->Init();
	highLight->SetScale({ 3.5f, 4.f });
	highLight->SetUI(true);
	name = new TextObj();
	name->Init();
	name->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	name->SetSize(25);
	name->SetFillColor(Color::White);
	name->SetUI(true);
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
	if (name != nullptr)
		delete name;
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
	if (name != nullptr)
		name->Draw(window);
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

void SkillBookButton::SetName(const string& name)
{
	this->name->SetString(name);
	this->name->AsciiToUnicode();
	this->name->SetOrigin(Origins::MC);
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
	name->SetPos(position + Vector2f(0.f, option->GetGlobalBounds().height * 0.5f + 40.f));
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
