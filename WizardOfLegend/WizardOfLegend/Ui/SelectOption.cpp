#include "SelectOption.h"
#include "../GameObject/TextObj.h"
#include "../GameObject/Button2.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/Framework.h"
#include "../Framework/InputMgr.h"
#include "../Scene/SceneMgr.h"
#include <fstream>
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/SkillTable.h"
#include "../GameObject/SkillSet.h"
#include "DataTableList.h"

SelectOption::SelectOption()
	:skillList(nullptr), skillSetList(nullptr)
{
}

SelectOption::~SelectOption()
{
}

void SelectOption::Init()
{
	auto windowSize = (Vector2f)FRAMEWORK->GetWindowSize();
	vector<string> opts = { "SkillName", "Element", "AttackType", "AttackCntLim", "AttackInterval", "Distance", "AttackShape", "Amplitude", "Frequency", "WaveType",  "FallingHeight", "RangeType", "PlayerAction", "StopMoving", "SkillDelay", "SkillCoolDown", "DmgType", "DmgRatio", "DmgDelay", "Duration", "Speed", "AnimClipName_1", "AnimClipName_2", "SoundName_1", "SoundName_2" };
	selectedSet.animClipName.assign(2, "");
	selectedSet.soundName.assign(2, "");
	float buttonHeight = windowSize.y * 0.8f / (int)Options::Count - 4.f;
	options.first = new OptionButtons();
	options.first->SetPos({ windowSize.x * 0.7f, 0.f });
	options.second = new OptionButtons();
	options.second->SetPos({ windowSize.x * 0.7f + 204.f, 0.f });
	for (int i = 0; i < (int)Options::Count; ++i)
	{
		Button2* button1 = new Button2();
		Button2* button2 = new Button2();
		button2->SetMouseOnSound(false);
		options.first->AddButton(button1, opts[i], { 0.f, 0.f, 200.f, buttonHeight });
		options.second->AddButton(button2, "", { 0.f, 0.f, windowSize.x * 0.3f - 202.f, buttonHeight });
		button2->MousePointerOn = bind(&Button2::FillBoxComplementaryColor, button2);
		button2->MousePointerOff = bind(&Button2::FillBoxInitColor, button2);

		switch ((Options)i)
		{
		case Options::Element:
			{
				OptionButtons* buttons = new OptionButtons();
				buttons->SetActive(false);
				buttons->SetPos(button2->GetPos() + Vector2f(0.f, buttonHeight));
				vector<string> btnStr = { "Fire", "Air", "Earth", "Lightning", "Water", "Chaos" };
				for (int j = 0; j < btnStr.size(); ++j)
				{
					Button2* newButton = new Button2();
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color(163, 204, 162, 255));
					newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
					newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
					newButton->ClickOn = bind(&SelectOption::ApplyOptBtn, this, (Options)i, buttons, newButton);
					newButton->ClickOff = bind(&OptionButtons::SetActive, buttons, false);
				}
				button2->ClickOn = bind(&OptionButtons::SetActive, buttons, true);
				optButtons.insert({ Options::Element, buttons });
			}
			break;
		case Options::AttackType:
			{
				OptionButtons* buttons = new OptionButtons();
				buttons->SetActive(false);
				buttons->SetPos(button2->GetPos() + Vector2f(0.f, buttonHeight));
				vector<string> btnStr = { "Single", "Multiple", "SaveAttacks" };
				for (int j = 0; j < btnStr.size(); ++j)
				{
					Button2* newButton = new Button2();
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color(163, 204, 162, 255));
					newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
					newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
					newButton->ClickOn = bind(&SelectOption::ApplyOptBtn, this, (Options)i, buttons, newButton);
					newButton->ClickOff = bind(&OptionButtons::SetActive, buttons, false);
				}
				button2->ClickOn = bind(&OptionButtons::SetActive, buttons, true);
				optButtons.insert({ Options::AttackType, buttons });
			}
			break;
		case Options::AttackShape:
			{
				OptionButtons* buttons = new OptionButtons();
				buttons->SetActive(false);
				buttons->SetPos(button2->GetPos() + Vector2f(0.f, buttonHeight));
				vector<string> btnStr = { "Surrounded", "Range", "Rotate", "Wave" };
				for (int j = 0; j < btnStr.size(); ++j)
				{
					Button2* newButton = new Button2();
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color(163, 204, 162, 255));
					newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
					newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
					newButton->ClickOn = bind(&SelectOption::ApplyOptBtn, this, (Options)i, buttons, newButton);
					newButton->ClickOff = bind(&OptionButtons::SetActive, buttons, false);
				}
				button2->ClickOn = bind(&OptionButtons::SetActive, buttons, true);
				optButtons.insert({ Options::AttackShape, buttons });
			}
			break;
		case Options::WaveType:
			{
				OptionButtons* buttons = new OptionButtons();
				buttons->SetActive(false);
				buttons->SetPos(button2->GetPos() + Vector2f(0.f, buttonHeight));
				vector<string> btnStr = { "None", "OneWay", "BackAndForth" };
				for (int j = 0; j < btnStr.size(); ++j)
				{
					Button2* newButton = new Button2();
					if(j == 0)
						buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color(103, 144, 102, 255));
					else
					{
						buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color(163, 204, 162, 255));
						newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
						newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
						newButton->ClickOn = bind(&SelectOption::ApplyOptBtn, this, (Options)i, buttons, newButton);
						newButton->ClickOff = bind(&OptionButtons::SetActive, buttons, false);
					}
				}
				button2->ClickOn = bind(&OptionButtons::SetActive, buttons, true);
				optButtons.insert({ Options::WaveType, buttons });
			}
			break;
		case Options::RangeType:
			{
				OptionButtons* buttons = new OptionButtons();
				buttons->SetActive(false);
				buttons->SetPos(button2->GetPos() + Vector2f(0.f, buttonHeight));
				vector<string> btnStr = { "None", "Default", "VerticalDescent", "FromAbovePlayer" };
				for (int j = 0; j < btnStr.size(); ++j)
				{
					Button2* newButton = new Button2();
					if (j == 0)
						buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color(103, 144, 102, 255));
					else
					{
						buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color(163, 204, 162, 255));
						newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
						newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
						newButton->ClickOn = bind(&SelectOption::ApplyOptBtn, this, (Options)i, buttons, newButton);
						newButton->ClickOff = bind(&OptionButtons::SetActive, buttons, false);
					}
				}
				button2->ClickOn = bind(&OptionButtons::SetActive, buttons, true);
				optButtons.insert({ Options::RangeType, buttons });
			}
			break;
		case Options::PlayerAction:
			{
				OptionButtons* buttons = new OptionButtons();
				buttons->SetActive(false);
				buttons->SetPos(button2->GetPos() + Vector2f(0.f, buttonHeight));
				vector<string> btnStr = { "NormalSpell", "Dash", "PBAoE", "Jump", "GroundSlam" };
				for (int j = 0; j < btnStr.size(); ++j)
				{
					Button2* newButton = new Button2();
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color(163, 204, 162, 255));
					newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
					newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
					newButton->ClickOn = bind(&SelectOption::ApplyOptBtn, this, (Options)i, buttons, newButton);
					newButton->ClickOff = bind(&OptionButtons::SetActive, buttons, false);
				}
				button2->ClickOn = bind(&OptionButtons::SetActive, buttons, true);
				optButtons.insert({ Options::PlayerAction, buttons });
			}
			break;
		case Options::StopMoving:
			{
				OptionButtons* buttons = new OptionButtons();
				buttons->SetActive(false);
				buttons->SetPos(button2->GetPos() + Vector2f(0.f, buttonHeight));
				vector<string> btnStr = { "Movable", "Immovable" };
				for (int j = 0; j < btnStr.size(); ++j)
				{
					Button2* newButton = new Button2();
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color(163, 204, 162, 255));
					newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
					newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
					newButton->ClickOn = bind(&SelectOption::ApplyOptBtn, this, (Options)i, buttons, newButton);
					newButton->ClickOff = bind(&OptionButtons::SetActive, buttons, false);
				}
				button2->ClickOn = bind(&OptionButtons::SetActive, buttons, true);
				optButtons.insert({ Options::StopMoving, buttons });
			}
			break;
		case Options::DmgType:
			{
				OptionButtons* buttons = new OptionButtons();
				buttons->SetActive(false);
				buttons->SetPos(button2->GetPos() + Vector2f(0.f, buttonHeight));
				vector<string> btnStr = { "Once", "Periodic", "NoDamage"};
				for (int j = 0; j < btnStr.size(); ++j)
				{
					Button2* newButton = new Button2();
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color(163, 204, 162, 255));
					newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
					newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
					newButton->ClickOn = bind(&SelectOption::ApplyOptBtn, this, (Options)i, buttons, newButton);
					newButton->ClickOff = bind(&OptionButtons::SetActive, buttons, false);
				}
				button2->ClickOn = bind(&OptionButtons::SetActive, buttons, true);
				optButtons.insert({ Options::DmgType, buttons });
			}
			break;
		default:
			{
				TextBox* newBox = new TextBox();
				newBox->Init();
				newBox->SetHitBox({ 0.f, 0.f, button2->GetHitBounds().width * 0.95f, buttonHeight * 0.8f }, Color(255, 255, 255, 255));
				newBox->SetPos(button2->GetPos() + Vector2f(5.f, buttonHeight * 0.1f));
				newBox->SendWritten = bind(&SelectOption::ApplyText, this, (Options)i, newBox);
				button2->ClickOn = bind(&TextBox::SetActive, newBox, true);
				textBoxs.insert({ (Options)i, newBox });
			}
			break;
		}
	}

	skillList = new DataTableList();
	skillList->Init();
	skillList->SetDataTable(DataTable::Types::Skill);
	skillList->Selected = bind(&SelectOption::Load, this, placeholders::_1);

	skillSetList = new DataTableList();
	skillSetList->Init();
	skillSetList->SetDataTable(DataTable::Types::SkillSet);
	skillSetList->Selected = bind(&SelectOption::LoadSkillSet, this, placeholders::_1);

	vector<vector<string>> menus = { { "New Skill", "Load Skill" }, { "Apply Skill", "Save Skill" }, { "Load SkillSet" } };
	buttonHeight = windowSize.y * 0.1f / menus.size() - 4.f;
	for (int i = 0; i < menus.size(); ++i)
	{
		OptionButtons* newMenu = new OptionButtons();
		newMenu->SetAxis(OptionButtons::Axis::Horizontal);
		newMenu->SetPos({ windowSize.x * 0.7f, windowSize.y * 0.85f + buttonHeight * i });
		for (int j = 0; j < menus[i].size(); ++j)
		{
			auto newButton = new Button2();
			newMenu->AddButton(newButton, menus[i][j], { 0.f, 0.f, windowSize.x * 0.3f / menus[i].size(), buttonHeight });
			newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
			newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
			if (menus[i][j] == "New Skill")
				newButton->ClickOn = bind(&SelectOption::Reset, this);
			else if (menus[i][j] == "Load Skill")
				newButton->ClickOn = bind(&DataTableList::SetActive, skillList, true);
			else if (menus[i][j] == "Apply Skill")
				newButton->ClickOn = bind(&SelectOption::SetPlayer1stSkill, this);
			else if(menus[i][j] == "Save Skill")
				newButton->ClickOn = bind(&SelectOption::SaveSetToCSV, this);
			else if (menus[i][j] == "Load SkillSet")
				newButton->ClickOn = bind(&DataTableList::SetActive, skillSetList, true);;
		}
		menuButtons.push_back(newMenu);
	}
}

