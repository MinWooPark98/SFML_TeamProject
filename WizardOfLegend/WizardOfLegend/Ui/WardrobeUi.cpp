#include "WardrobeUi.h"
#include "../GameObject/SpriteObj.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/FrameWork.h"
#include "../GameObject/TextObj.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/Player.h"
#include "../Scene/SceneMgr.h"
#include "../GameObject/Item/ItemMgr.h"
#include "../GameObject/Interactive/Wardrobe.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/HoodTable.h"

WardrobeUi::WardrobeUi()
	:frame(nullptr), display(nullptr), actionIdx(0), angle(0.f), currAngle(0.f), panel(nullptr), vecIdx(0), lastIdx(0), itemName(nullptr), itemIntro(nullptr), isMoving(true), moveSpeed(7200.f)
{
}

WardrobeUi::~WardrobeUi()
{
}

void WardrobeUi::Init()
{
	frame = new SpriteObj();
	frame->Init();
	frame->SetTexture(*RESOURCE_MGR->GetTexture("graphics/WardrobeUIBG.png"));
	frame->SetOrigin(Origins::TC);
	frame->SetScale({ 4.f, 4.f });

	display = new SpriteObj();
	display->Init();
	display->SetScale({ 4.f, 4.f });
	actions = { "graphics/WizardPBAoEDown3.png", "graphics/WizardKickRight2.png", "graphics/WizardGroundSlamDown5.png", "graphics/WizardBackHandRight2.png", "graphics/WizardDashDown.png" };
	auto& name = actions[Utils::RandomRange(0, actions.size())];
	display->SetTexture(*RESOURCE_MGR->GetTexture(name));
	display->SetSpriteShader();
	display->SetSpritePalette(64, "graphics/WizardPalette.png");
	display->SetOrigin(Origins::MC);

	panel = new SpriteObj();
	panel->Init();
	panel->SetTexture(*RESOURCE_MGR->GetTexture("graphics/WardrobeUITitleBG.png"));
	panel->SetOrigin(Origins::TC);
	panel->SetScale({ 20.f, 4.f });

	itemName = new TextObj();
	itemName->Init();
	itemName->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	itemName->SetSize(35);
	itemName->SetFillColor(Color::White);

	itemIntro = new TextObj();
	itemIntro->Init();
	itemIntro->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
	itemIntro->SetSize(25);
	itemIntro->SetFillColor(Color(155, 155, 155, 255));

	/*for (int i = 0; i < 2; ++i)
	{
		SpriteObj* icon = new SpriteObj();
		icon->Init();
		icon->SetScale({ 3.75f, 3.75f });
		TextObj* text = new TextObj();
		text->Init();
		text->SetFont(*RESOURCE_MGR->GetFont("fonts/NotoSansKR-Bold.otf"));
		text->SetSize(22);
		text->SetFillColor(Color(155, 155, 155, 255));
		keyInfos.push_back({ icon, text });
	}
	keyInfos[0].first->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Space.png"));
	keyInfos[0].first->SetOrigin(Origins::MC);
	keyInfos[0].second->SetString("ÀåÂø");
	keyInfos[0].second->AsciiToUnicode();
	keyInfos[0].second->SetOrigin(Origins::ML);
	keyInfos[1].first->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Escape.png"));
	keyInfos[1].first->SetOrigin(Origins::MC);
	keyInfos[1].second->SetString("´Ý±â");
	keyInfos[1].second->AsciiToUnicode();
	keyInfos[1].second->SetOrigin(Origins::ML);*/

	Object::Init();
	SetActive(false);
}

void WardrobeUi::Reset()
{
	Object::Reset();
}

void WardrobeUi::Release()
{
	Object::Release();
}

