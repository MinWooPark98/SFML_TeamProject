#include "DrawObj.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/SpriteObj.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMgr.h"

DrawObj::DrawObj(UiMgr* uimgr)
	:Button(uimgr), isMove(true)
{
	SetMove(true);
}

DrawObj::~DrawObj()
{
}

void DrawObj::Update(float dt)
{
	Button::Update(dt);

	if (isMove)
	{
		auto mousePos = InputMgr::GetMousePos();
		mousePos = SCENE_MGR->GetCurrentScene()->ScreenToUiPosition((Vector2i)mousePos);
		SetPos(mousePos);
	}

}

void DrawObj::SetMove(bool move)
{
	isMove = move;
}

void DrawObj::SetSize(Vector2f size)
{
	auto localSize = sprite->GetLocalBounds();
	sprite->SetScale({ size.x / localSize.width, size.y / localSize.height });
}

Vector2f DrawObj::GetSize()
{
	return sprite->GetSize();
}