void SelectOption::Release()
{
	Object::Release();
}

void SelectOption::Reset()
{
	Object::Reset();
	if (options.second != nullptr)
		options.second->Reset();
	for (auto& buttons : optButtons)
	{
		buttons.second->SetActive(false);
	}
	for (auto& box : textBoxs)
	{
		box.second->Reset();
		box.second->SetActive(false);
	}
	selectedSet.Reset();
}

void SelectOption::Update(float dt)
{
	Object::Update(dt);

	auto& windowSize = FRAMEWORK->GetWindowSize();
	if (InputMgr::GetMouseButtonDown(Mouse::Left) && InputMgr::GetMousePos().x < windowSize.x * 0.7f)
	{
		Player* player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
		auto skill = player->GetSkillSets()[0]->GetCurrSkill();
		if(skill != nullptr && skill->GetSetting()->playerAction == Player::SkillAction::Dash &&
			(player->GetState() == Player::States::Idle || player->GetState() == Player::States::Run))
			player->SetState(Player::States::Dash);
	}

	for (auto& buttons : menuButtons)
	{
		if (buttons->GetActive())
			buttons->Update(dt);
	}

	for (auto& buttons : optButtons)
	{
		if (buttons.second->GetActive())
		{
			buttons.second->Update(dt);
			return;
		}
	}
	for (auto& box : textBoxs)
	{
		if (box.second->GetActive())
		{
			box.second->Update(dt);
			return;
		}
	}
	if (options.first != nullptr)
		options.first->Update(dt);
	if (options.second != nullptr)
		options.second->Update(dt);
}