void WardrobeUi::Update(float dt)
{
	Object::Update(dt);
	if (isMoving)
	{
		Translate(direction * moveSpeed * dt);
		auto& windowSize = FRAMEWORK->GetWindowSize();
		if (direction.x < 0.f && position.x < -windowSize.x * 0.25f)
		{
			isMoving = false;
			SetActive(false);
		}
		else if (direction.x > 0.f && position.x > windowSize.x * 0.25f)
			isMoving = false;
		return;
	}

	if (!Utils::EqualFloat(0.f, angle))
	{
		currAngle += 8 * (angle > 0.f ? dt : -dt);
		auto frameBnd = frame->GetGlobalBounds();
		int collectionsSize = collections.size();
		auto center = Vector2f(frameBnd.left + frameBnd.width * 0.5f, frameBnd.top + frameBnd.height * 0.36f);
		for (int i = 0; i < collectionsSize; ++i)
		{
			collections[i].backImg->SetPos(center + 240.f * Vector2f(-sin(M_PI * 2 / collectionsSize * (i - vecIdx) + currAngle), -cos(M_PI * 2 / collectionsSize * (i - vecIdx) + currAngle)));
			collections[i].image->SetPos(collections[i].backImg->GetPos());
		}
		if (angle > 0.f && currAngle > angle)
		{
			CollectionHighLightOff();
			--vecIdx;
			if (vecIdx < 0)
				vecIdx = collections.size() - 1;
			FinishRoate();
		}
		else if (angle < 0.f && currAngle < angle)
		{
			CollectionHighLightOff();
			++vecIdx;
			if (vecIdx > collections.size() - 1)
				vecIdx = 0;
			FinishRoate();
		}
		return;
	}

	if (InputMgr::GetKeyDown(Keyboard::Space))
		ChangeHood();
	if (InputMgr::GetKeyDown(Keyboard::Escape))
		Disappear();

	auto inputH = InputMgr::GetAxisDown(Axis::Horizontal);
	if (inputH == 0)
		return;
	if (inputH != 0)
	{
		if (inputH < 0)
			angle = -2.f * M_PI / collections.size();
		else if (inputH > 0)
			angle = 2.f * M_PI / collections.size();
	}
}

void WardrobeUi::Draw(RenderWindow& window)
{
	Object::Draw(window);
	frame->Draw(window);
	if(collections[vecIdx].unlocked)
		display->Draw(window);
	panel->Draw(window);
	for (auto& collection : collections)
	{
		collection.backImg->Draw(window);
		collection.image->Draw(window);
	}
	itemName->Draw(window);
	itemIntro->Draw(window);
	/*for (auto& pair : keyInfos)
	{
		pair.first->Draw(window);
		pair.second->Draw(window);
	}*/
}

void WardrobeUi::Reposition()
{
	auto& windowSize = FRAMEWORK->GetWindowSize();
	frame->SetPos(position);
	auto frameBnd = frame->GetGlobalBounds();
	auto center = Vector2f(frameBnd.left + frameBnd.width * 0.5f, frameBnd.top + frameBnd.height * 0.36f);
	display->SetPos(center);
	panel->SetPos({ position.x, windowSize.y * 0.65f });
	auto panelBnd = panel->GetGlobalBounds();
	int collectionsSize = collections.size();
	for (int i = 0; i < collectionsSize; ++i)
	{
		collections[i].backImg->SetPos(center + 240.f * Vector2f(-sin(M_PI * 2 / collectionsSize * i), -cos(M_PI * 2 / collectionsSize * i)));
		collections[i].image->SetPos(collections[i].backImg->GetPos());
	}
	itemName->SetPos({ frameBnd.left + frameBnd.width * 0.5f, frameBnd.top + frameBnd.height * 0.655f });
	itemIntro->SetPos({ frameBnd.left + frameBnd.width * 0.5f, frameBnd.top + frameBnd.height * 0.75f });
	/*for (int i = 0; i < keyInfos.size(); ++i)
	{
		keyInfos[i].first->SetPos({ frameBnd.left + frameBnd.width * (0.2f + 0.5f * i), frameBnd.top + frameBnd.height * 0.9f });
		auto infoIconBnd = keyInfos[i].first->GetGlobalBounds();
		keyInfos[i].second->SetPos({ infoIconBnd.left + infoIconBnd.width + 20.f, infoIconBnd.top + infoIconBnd.height * 0.5f });
	}*/
}

void WardrobeUi::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	Reposition();
}

void WardrobeUi::Translate(const Vector2f& delta)
{
	Object::Translate(delta);
	Reposition();
}

