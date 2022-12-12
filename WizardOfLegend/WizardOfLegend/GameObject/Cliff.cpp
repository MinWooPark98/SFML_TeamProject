#include "Cliff.h"
#include <SFML/Graphics.hpp>
#include "../Framework/InputMgr.h"
#include "../Ui/MapToolUiMgr.h"
#include "../GameObject/DrawObj.h"

using namespace sf;

Cliff::Cliff()
{
	cliff = new RectangleShape();
	cliff->setFillColor({ 0,0,0,0 });
	cliff->setOutlineColor(Color::Blue);
	cliff->setOutlineThickness(4.f);
	isPlayerInToTheRoom = false;
	Init();
}

Cliff::~Cliff()
{
}

void Cliff::Update(float dt)
{
	Object::Update(dt);

}

void Cliff::Draw(RenderWindow& window)
{

	window.draw(*cliff);
}

void Cliff::UpdateNowDraw(float dt, DrawObj* nowDraw)
{
	if (nowDraw == nullptr)
		return;
	if (InputMgr::GetMouseButtonDown(Mouse::Left))
	{
		SetType(nowDraw->GetType());
		SetPath(nowDraw->GetPath());
	}
}

void Cliff::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	cliff->setPosition(pos);
	//cout << "sectorpos" << sector->getPosition().x << "," << sector->getPosition().y << endl;
}
