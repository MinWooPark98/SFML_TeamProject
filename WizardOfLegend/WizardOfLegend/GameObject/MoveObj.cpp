#include "MoveObj.h"
#include "../../Framework/InputMgr.h"

MoveObj::MoveObj(UiMgr* scene)
	:Button(scene)
{
	SetMove(true);
}

MoveObj::~MoveObj()
{
}

void MoveObj::Update(float dt)
{
	Button::Update(dt);
	
	if(IsDragRight())
		SetPos(InputMgr::GetMousePos());
}

void MoveObj::Init()
{
	Button::Init();
}

void MoveObj::Reset()
{
	Button::Reset();
}
