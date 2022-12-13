#include "MessageUi.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/SoundMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/Framework.h"
#include "../GameObject/TextObj.h"
#include "../DataTable/NpcTalkTable.h"
#include "../DataTable/DataTableMGR.h"

MessageUi::MessageUi()
	: windowSize(FRAMEWORK->GetWindowSize())
{
}

MessageUi::~MessageUi()
{
}

void MessageUi::Init()
{
	SpriteObj::Init();
	isTalk = false;
	// 3까지 메세지 틀
	// 4 = npc 이미지, 5 = 플레이어 이미지
	for (int i = 0; i < 6; i++)
		massageImages.push_back(new SpriteObj());

	for (int i = 0; i < massageImages.size(); i++)
	{
		massageImages[i]->Init();

		switch (i)
		{
		case 0:
			massageImages[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/ItemInfoPanel.png")); // 대화창 바깥 판넬
			massageImages[i]->SetScale({ 12.7, 4.6 });
			massageImages[i]->SetPos({ windowSize.x * 0.5f, windowSize.y * 0.8f });
			massageImages[i]->SetColor({50, 50, 50});
			break;
		case 1:
			massageImages[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/ItemInfoPanel.png")); // 대화창 안쪽 판넬
			massageImages[i]->SetScale({ 9, 4 });
			massageImages[i]->SetPos({ windowSize.x * 0.45f, windowSize.y * 0.8f });
			massageImages[i]->SetColor({ 120, 120, 120 });
			break;
		case 2:
			massageImages[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Panel.png")); // npc 판넬
			massageImages[i]->SetScale({ 2.3, 2.3 });
			massageImages[i]->SetPos({ windowSize.x * 0.15f, windowSize.y * 0.8f });
			massageImages[i]->SetColor({ 160, 160, 160 });
			break;
		case 3:
			massageImages[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Panel.png")); // 플레이어 판넬
			massageImages[i]->SetScale({ 1.5, 1.5 });
			massageImages[i]->SetPos({ windowSize.x * 0.85f, windowSize.y * 0.8f });
			massageImages[i]->SetColor({ 80, 80, 80 });
			break;
		case 4:
			massageImages[i]->SetPos({ windowSize.x * 0.15f, windowSize.y * 0.8f }); // npc 이미지
			break;
		case 5:
			massageImages[i]->SetPos({ windowSize.x * 0.85f, windowSize.y * 0.8f }); // 플레이어 이미지
			massageImages[i]->UseShader();
			massageImages[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Portrait.png"));
			massageImages[i]->SetScale({ 4.3, 4.3 });
			massageImages[i]->SetSpriteShader();
			massageImages[i]->SetSpritePalette(32, "graphics/WizardPalette.png");
			massageImages[i]->SetFlipX(1);
			break;
		}

		massageImages[i]->SetOrigin(Origins::MC);
		massageImages[i]->SetActive(false);
	}

	text = new TextObj();
	text->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	text->SetFillColor(Color::White);
	text->SetText(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"), 25, Color::White, "");
	text->SetOutlineColor(Color::Black);
	text->SetOutlineThickness(1.f);
	text->SetActive(false);


	npcName = new TextObj();
	npcName->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	npcName->SetFillColor(Color::White);
	npcName->SetText(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"), 40, Color::White, "");
	npcName->SetPos({ windowSize.x * 0.47f, windowSize.y * 0.705f });
	npcName->SetOutlineColor(Color::Black);
	npcName->SetOutlineThickness(1.f);
	npcName->SetActive(false);


	spaceBarImage = new SpriteObj();
	spaceBarImage->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Space.png"));
	spaceBarImage->SetScale({ 3, 3 });
	spaceBarImage->SetPos({ windowSize.x * 0.725f, windowSize.y * 0.87f });
	spaceBarImage->SetOrigin(Origins::MC);

	playerShader.setFillColor({0, 0, 0, 180});
	playerShader.setSize(massageImages[5]->GetSize() * 4.3f);
	playerShader.setPosition(massageImages[5]->GetPos());
	playerShader.setOrigin(playerShader.getSize() * 0.5f);

	auto npcTalkTable = DATATABLE_MGR->Get<NpcTalkTable>(DataTable::Types::MonsterProperty);
	auto& npc = npcTalkTable->GetTable();
}

void MessageUi::Update(float dt)
{
	SpriteObj::Update(dt);

	if (isTalk)
	{
		InputMgr::StackedOrderAdd(this);

		enabledTime -= dt;

		if (enabledTime <= 0.f)
		{
			UiEnabled(true);
		}
	}
	else
		InputMgr::StackedOrderRemove(this);
}

void MessageUi::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);

	if (massageImages[0]->GetActive())
	{
		for (int i = 0; i < massageImages.size(); i++)
		{
			massageImages[i]->Draw(window);
		}

		spaceBarImage->Draw(window);
		text->Draw(window);

		npcName->Draw(window);
		window.draw(playerShader);
	}
}

void MessageUi::Release()
{
	InputMgr::StackedOrderRemove(this);
	for (auto& messages : massageImages)
	{
		if (messages != nullptr)
			delete messages;
		messages = nullptr;
	}
	massageImages.clear();

	if (text != nullptr)
		delete text;
	text = nullptr;

	if (npcName != nullptr)
		delete npcName;
	npcName = nullptr;

	if (spaceBarImage != nullptr)
		delete spaceBarImage;
	spaceBarImage = nullptr;
}

void MessageUi::UiEnabled(bool set)
{
	if (massageImages.size() != 0)
	{
		for (int i = 0; i < massageImages.size(); i++)
		{
			if (massageImages[i]->GetActive() != set)
				massageImages[i]->SetActive(set);
		}
	}

	if (spaceBright != 255)
	{
		spaceBright += 1;
		Color fadeColor = spaceBarImage->GetSprite().getColor();
		fadeColor.a = spaceBright;
		spaceBarImage->SetColor(fadeColor);
	}
}

void MessageUi::SetTexts(const string& string)
{
	text->SetString(string);
	text->SetPos({ windowSize.x * 0.25f, windowSize.y * 0.75f });
	text->AsciiToUnicode();
}

void MessageUi::SetNpcName(const string& name)
{
	npcName->SetText(name);
	npcName->AsciiToUnicode();
	npcName->SetOrigin(Origins::TC);
}

void MessageUi::SetNpcImage(const string& imageName)
{
	if (massageImages[4] != nullptr)
		massageImages[4]->SetTexture(*RESOURCE_MGR->GetTexture(imageName));
	massageImages[4]->SetScale({6.3, 6.3});
	massageImages[4]->SetOrigin(Origins::MC);
}

void MessageUi::SetPlayerImage(int playerPaletteColor)
{
	massageImages[5]->SetPaletteColor(playerPaletteColor);
}

void MessageUi::MessageUiOn()
{
	UiEnabled(true);
	SetIsTalk(true);
}

void MessageUi::Talk()
{
	if (InputMgr::GetKeyDown(Keyboard::Space))
	{
		UiEnabled(false);
		enabledTime = 0.05f;
		spaceBright = 0;
	}
}

void MessageUi::SetTalkTable(string npcName, vector<string> npcTalk)
{
	names.push_back(npcName);

	for (int i = 0; i < npcTalk.size(); i++)
		tables.push_back(npcTalk[i]);
}
