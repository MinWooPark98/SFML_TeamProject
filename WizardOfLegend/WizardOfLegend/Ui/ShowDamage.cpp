#include "ShowDamage.h"
#include "../Framework/ResourceMgr.h"
#include <sstream>
#include "../Framework/InputMgr.h"

using namespace std;

ShowDamage::ShowDamage()
	:speed(100.f), direction(0,-1), duration(1.f)
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
	SetSize(40);
}

void ShowDamage::Reset()
{
	TextObj::Reset();
	timer = 0.f;
}

void ShowDamage::Update(float dt)
{	
	TextObj::Update(dt);
	if (InputMgr::GetKeyDown(Keyboard::Key::F2))
	{
		timer += dt;
		if (duration > timer)
		{
			Translate(direction * speed * dt);
			timer = 0.f;
			SetActive(false);
		}
	}
}

void ShowDamage::Draw(RenderWindow& window)
{
	TextObj::Draw(window);
}

void ShowDamage::ShowDamageFire(Vector2f objPos, int dmg)
{	
	SetString(to_string(dmg));
	SetPos(objPos);
}
