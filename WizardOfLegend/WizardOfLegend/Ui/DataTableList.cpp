#include "DataTableList.h"
#include "../Framework/FrameWork.h"
#include "OptionButtons.h"
#include "../GameObject/Button2.h"
#include "../Framework/InputMgr.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/LanguageTable.h"
#include "../DataTable/SkillTable.h"
#include "../DataTable/SkillSetTable.h"

DataTableList::DataTableList()
	:buttons(nullptr), dataType(DataTable::Types::None)
{
}

DataTableList::~DataTableList()
{
}

void DataTableList::SetActive(bool active)
{
	Object::SetActive(active);
	if (active)
	{
		Vector2i windowSize = FRAMEWORK->GetWindowSize() / 2;
		window.create(VideoMode(windowSize.x, windowSize.y), "Game");
		Vector2f windowSizeF = (Vector2f)windowSize;
		view.setSize(windowSizeF);
		view.setCenter(windowSizeF * 0.5f);
		window.setView(view);
		Reset();
		InputMgr::Clear();
		while (window.isOpen())
		{
			deltaTime = clock.restart();
			float dt = deltaTime.asSeconds();

			InputMgr::Update(dt);
			Event ev;
			while (window.pollEvent(ev))
			{
				InputMgr::ProcessInput(ev);
			}
			if (ev.type == sf::Event::Closed)
			{
				window.close();
			}
			Update(dt);
			Draw();
		}
	}
	else
	{
		window.close();
		InputMgr::Clear();
	}
}

void DataTableList::Init()
{
	Object::Init();
	buttons = new OptionButtons();
	buttons->Init();
	enabled = false;
}

void DataTableList::Release()
{
	Object::Release();
	if (buttons != nullptr)
		delete buttons;
	buttons = nullptr;
}

void DataTableList::Reset()
{
}

void DataTableList::Update(float dt)
{
	if (InputMgr::GetKeyDown(Keyboard::Escape))
	{
		SetActive(false);
		return;
	}
	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
	for (auto button : buttons->GetButtons())
	{
		button->SetMousePos(mousePos);
	}
	buttons->Update(dt);
	auto lastBtn = buttons->GetButtons().back();
	auto btnBottom = lastBtn->GetPos().y + lastBtn->GetHitBounds().height;
	auto& originalCenter = view.getCenter();
	auto centerDistance = window.getSize().y * 0.5f;
	auto newCenter = Vector2f(originalCenter.x, originalCenter.y - InputMgr::GetMouseWheelMoved() * 50.f);
	if (newCenter.y < centerDistance)
		newCenter.y = centerDistance;
	if (btnBottom > window.getSize().y && newCenter.y > btnBottom - centerDistance)
		newCenter.y = btnBottom - centerDistance;
	view.setCenter(newCenter);
}

void DataTableList::Draw()
{
	window.clear();
	window.setView(view);
	buttons->Draw(window);
	window.display();
}

void DataTableList::SetDataTable(DataTable::Types type)
{
	buttons->Clear();
	dataType = type;
	Vector2i windowSize = FRAMEWORK->GetWindowSize() / 2;
	vector<string> keys;
	switch (type)
	{
	case DataTable::Types::Language:
		keys = DATATABLE_MGR->Get<LanguageTable>(type)->GetKeys();
		break;
	case DataTable::Types::Skill:
		keys = DATATABLE_MGR->Get<SkillTable>(type)->GetKeys();
		break;
	case DataTable::Types::SkillSet:
		keys = DATATABLE_MGR->Get<SkillSetTable>(type)->GetKeys();
		break;
	default:
		break;
	}
	for (int j = 0; j < keys.size(); ++j)
	{
		Button2* newButton = new Button2();
		buttons->AddButton(newButton, keys[j], {0.f, 0.f, (float)windowSize.x, 30.f});
		newButton->IsOtherView(true);
		newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
		newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
		newButton->ClickOn = bind(&DataTableList::EndSelection, this, keys[j]);
	}
}

void DataTableList::EndSelection(const string& str)
{
	if (Selected != nullptr)
		Selected(str);
	SetActive(false);
}