void SelectOption::Draw(RenderWindow& window)
{
	Object::Draw(window);
	if(options.first != nullptr)
		options.first->Draw(window);
	if (options.second != nullptr)
		options.second->Draw(window);
	for (auto& buttons : optButtons)
	{
		if (buttons.second->GetActive())
			buttons.second->Draw(window);
	}
	for (auto& box : textBoxs)
	{
		if(box.second->GetActive())
			box.second->Draw(window);
	}
	for (auto& buttons : menuButtons)
	{
		if (buttons->GetActive())
			buttons->Draw(window);
	}
}

void SelectOption::ActivateAll()
{
	auto& buttons = options.second->GetButtons();
	for (int i = 0; i < (int)Options::Count; ++i)
	{
		if (!buttons[i]->GetActivated())
		{
			buttons[i]->SetActivated(true);
			ApplyTextStr((Options)i, "");
			buttons[i]->GetText()->SetString("");
		}
	}
}

void SelectOption::ActivateBasedOnAtkShape()
{
	ActivateOption(Options::AttackType);
	ActivateOption(Options::Distance);
	ActivateOption(Options::Amplitude);
	ActivateOption(Options::Frequency);
	ActivateOption(Options::WaveType);
	ActivateOption(Options::FallingHeight);
	ActivateOption(Options::RangeType);
	ActivateOption(Options::Speed);
	optButtons[Options::AttackType]->GetButtons()[2]->SetActivated(true);
}

