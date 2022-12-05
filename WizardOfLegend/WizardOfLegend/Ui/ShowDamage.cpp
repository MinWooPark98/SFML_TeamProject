#include "ShowDamage.h"
#include "../Framework/ResourceMgr.h"
#include <sstream>

using namespace std;

ShowDamage::ShowDamage()
	:speed(100.f), direction(0,-1)
{
}

ShowDamage::~ShowDamage()
{
}

void ShowDamage::Init()
{
	TextObj::Init();
	text.setFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	text.setFillColor(Color::White);
}

void ShowDamage::Update(float dt)
{	
	stringstream damagetext;
	damagetext << damage;
	text.setString(damagetext.str());
	if(duration>dt)
		Translate(direction * speed * dt);
}

void ShowDamage::Draw(RenderWindow& window)
{
	TextObj::Draw(window);
}

void ShowDamage::SetPos(Vector2f pos)
{
	TextObj::SetPos(pos);
}

void ShowDamage::SetDamage(int dmg)
{
	damage = dmg;
}

void ShowDamage::ShowDamageFire()
{
}
