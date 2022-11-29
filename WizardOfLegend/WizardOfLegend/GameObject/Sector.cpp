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

	if (InputMgr::GetMouseButtonDown(Mouse::Left))
	{
		//Vector2f startMousePos = InputMgr::GetMousePos();
		//startPos.x = (int)startMousePos.x / 16 * 16;
		//startPos.y = (int)startMousePos.y / 16 * 16;
		//cout<<"startpos" << startPos.x << "," << startPos.y << endl;

		SetType(nowDraw->GetType());
		SetPath(nowDraw->GetPath());
		//SetPos({ grids[i][j]->GetPos()+ (Vector2f)startPos });
	}
	//endPos = (Vector2i)InputMgr::GetMousePos();
	//SetSize({ (float)endPos.x - GetPos().x,(float)endPos.y - GetPos().y});
	//cout << "Update" << endl;
}

void Sector::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	sector->setPosition(pos);
	//cout << "sectorpos" << sector->getPosition().x << "," << sector->getPosition().y << endl;
}
