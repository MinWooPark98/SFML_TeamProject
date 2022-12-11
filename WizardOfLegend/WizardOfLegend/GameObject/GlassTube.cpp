#include "GlassTube.h"
#include "../Framework/ResourceMgr.h"
#include "../GameObject/SpriteObj.h"


GlassTube::GlassTube()
	:isPlayerAdjacent(false)
{
}

GlassTube::~GlassTube()
{
}

void GlassTube::Init()
{

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
	FKey=new SpriteObj();
	FKey->SetTexture(*RESOURCE_MGR->GetTexture("graphics/F.png"));
	FKey->SetOrigin(Origins::MC);
	FKey->SetPos({ GetPos().x-GetGlobalBounds().height-5.f, GetPos().y});
	FKey->SetActive(false);

}

void GlassTube::Update(float dt)
{
	SpriteObj::Update(dt);
	FKey->Update(dt);
	if (isPlayerAdjacent)
	{
		FKey->SetActive(true);
	}
	else
	{
		FKey->SetActive(false);
	}
}

void GlassTube::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
	FKey->Draw(window);
}

void GlassTube::SetIsPlayerAdjacent(bool ad)
{
	isPlayerAdjacent = ad;
}