void SelectOption::ActivateOption(Options option)
{
	auto& buttons = options.second->GetButtons();
	if (buttons[(int)option]->GetActivated())
		return;
	buttons[(int)option]->SetActivated(true);
	ApplyTextStr(option, "");
	buttons[(int)option]->GetText()->SetString("");
}

void SelectOption::DeactivateOption(Options option)
{
	auto& buttons = options.second->GetButtons();
	buttons[(int)option]->SetActivated(false);
	ApplyTextStr(option, "0");
	ApplyOptBtnIdx(option, 0);
}

void SelectOption::ApplyText(Options opt, TextBox* box)
{
	string str = box->GetString();
	ApplyTextStr(opt, str);
}

void SelectOption::ApplyTextStr(Options opt, string str)
{
	try
	{
		switch (opt)
		{
		case SelectOption::Options::SkillName:
			selectedSet.skillName = str;
			break;
		case SelectOption::Options::AttackCntLim:
			ConvertVal(str, selectedSet.attackCntLim);
			break;
		case SelectOption::Options::AttackInterval:
			ConvertVal(str, selectedSet.attackInterval);
			break;
		case SelectOption::Options::Distance:
			ConvertVal(str, selectedSet.distance);
			break;
		case SelectOption::Options::Amplitude:
			ConvertVal(str, selectedSet.amplitude);
			break;
		case SelectOption::Options::Frequency:
			ConvertVal(str, selectedSet.frequency);
			break;
		case SelectOption::Options::FallingHeight:
			ConvertVal(str, selectedSet.fallingHeight);
			break;
		case SelectOption::Options::SkillDelay:
			ConvertVal(str, selectedSet.skillDelay);
			break;
		case SelectOption::Options::SkillCoolDown:
			ConvertVal(str, selectedSet.skillCoolDown);
			break;
		case SelectOption::Options::DmgRatio:
			ConvertVal(str, selectedSet.dmgRatio);
			break;
		case SelectOption::Options::DmgDelay:
			ConvertVal(str, selectedSet.dmgDelay);
			break;
		case SelectOption::Options::Duration:
			ConvertVal(str, selectedSet.duration);
			break;
		case SelectOption::Options::Speed:
			ConvertVal(str, selectedSet.speed);
			break;
		case SelectOption::Options::AnimClipName1:
			selectedSet.animClipName[0] = str;
			break;
		case SelectOption::Options::AnimClipName2:
			selectedSet.animClipName[1] = str;
			break;
		case SelectOption::Options::SoundName1:
			selectedSet.soundName[0] = str;
			break;
		case SelectOption::Options::SoundName2:
			selectedSet.soundName[1] = str;
			break;
		default:
			return;
		}
	}
	catch (exception expn)
	{
		// 잘못된 입력임을 나타내는 창 생성
		string lastStr;
		if (str.empty())
		{
			lastStr.clear();
			options.second->GetButtons()[(int)opt]->GetText()->SetString("");
		}
		else
			lastStr = options.second->GetButtons()[(int)opt]->GetText()->GetString();
		textBoxs[opt]->SetString(lastStr);
		return;
	}
	textBoxs[opt]->SetString(str);
	options.second->GetButtons()[(int)opt]->GetText()->SetString(str);
	options.second->GetButtons()[(int)opt]->Reposition();
}

