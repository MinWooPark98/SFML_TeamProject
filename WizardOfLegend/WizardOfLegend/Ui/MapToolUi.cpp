#include "MapToolUi.h"
#include "../Scene/Scene.h"
#include "../Framework/Utils.h"
#include "../Framework/Framework.h"

MapToolUi::MapToolUi(Scene* scene)
	:UiMgr(scene)
{
}

MapToolUi::~MapToolUi()
{
}

void MapToolUi::Init()
{
	Vector2i windowSize = FRAMEWORK->GetWindowSize();

	RectangleShape* tilePalette = new RectangleShape();
	tilePalette->setSize({ 800,400 });
	Utils::SetOrigin(*tilePalette, Origins::TC);
	tilePalette->setPosition({ windowSize.x*0.5f,windowSize.y*0.5f });
	//uiObjList.push_back(tilePalette);

}

void MapToolUi::Release()
{
}

void MapToolUi::Reset()
{
}

void MapToolUi::Update(float dt)
{
}

void MapToolUi::Draw(RenderWindow& window)
{
}
