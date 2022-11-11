#include "MapToolUiMgr.h"
#include "../Scene/SceneMgr.h"
#include "../Framework/Utils.h"
#include "../Framework/Framework.h"
#include "../GameObject/SpriteObj.h"
#include "../Framework/ResourceMgr.h"

MapToolUiMgr::MapToolUiMgr()
	:UiMgr(SCENE_MGR->GetScene(Scenes::MapTool))
{
}

MapToolUiMgr::~MapToolUiMgr()
{
}

void MapToolUiMgr::Init()
{
	Vector2i windowSize = FRAMEWORK->GetWindowSize();

	SpriteObj* tilePalette = new SpriteObj();
	tilePalette->SetTexture(*ResourceMgr::GetInstance()->GetTexture("graphics/Map/MapToolBook.png"));
	tilePalette->SetOrigin(Origins::TC);
	tilePalette->SetPos({ windowSize.x*0.5f,windowSize.y*0.45f });
	uiObjList.push_back(tilePalette);


	UiMgr::Init();
}

void MapToolUiMgr::Reset()
{
	UiMgr::Reset();
}

void MapToolUiMgr::Update(float dt)
{
	UiMgr::Update(dt);
}

void MapToolUiMgr::Draw(RenderWindow& window)
{
	UiMgr::Draw(window);
}
