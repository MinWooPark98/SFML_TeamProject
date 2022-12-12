#include "SpawnArea.h"
#include <SFML/Graphics.hpp>
#include "../Framework/InputMgr.h"
#include "../Ui/MapToolUiMgr.h"
#include "../GameObject/DrawObj.h"

SpawnArea::SpawnArea()
{
	spawnArea = new RectangleShape();
	spawnArea->setFillColor({ 0,0,0,0 });
	spawnArea->setOutlineColor(Color::Green);
	spawnArea->setOutlineThickness(4.f);
	Init();
}

SpawnArea::~SpawnArea()
{
}

void SpawnArea::Update(float dt)
{
	Object::Update(dt);
}

void SpawnArea::Draw(RenderWindow& window)
{
	window.draw(*spawnArea);
}

void SpawnArea::Release()
{
	Object::Release();
	if (spawnArea != nullptr)
	{
		delete spawnArea;
		spawnArea = nullptr;
	}
}

void SpawnArea::UpdateNowDraw(float dt, DrawObj* nowDraw)
{
	if (nowDraw == nullptr)
		return;
	if (InputMgr::GetMouseButtonDown(Mouse::Left))
	{
		SetType(nowDraw->GetType());
		SetPath(nowDraw->GetPath());
	}
}

void SpawnArea::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	spawnArea->setPosition(pos);
}
