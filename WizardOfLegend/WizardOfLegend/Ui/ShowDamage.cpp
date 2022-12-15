#include "ShowDamage.h"
#include "../Framework/ResourceMgr.h"
#include <sstream>
#include "../Framework/InputMgr.h"

using namespace std;

ShowDamage::ShowDamage()
	:speed(80.f), duration(0.3f)
{
}

ShowDamage::~ShowDamage()
{
}

void ShowDamage::Init()
{
	TextObj::Init();
	SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	SetFillColor(Color::White);
	SetOutlineColor(Color::Black);
	SetOutlineThickness(0.75f);
	SetSize(32);
	text.setScale({ 0.25f, 0.25f });
}

void ShowDamage::Reset()
{
	TextObj::Reset();
	timer = 0.f;
	direction = { 0.f, -1.f };
}

void ShowDamage::Update(float dt)
{	
	TextObj::Update(dt);
	timer += dt;
	Translate(direction * speed * dt);
	if (timer >= duration)
	{
		timer = 0.f;
		SetActive(false);
	}
}

void ShowDamage::Draw(RenderWindow& window)
{
	TextObj::Draw(window);
}

void ShowDamage::ShowDamageFire(Vector2f objPos, int dmg)
{	
	SetString(to_string(dmg));
	SetPos(objPos + Utils::RandAreaPoint() * 5.f);
}

void ShowDamage::ShowDamageFire(Vector2f objPos, const string& str, bool korean)
{
	SetString(str);
	if (korean)
		AsciiToUnicode();
	SetPos(objPos + Utils::RandAreaPoint() * 5.f);
}
