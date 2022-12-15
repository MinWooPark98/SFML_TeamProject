#include "ItemBoxUi.h"
#include "../GameObject/SpriteObj.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/FrameWork.h"
#include "../GameObject/TextObj.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/Player.h"
#include "../Scene/SceneMgr.h"
#include "../GameObject/Item/ItemMgr.h"
#include "../GameObject/Interactive/ItemBox.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/RelicTable.h"
#include "../../DataTable/SavedDataTable.h"

ItemBoxUi::ItemBoxUi()
	:frame(nullptr), panel(nullptr), currPage(0), currRow(0), currColumn(0), indexChanged(false), currPlayerItem(nullptr), itemName(nullptr), itemIntro(nullptr), isMoving(true), moveSpeed(7200.f)
{
}

ItemBoxUi::~ItemBoxUi()
{
}

void ItemBoxUi::Init()
{
	frame = new SpriteObj();
	frame->Init();
	frame->SetTexture(*RESOURCE_MGR->GetTexture("graphics/RelicUISelectPageBG.png"));
	frame->SetOrigin(Origins::TC);
	frame->SetScale({ 4.f, 4.f });

	panel = new SpriteObj();
	panel->Init();
	panel->SetTexture(*RESOURCE_MGR->GetTexture("graphics/ItemInfoPanel.png"));
	panel->SetOrigin(Origins::TC);
	panel->SetColor(Color(50, 50, 50, 220));
	panel->SetScale({ 6.75f, 4.f });

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

	for (int i = 0; i < 2; ++i)
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
	keyInfos[1].second->SetOrigin(Origins::ML);

	Object::Init();
	SetActive(false);
}

void ItemBoxUi::Reset()
{
	Object::Reset();
}

void ItemBoxUi::Release()
{
	Object::Release();
}

void ItemBoxUi::Update(float dt)
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

	auto inputV = InputMgr::GetAxisDown(Axis::Vertical);
	auto inputH = InputMgr::GetAxisDown(Axis::Horizontal);
	if (inputV != 0 || inputH != 0)
	{
		if (inputV > 0 && currRow < collections[currPage].size() - 1)
		{
			CollectionHighLightOff();
			++currRow;
			if (collections[currPage][currRow].size() <= currColumn)
				currColumn = collections[currPage][currRow].size() - 1;
			CollectionHighLightOn();
		}
		else if (inputV < 0 && currRow > 0)
		{
			CollectionHighLightOff();
			--currRow;
			CollectionHighLightOn();
		}

		if (inputH > 0)
		{
			if (currColumn < collections[currPage][currRow].size() - 1)
			{
				CollectionHighLightOff();
				++currColumn;
				CollectionHighLightOn();
			}
			else if (currPage < collections.size() - 1)
			{
				CollectionHighLightOff();
				++currPage;
				currColumn = 0;
				if (currRow >= collections[currPage].size())
					currRow = 0;
				CollectionHighLightOn();
			}
		}
		else if (inputH < 0)
		{
			if (currColumn > 0)
			{
				CollectionHighLightOff();
				--currColumn;
				CollectionHighLightOn();
			}
			else if (currPage > 0)
			{
				CollectionHighLightOff();
				--currPage;
				currColumn = collections[currPage][currRow].size() - 1;
				CollectionHighLightOn();
			}
		}
		SetItemInfo();
	}

	if (InputMgr::GetKeyDown(Keyboard::Space))
		SelectItem();
	if (InputMgr::GetKeyDown(Keyboard::Escape))
		Disappear();
}

void ItemBoxUi::Draw(RenderWindow& window)
{
	Object::Draw(window);
	frame->Draw(window);
	panel->Draw(window);
	for (auto& row : collections[currPage])
	{
		for (auto& column : row)
		{
			column.image->Draw(window);
		}
	}
	itemName->Draw(window);
	itemIntro->Draw(window);
	for (auto& pair : keyInfos)
	{
		pair.first->Draw(window);
		pair.second->Draw(window);
	}
}

void ItemBoxUi::Reposition()
{
	auto& windowSize = FRAMEWORK->GetWindowSize();
	frame->SetPos(position);
	auto frameBnd = frame->GetGlobalBounds();
	panel->SetPos({ position.x, windowSize.y * 0.75f });
	auto panelBnd = panel->GetGlobalBounds();
	for (auto& page : collections)
	{
		for (int i = 0; i < page.size(); ++i)
		{
			for (int j = 0; j < page[i].size(); ++j)
			{
				page[i][j].image->SetPos({ frameBnd.left + frameBnd.width * (0.191f + 0.125f * j) , frameBnd.top + frameBnd.height * (0.2625f + 0.11f * i) });
			}
		}
	}
	itemName->SetPos({ panelBnd.left + panelBnd.width * 0.5f, panelBnd.top + 10.f });
	itemIntro->SetPos({ panelBnd.left + panelBnd.width * 0.5f, panelBnd.top + panelBnd.height * 0.45f });
	for (int i = 0; i < keyInfos.size(); ++i)
	{
		keyInfos[i].first->SetPos({ panelBnd.left + panelBnd.width * (0.2f + 0.5f * i), panelBnd.top + panelBnd.height - keyInfos[i].first->GetGlobalBounds().height * 0.5f - 5.f });
		auto infoIconBnd = keyInfos[i].first->GetGlobalBounds();
		keyInfos[i].second->SetPos({ infoIconBnd.left + infoIconBnd.width + 20.f, infoIconBnd.top + infoIconBnd.height * 0.5f });
	}
}

void ItemBoxUi::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	Reposition();
}

void ItemBoxUi::Translate(const Vector2f& delta)
{
	Object::Translate(delta);
	Reposition();
}

