#include "GlassTube.h"
#include "../Framework/ResourceMgr.h"
#include "../GameObject/SpriteObj.h"
#include "../GameObject/Player.h"
#include "../Ui/MessageUi.h"
#include "../Framework/InputMgr.h"
#include "../Scene/SceneMgr.h"
#include "../DataTable/NpcTalkTable.h"
#include "../DataTable/DataTableMGR.h"

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

	auto messageTable = DATATABLE_MGR->Get<NpcTalkTable>(DataTable::Types::NpcTalk);

	if (GetFileName() == "graphics/Map/Object/tutorial_skill_2.png")
	{
		SetGlass(messageTable, "graphics/Map/Object/tutorial_skill_2.png", "���� ȭ����");
	}
	else if (GetFileName() == "graphics/Map/Object/tutorial_skill_1.png")
	{
		SetGlass(messageTable, "graphics/Map/Object/tutorial_skill_1.png", "���� �Ҽ�");
	}
	else if (GetFileName() == "graphics/Map/Object/tutorial_skill_3.png")
	{
		SetGlass(messageTable, "graphics/Map/Object/tutorial_skill_3.png", "���� ���� ����");
	}
	else if (GetFileName() == "graphics/Map/Object/tutorial_skill_4.png")
	{
		SetGlass(messageTable, "graphics/Map/Object/tutorial_skill_4.png", "�ٶ����� ����");
	}
	SetOrigin(Origins::BC);

	interactKey->SetPos({ GetPos().x, GetPos().y - GetGlobalBounds().height - 5.f });
}

void GlassTube::Update(float dt)
{
	Interactive::Update(dt);

	auto player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
	if (player != nullptr)
	{
		if (GetHitBounds().intersects(player->GetHitBounds()))
			isPlayerAdjacent = true;
		else
			isPlayerAdjacent = false;
	}
}

void GlassTube::Draw(RenderWindow& window)
{
	Interactive::Draw(window);
}

void GlassTube::SetGlass(NpcTalkTable* table, string image, string npcName)
{
	SetTexture(*RESOURCE_MGR->GetTexture(image));
	auto& p = table->Get(npcName);
	name = table->GetNpcName(npcName);
	for (int i = 0; i < p.size(); i++)
		SetMessage(p[i]);
}
