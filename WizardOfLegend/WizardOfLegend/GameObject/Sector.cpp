#include "Sector.h"
#include <SFML/Graphics.hpp>
#include "../Framework/InputMgr.h"
#include "../Ui/MapToolUiMgr.h"
#include "../GameObject/DrawObj.h"

using namespace sf;

Sector::Sector()
{
	sector = new RectangleShape();
	sector->setFillColor({ 0,0,0,0 });
	sector->setOutlineColor(Color::Red);
	sector->setOutlineThickness(4.f);
	Init();
}

Sector::~Sector()
{
}

void Sector::Update(float dt)
{
	Object::Update(dt);

}

void Sector::Draw(RenderWindow& window)
{
	
	window.draw(*sector);
}

void Sector::UpdateNowDraw(float dt, DrawObj* nowDraw)
{
	if (nowDraw == nullptr)
		return;
	if (InputMgr::GetMouseButtonDown(Mouse::Left))
	{
		SetType(nowDraw->GetType());
		SetPath(nowDraw->GetPath());
	}
}

void Sector::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	sector->setPosition(pos);
	//cout << "sectorpos" << sector->getPosition().x << "," << sector->getPosition().y << endl;
}