void SelectOption::ApplyOptBtn(Options opt, OptionButtons* opts, Button2* btn)
{
	auto& buttons = opts->GetButtons();
	int i = 0;
	while (i < buttons.size())
	{
		if (buttons[i] == btn)
			break;
		++i;
	}
	if (i == buttons.size())
		return;

	ApplyOptBtnIdx(opt, i);
}

void SelectOption::ApplyOptBtnIdx(Options opt, int vecIdx)
{
	auto& buttons = options.second->GetButtons();
	switch (opt)
	{
	case SelectOption::Options::Element:
		selectedSet.element = (Skill::Element)vecIdx;
		break;
	case SelectOption::Options::AttackType:
		selectedSet.attackType = (Skill::AttackType)vecIdx;
		switch ((Skill::AttackType)vecIdx)
		{
		case Skill::AttackType::Single:
			// 어택카운트 1로 만드는 코드 추가
			DeactivateOption(Options::AttackInterval);
			break;
		default:
			ActivateOption(Options::AttackInterval);
			break;
		}
		break;
	case SelectOption::Options::AttackShape:
		selectedSet.attackShape = (Skill::AttackShape)vecIdx;
		ActivateBasedOnAtkShape();
		{
			switch ((Skill::AttackShape)vecIdx)
			{
			case Skill::AttackShape::Surrounded:
				DeactivateOption(Options::Distance);
				DeactivateOption(Options::Amplitude);
				DeactivateOption(Options::Frequency);
				DeactivateOption(Options::WaveType);
				DeactivateOption(Options::FallingHeight);
				DeactivateOption(Options::RangeType);
				DeactivateOption(Options::Speed);
				break;
			case Skill::AttackShape::Range:
				DeactivateOption(Options::WaveType);
				DeactivateOption(Options::Speed);
				if(buttons[(int)Options::AttackType]->GetText()->GetString() == "SaveAttacks")
					buttons[(int)Options::AttackType]->GetText()->SetString("");
				optButtons[Options::AttackType]->GetButtons()[2]->SetActivated(false);
				break;
			case Skill::AttackShape::Rotate:
				DeactivateOption(Options::Amplitude);
				DeactivateOption(Options::Frequency);
				DeactivateOption(Options::WaveType);
				DeactivateOption(Options::FallingHeight);
				DeactivateOption(Options::RangeType);
				break;
			case Skill::AttackShape::Wave:
				DeactivateOption(Options::Distance);
				DeactivateOption(Options::FallingHeight);
				DeactivateOption(Options::RangeType);
				break;
			default:
				break;
			}
		}
		break;
	case SelectOption::Options::WaveType:
		selectedSet.waveType = (Skill::WaveType)(vecIdx - 1);
		break;
	case SelectOption::Options::RangeType:
		selectedSet.rangeType = (Skill::RangeType)(vecIdx - 1);
		switch ((Skill::RangeType)(vecIdx - 1))
		{
		case Skill::RangeType::None:
			break;
		case Skill::RangeType::Default:
			DeactivateOption(Options::Frequency);
			DeactivateOption(Options::FallingHeight);
			break;
		default:
			ActivateOption(Options::Frequency);
			ActivateOption(Options::FallingHeight);
			break;
		}
		break;
	case SelectOption::Options::PlayerAction:
		selectedSet.playerAction = (Player::SkillAction)vecIdx;
		switch ((Player::SkillAction)vecIdx)
		{
		case Player::SkillAction::Dash:
			DeactivateOption(Options::StopMoving);
			break;
		case Player::SkillAction::Jump:
			DeactivateOption(Options::StopMoving);
			break;
		default:
			ActivateOption(Options::StopMoving);
			break;
		}
		break;
	case SelectOption::Options::StopMoving:
		selectedSet.stopMoving = (Skill::StopMoving)vecIdx;
		break;
	case SelectOption::Options::DmgType:
		selectedSet.dmgType = (Skill::DamageType)vecIdx;
		switch ((Skill::DamageType)vecIdx)
		{
		case Skill::DamageType::Once:
			ActivateOption(Options::DmgRatio);
			DeactivateOption(Options::DmgDelay);
			break;
		case Skill::DamageType::NoDamage:
			DeactivateOption(Options::DmgRatio);
			DeactivateOption(Options::DmgDelay);
			break;
		default:
			ActivateOption(Options::DmgRatio);
			ActivateOption(Options::DmgDelay);
			break;
		}
		break;
	default:
		return;
	}
	optButtons[opt]->SetActive(false);
	options.second->GetButtons()[(int)opt]->GetText()->SetString(optButtons[opt]->GetButtons()[vecIdx]->GetText()->GetString());
	options.second->GetButtons()[(int)opt]->Reposition();
}

