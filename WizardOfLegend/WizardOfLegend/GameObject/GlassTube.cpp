#include "GlassTube.h"
#include "../Framework/ResourceMgr.h"
#include "../GameObject/SpriteObj.h"
#include "../GameObject/Player.h"
#include "../Ui/MessageUi.h"
#include "../Framework/InputMgr.h"

GlassTube::GlassTube()
	:isPlayerAdjacent(false)
{
}

GlassTube::~GlassTube()
{
}

void GlassTube::Init()
{
	Interactive::Init();

	if (GetFileName() == "graphics/Map/Object/tutorial_skill_1.png")
	{
		SetTexture(*RESOURCE_MGR->GetTexture("graphics/Map/Object/tutorial_skill_1.png"));
	}
	else if (GetFileName() == "graphics/Map/Object/tutorial_skill_2.png")
	{
		SetTexture(*RESOURCE_MGR->GetTexture("graphics/Map/Object/tutorial_skill_2.png"));
	}
	else if (GetFileName() == "graphics/Map/Object/tutorial_skill_3.png")
	{
		SetTexture(*RESOURCE_MGR->GetTexture("graphics/Map/Object/tutorial_skill_3.png"));
	}
	else if (GetFileName() == "graphics/Map/Object/tutorial_skill_4.png")
	{
		SetTexture(*RESOURCE_MGR->GetTexture("graphics/Map/Object/tutorial_skill_4.png"));
	}
	SetOrigin(Origins::BC);

	interactKey->SetPos({ GetPos().x, GetPos().y - GetGlobalBounds().height - 5.f });

	msgUi = new MessageUi();
	msgUi->Init();
}

void GlassTube::Update(float dt)
{
	Interactive::Update(dt);
}

void GlassTube::Draw(RenderWindow& window)
{
	Interactive::Draw(window);
	msgUi->Draw(window);
}

void GlassTube::Release()
{
	msgUi->Release();
}