void WardrobeUi::SetActive(bool active)
{
	Object::SetActive(active);
	auto currScene = SCENE_MGR->GetCurrentScene();
	if (active)
	{
		InputMgr::StackedOrderAdd(this);
		collections.clear();
		auto table = DATATABLE_MGR->Get<HoodTable>(DataTable::Types::Hood)->GetTable();
		map<int, pair<HoodTable::Locked, Hood::HoodInfo>> wholeTable;
		for (int i = 0; i < table.size(); ++i)
		{
			for (auto& info : table[(HoodTable::Locked)i])
			{
				wholeTable[info.first] = { (HoodTable::Locked)i, info.second };
			}
		}
		for (auto& info : wholeTable)
		{
			auto newBack = new SpriteObj();
			newBack->Init();
			newBack->SetScale({ 3.f, 3.f });
			auto newItem = new SpriteObj();
			newItem->Init();
			newItem->SetScale({ 3.f, 3.f });
			bool unlocked = false;
			if (info.second.first == HoodTable::Locked::Locked)
			{
				newBack->SetTexture(*RESOURCE_MGR->GetTexture("graphics/WardrobeUILocked.png"));
				newBack->SetOrigin(Origins::MC);
			}
			else
			{
				newBack->SetTexture(*RESOURCE_MGR->GetTexture("graphics/WardrobeUIOutfitBG.png"));
				newBack->SetOrigin(Origins::MC);
				newItem->SetTexture(*RESOURCE_MGR->GetTexture("graphics/WizardOutfitMannequin.png"));
				newItem->SetOrigin(Origins::MC);
				newItem->SetSpriteShader();
				newItem->SetSpritePalette(64, "graphics/WizardPalette.png");
				newItem->SetSpriteColor(info.second.second.paletteIdx);
				unlocked = true;
			}
			collections.push_back({ newBack, newItem, unlocked,info.second.second });
		}
		((Wardrobe*)currScene->FindGameObj("WARDROBE"))->SetState(Wardrobe::States::Open);
		Reappear();
	}
	else
	{
		((Wardrobe*)currScene->FindGameObj("WARDROBE"))->SetState(Wardrobe::States::Close);
		InputMgr::StackedOrderRemove(this);
	}
}

void WardrobeUi::Reappear()
{
	isMoving = true;
	auto& windowSize = FRAMEWORK->GetWindowSize();
	if (frame != nullptr)
		SetPos(Vector2f(-windowSize.x * 0.25f, windowSize.y * 0.05f));
	direction = { 1.f, 0.f };
}

void WardrobeUi::Disappear()
{
	isMoving = true;
	direction = { -1.f, 0.f };
}

void WardrobeUi::SetItemInfo()
{
	if (collections[vecIdx].unlocked)
	{
		itemName->SetString(collections[vecIdx].info.name);
		itemIntro->SetString(collections[vecIdx].info.intro);
	}
	else
	{
		itemName->SetString("???");
		itemIntro->SetString("Àá±è");
	}
	itemName->AsciiToUnicode();
	itemIntro->AsciiToUnicode();
	itemName->SetOrigin(Origins::TC);
	itemIntro->SetOrigin(Origins::MC);
}

void WardrobeUi::SelectItem()
{
	auto& currItem = collections[vecIdx];
	if (currItem.unlocked)
	{
		auto player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
		auto itemMgr = player->GetItemMgr();
		itemMgr->SetHood(currItem.info.id);
	}
}

void WardrobeUi::FinishRoate()
{
	currAngle = 0.f;
	angle = 0.f;
	while (true)
	{
		auto idx = Utils::RandomRange(0, actions.size());
		if (idx != actionIdx)
		{
			actionIdx = idx;
			break;
		}
	}
	display->SetTexture(*RESOURCE_MGR->GetTexture(actions[actionIdx]));
	display->SetSpriteColor(collections[vecIdx].info.paletteIdx);
	Utils::RandomRange(0, 2) < 1 ? display->SetScale({ 4.f, 4.f }) : display->SetScale({ -4.f, 4.f });
	display->SetOrigin(Origins::MC);
	CollectionHighLightOn();
	SetItemInfo();
}

void WardrobeUi::ChangeHood()
{
	if (collections[vecIdx].unlocked)
	{
		auto player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
		player->GetItemMgr()->SetHood(collections[vecIdx].info.id);
		collections[lastIdx].backImg->SetColor(Color::White);
		collections[vecIdx].backImg->SetColor({ 0, 255, 0, 255 });
		lastIdx = vecIdx;
	}
}

void WardrobeUi::CollectionHighLightOn()
{
	collections[vecIdx].backImg->SetScale({ 4.f, 4.f });
	collections[vecIdx].image->SetScale({ 4.f, 4.f });
}

void WardrobeUi::CollectionHighLightOff()
{
	collections[vecIdx].backImg->SetScale({ 3.f, 3.f });
	collections[vecIdx].image->SetScale({ 3.f, 3.f });
}