void SelectOption::ConvertVal(string& str, int& opt)
{
	int n = stoi(str);
	opt = n;
	str = to_string(n);
}

void SelectOption::ConvertVal(string& str, float& opt)
{
	float f = stof(str);
	opt = f;
	str = to_string(f);
}

void SelectOption::SaveSetToCSV()
{
	auto& buttons = options.second->GetButtons();
	for (int i = 0; i < (int)Options::Count; ++i)
	{
		if ((Options)i == Options::SoundName1 || (Options)i == Options::SoundName2)
			continue;

		if (buttons[i]->GetText()->GetString().empty())
			return;
	}

	rapidcsv::Document doc("tables/skillTable.csv", rapidcsv::LabelParams(0, -1));

	auto skillNames = doc.GetColumn<string>(0);
	auto row = doc.GetRowCount();
	bool isExist = false;
	for (int i = 0; i < row; ++i)
	{
		if (skillNames[i] == selectedSet.skillName)
		{
			isExist = true;
			for (int j = 0; j < (int)Options::Count; ++j)
			{
				switch ((Options)j)
				{
				case SelectOption::Options::Element:
					doc.SetCell(j, i, (int)selectedSet.element);
					break;
				case SelectOption::Options::AttackType:
					doc.SetCell(j, i, (int)selectedSet.attackType);
					break;
				case SelectOption::Options::AttackShape:
					doc.SetCell(j, i, (int)selectedSet.attackShape);
					break;
				case SelectOption::Options::WaveType:
					doc.SetCell(j, i, (int)selectedSet.waveType);
					break;
				case SelectOption::Options::RangeType:
					doc.SetCell(j, i, (int)selectedSet.rangeType);
					break;
				case SelectOption::Options::PlayerAction:
					doc.SetCell(j, i, (int)selectedSet.playerAction);
					break;
				case SelectOption::Options::StopMoving:
					doc.SetCell(j, i, (int)selectedSet.stopMoving);
					break;
				case SelectOption::Options::DmgType:
					doc.SetCell(j, i, (int)selectedSet.dmgType);
					break;
				default:
					doc.SetCell(j, i, options.second->GetButtons()[j]->GetText()->GetString());
					break;
				}
			}
			break;
		}
	}
	if (!isExist)
	{
		for (int j = 0; j < (int)Options::Count; ++j)
		{
			switch ((Options)j)
			{
			case SelectOption::Options::Element:
				doc.SetCell(j, row, (int)selectedSet.element);
				break;
			case SelectOption::Options::AttackType:
				doc.SetCell(j, row, (int)selectedSet.attackType);
				break;
			case SelectOption::Options::AttackShape:
				doc.SetCell(j, row, (int)selectedSet.attackShape);
				break;
			case SelectOption::Options::WaveType:
				doc.SetCell(j, row, (int)selectedSet.waveType);
				break;
			case SelectOption::Options::RangeType:
				doc.SetCell(j, row, (int)selectedSet.rangeType);
				break;
			case SelectOption::Options::PlayerAction:
				doc.SetCell(j, row, (int)selectedSet.playerAction);
				break;
			case SelectOption::Options::StopMoving:
				doc.SetCell(j, row, (int)selectedSet.stopMoving);
				break;
			case SelectOption::Options::DmgType:
				doc.SetCell(j, row, (int)selectedSet.dmgType);
				break;
			default:
				doc.SetCell(j, row, options.second->GetButtons()[j]->GetText()->GetString());
				break;
			}
		}
	}
	doc.Save("tables/skillTable.csv");
}

