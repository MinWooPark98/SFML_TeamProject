#include "MessageUi.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/SoundMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/Framework.h"
#include "../GameObject/TextObj.h"

MessageUi::MessageUi()
	: windowSize(FRAMEWORK->GetWindowSize()), textXY(0.f, 0.f)
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

	for (int i = 0; i < 150; i++)
	{
		texts.push_back(new TextObj());
	}
	for (int i = 0; i < texts.size(); i++)
	{
		texts[i]->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
		texts[i]->SetFillColor(Color::White);
		texts[i]->SetText(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"), 30, Color::White, "");
		texts[i]->SetOutlineColor(Color::Black);
		texts[i]->SetOutlineThickness(1.f);
		texts[i]->SetActive(false);
	}
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
}

void MessageUi::Update(float dt)
{
	SpriteObj::Update(dt);

	// 상호작용이랑 연결
	//if (InputMgr::GetKeyDown(Keyboard::F))
	//{
	//	// 설정할 곳에서 사용할 코드
	//	{
	//		// 임시 테스트용 string
	//		vector<string> str;
	//		for (int i = 0; i < 20; i++)
	//			str.push_back("");

	//		SetTexts(str);

	//		string devName = "박민우의  볼케이노  순삭  쇼";
	//		SetNpcName(devName);
	//		SetPlayerImage(63);
	//		SetNpcImage("graphics/HumanKnightPortrait.png");

	//		UiEnabled(true);
	//		SetIsTalk(true);
	//	}
	//}

	if (isTalk)
	{
		InputMgr::StackedOrderAdd(this);

		enabledTime -= dt;

		if (InputMgr::GetKeyDown(Keyboard::Space))
		{
			UiEnabled(false);

			// 설정할 곳에서 사용할 코드
			{
				// 임시 테스트용 string
				vector<string> string;
				for (int i = 0; i < 100; i++)
					string.push_back("냠");

				SetTexts(string);
			}

			enabledTime = 0.1f;
			spaceBright = 0;
		}

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
		
		for (int i = 0; i < texts.size(); i++)
			texts[i]->Draw(window);

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

	for (auto& text : texts)
	{
		if (text != nullptr)
			delete text;
		text = nullptr;
	}
	texts.clear();

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

void MessageUi::SetTexts(vector<string> strings)
{
	for (int i = 0; i < texts.size(); i++)
	{
		texts[i]->SetText("");
	}

	for (int i = 0; i < strings.size(); i++)
	{
		if ((windowSize.x * 0.25 + (textXY.x * 30.f)) <= windowSize.x * 0.68 && i != 0)
		{
			textXY.x += 1.f;
		}
		else
		{
			textXY.x = 0.f;
			textXY.y += 35.f;
		}

		texts[i]->SetString(strings[i]);
		texts[i]->SetPos({ windowSize.x * 0.25f + (textXY.x * 30.f), windowSize.y * 0.73f + textXY.y });
	}

	textXY = { 0, 0 };

	for (int i = 0; i < texts.size(); i++)
		texts[i]->AsciiToUnicode();
}

void MessageUi::SetNpcName(string name)
{
	npcName->SetText(name);
	npcName->AsciiToUnicode();
	npcName->SetOrigin(Origins::TC);
}

void MessageUi::SetNpcImage(string imageName)
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