void ItemBoxUi::SetActive(bool active)
{
	Object::SetActive(active);
	auto currScene = SCENE_MGR->GetCurrentScene();
	if (active)
	{
		InputMgr::StackedOrderAdd(this);
		collections.clear();
		auto table = DATATABLE_MGR->Get<RelicTable>(DataTable::Types::Relic)->GetTable();
		map<int, pair<RelicTable::Locked, Relic::RelicInfo>> wholeTable;
		for (int i = 0; i < table.size(); ++i)
		{
			for (auto& info : table[(RelicTable::Locked)i])
			{
				wholeTable[info.first] = { (RelicTable::Locked)i, info.second };
			}
		}

		int wholeTablei = 0;
		int wholeTablej = 0;
		int wholeTablek = 0;
		for (auto& info : wholeTable)
		{
			if (collections.size() <= wholeTablei)
				collections.push_back(vector<vector<Collection>>());
			if (collections[wholeTablei].size() <= wholeTablej)
				collections[wholeTablei].push_back(vector<Collection>());
			auto newItem = new SpriteObj();
			newItem->Init();
			newItem->SetScale({ 3.75f, 4.f });
			newItem->SetUI(true);
			bool unlocked = false;
			if (info.second.first == RelicTable::Locked::Locked)
			{
				newItem->SetTexture(*RESOURCE_MGR->GetTexture("graphics/EmoteQuestion.png"));
				newItem->SetOrigin(Origins::MC);
			}
			else
			{
				newItem->SetTexture(*RESOURCE_MGR->GetTexture(info.second.second.iconDir));
				newItem->SetOrigin(Origins::MC);
				unlocked = true;
			}
			newItem->SetScale({ 4.f, 4.f });
			newItem->SetHitBox({ 0.f, 0.f, 80.f, 80.f }, Color::Transparent);
			newItem->GetHitBox().setOutlineThickness(5.f);
			newItem->GetHitBox().setOutlineColor(Color::Transparent);
			newItem->SetHitBoxOrigin(Origins::MC);
			newItem->SetDevMode(true);
			collections[wholeTablei][wholeTablej].push_back({ newItem, unlocked, info.second.second });
			++wholeTablek;
			if (wholeTablek > 5)
			{
				wholeTablek = 0;
				++wholeTablej;
				if (wholeTablej > 3)
				{
					wholeTablej = 0;
					++wholeTablei;
				}
			}
		}
		((ItemBox*)currScene->FindGameObj("ITEMBOX"))->SetState(ItemBox::States::Open);
		Reappear();
		auto itemMgr = ((Player*)currScene->FindGameObj("PLAYER"))->GetItemMgr();
		for (int i = 0; i < collections.size(); ++i)
		{
			for (int j = 0; j < collections[i].size(); ++j)
			{
				for (int k = 0; k < collections[i][j].size(); ++k)
				{
					if (collections[i][j][k].info.id == DATATABLE_MGR->Get<SavedDataTable>(DataTable::Types::SavedData)->Get().relicId)
					{
						currPage = i;
						currRow = j;
						currColumn = k;
						currPlayerItem = collections[i][j][k].image;
						currPlayerItem->GetHitBox().setOutlineColor(Color::Red);
						CollectionHighLightOn();
						SetItemInfo();
						return;
					}
				}
			}
		}
		currPage = 0;
		currRow = 0;
		currColumn = 0;
		CollectionHighLightOn();
		SetItemInfo();
	}
	else
	{
		InputMgr::StackedOrderRemove(this);
		auto itemBox = ((ItemBox*)currScene->FindGameObj("ITEMBOX"));
		if(itemBox != nullptr)
			itemBox->SetState(ItemBox::States::Close);
	}
}

void ItemBoxUi::Reappear()
{
	isMoving = true;
	auto& windowSize = FRAMEWORK->GetWindowSize();
	if (frame != nullptr)
		SetPos(Vector2f(-windowSize.x * 0.25f, 0.f));
	direction = { 1.f, 0.f };
}

void ItemBoxUi::Disappear()
{
	isMoving = true;
	direction = { -1.f, 0.f };
}

void ItemBoxUi::SetItemInfo()
{
	if (collections[currPage][currRow][currColumn].unlocked)
	{
		itemName->SetString(collections[currPage][currRow][currColumn].info.name);
		itemIntro->SetString(collections[currPage][currRow][currColumn].info.intro);
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

void ItemBoxUi::SelectItem()
{
	auto& currItem = collections[currPage][currRow][currColumn];
	if (currItem.unlocked)
	{
		if(currPlayerItem != nullptr)
			currPlayerItem->GetHitBox().setOutlineColor(Color::Transparent);
		auto player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
		auto itemMgr = player->GetItemMgr();
		if (itemMgr->GetRelicList().empty())
			itemMgr->AddRelic(currItem.info.id);
		else
			itemMgr->ChangeRelic(currItem.info.id, 0);
		currPlayerItem = currItem.image;
		currPlayerItem->GetHitBox().setOutlineColor(Color::Red);
		DATATABLE_MGR->Get<SavedDataTable>(DataTable::Types::SavedData)->ChangeRelic(currItem.info.id);
	}
}

void ItemBoxUi::CollectionHighLightOn()
{
	const auto& currItem = collections[currPage][currRow][currColumn];
	if (currItem.image == currPlayerItem)
		currItem.image->GetHitBox().setOutlineColor(Color::Green);
	else
		currItem.image->GetHitBox().setOutlineColor(Color::Yellow);
}

void ItemBoxUi::CollectionHighLightOff()
{
	const auto& currItem = collections[currPage][currRow][currColumn];
	if (currItem.image == currPlayerItem)
		currItem.image->GetHitBox().setOutlineColor(Color::Red);
	else
		currItem.image->GetHitBox().setOutlineColor(Color::Transparent);
}
