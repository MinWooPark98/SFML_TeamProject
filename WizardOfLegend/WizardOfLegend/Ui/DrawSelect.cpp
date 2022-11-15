#include "DrawSelect.h"
#include "MapToolUiMgr.h"
#include "../Framework/ResourceMgr.h"

DrawSelect::DrawSelect(UiMgr* uimgr)
	:Button(uimgr)
{
}

DrawSelect::~DrawSelect()
{
}

void DrawSelect::Set(string type, string path, string drawPath)
{
	this->type = type;
	this->path = path;

	SetTexture(*RESOURCE_MGR->GetTexture(drawPath), true);
}
void DrawSelect::Update(float dt)
{
	Button::Update(dt);
	if (IsClick())
	{
		cout << "Click" << endl;
		((MapToolUiMgr*)uimgr)->Select(this);
	}
}