void SelectOption::SetPlayer1stSkill()
{
	auto& buttons = options.second->GetButtons();
	for (int i = 0; i < (int)Options::Count; ++i)
	{
		if ((Options)i == Options::SoundName1 || (Options)i == Options::SoundName2)
			continue;

		if (buttons[i]->GetText()->GetString().empty())
			return;
	}
	Player* player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
	player->GetSkillSets()[0]->SetOnlyOneSkill(selectedSet);
}

void SelectOption::Load(const string& skillName)
{
	SkillTable* table = DATATABLE_MGR->Get<SkillTable>(DataTable::Types::Skill);
	table->Load();
	selectedSet = table->Get(skillName);
	ActivateAll();
	for (int i = 0; i < (int)SelectOption::Options::Count; ++i)
	{
		switch ((SelectOption::Options)i)
		{
		case SelectOption::Options::Element:
			ApplyOptBtnIdx(Options::Element, (int)selectedSet.element);
			break;
		case SelectOption::Options::AttackType:
			ApplyOptBtnIdx(Options::AttackType, (int)selectedSet.attackType);
			break;
		case SelectOption::Options::AttackShape:
			ApplyOptBtnIdx(Options::AttackShape, (int)selectedSet.attackShape);
			break;
		case SelectOption::Options::WaveType:
			ApplyOptBtnIdx(Options::WaveType, (int)selectedSet.waveType + 1);
			break;
		case SelectOption::Options::RangeType:
			ApplyOptBtnIdx(Options::RangeType, (int)selectedSet.rangeType + 1);
			break;
		case SelectOption::Options::PlayerAction:
			ApplyOptBtnIdx(Options::PlayerAction, (int)selectedSet.playerAction);
			break;
		case SelectOption::Options::StopMoving:
			ApplyOptBtnIdx(Options::StopMoving, (int)selectedSet.stopMoving);
			break;
		case SelectOption::Options::DmgType:
			ApplyOptBtnIdx(Options::DmgType, (int)selectedSet.dmgType);
			break;
		case SelectOption::Options::SkillName:
			textBoxs[Options::SkillName]->SetString(selectedSet.skillName);
			options.second->GetButtons()[i]->GetText()->SetString(selectedSet.skillName);
			break;
		case SelectOption::Options::AttackCntLim:
			textBoxs[Options::AttackCntLim]->SetString(to_string(selectedSet.attackCntLim));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.attackCntLim));
			break;
		case SelectOption::Options::AttackInterval:
			textBoxs[Options::AttackInterval]->SetString(to_string(selectedSet.attackInterval));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.attackInterval));
			break;
		case SelectOption::Options::Distance:
			textBoxs[Options::Distance]->SetString(to_string(selectedSet.distance));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.distance));
			break;
		case SelectOption::Options::Amplitude:
			textBoxs[Options::Amplitude]->SetString(to_string(selectedSet.amplitude));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.amplitude));
			break;
		case SelectOption::Options::Frequency:
			textBoxs[Options::Frequency]->SetString(to_string(selectedSet.frequency));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.frequency));
			break;
		case SelectOption::Options::FallingHeight:
			textBoxs[Options::FallingHeight]->SetString(to_string(selectedSet.fallingHeight));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.fallingHeight));
			break;
		case SelectOption::Options::SkillDelay:
			textBoxs[Options::SkillDelay]->SetString(to_string(selectedSet.skillDelay));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.skillDelay));
			break;
		case SelectOption::Options::SkillCoolDown:
			textBoxs[Options::SkillCoolDown]->SetString(to_string(selectedSet.skillCoolDown));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.skillCoolDown));
			break;
		case SelectOption::Options::DmgRatio:
			textBoxs[Options::DmgRatio]->SetString(to_string(selectedSet.dmgRatio));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.dmgRatio));
			break;
		case SelectOption::Options::DmgDelay:
			textBoxs[Options::DmgDelay]->SetString(to_string(selectedSet.dmgDelay));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.dmgDelay));
			break;
		case SelectOption::Options::Duration:
			textBoxs[Options::Duration]->SetString(to_string(selectedSet.duration));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.duration));
			break;
		case SelectOption::Options::Speed:
			textBoxs[Options::Speed]->SetString(to_string(selectedSet.speed));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.speed));
			break;
		case SelectOption::Options::AnimClipName1:
			textBoxs[Options::AnimClipName1]->SetString(selectedSet.animClipName[0]);
			options.second->GetButtons()[i]->GetText()->SetString(selectedSet.animClipName[0]);
			break;
		case SelectOption::Options::AnimClipName2:
			textBoxs[Options::AnimClipName2]->SetString(selectedSet.animClipName[1]);
			options.second->GetButtons()[i]->GetText()->SetString(selectedSet.animClipName[1]);
			break;
		case SelectOption::Options::SoundName1:
			textBoxs[Options::SoundName1]->SetString(selectedSet.soundName[0]);
			options.second->GetButtons()[i]->GetText()->SetString(selectedSet.soundName[0]);
			break;
		case SelectOption::Options::SoundName2:
			textBoxs[Options::SoundName2]->SetString(selectedSet.soundName[1]);
			options.second->GetButtons()[i]->GetText()->SetString(selectedSet.soundName[1]);
			break;
		default:
			break;
		}
	}
	for (auto button : options.second->GetButtons())
	{
		button->Reposition();
	}
	SetPlayer1stSkill();
}

void SelectOption::LoadSkillSet(const string& skillSetName)
{
	Player* player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("PLAYER");
	player->SetSkillSet(0, skillSetName);
}
