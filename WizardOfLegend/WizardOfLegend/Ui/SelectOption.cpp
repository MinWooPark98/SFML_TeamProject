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

SelectOption::SelectOption()
{
}

SelectOption::~SelectOption()
{
}

void SelectOption::Init()
{
	auto windowSize = (Vector2f)FRAMEWORK->GetWindowSize();
	vector<string> opts = { "SkillName", "Element", "AttackType", "AttackCntLim", "AttackInterval", "Distance", "AttackShape", "Amplitude", "Frequency", "MoveType", "PlayerAction", "SkillDelay", "SkillCoolDown", "DmgRatio", "DmgType", "DmgDelay", "Duration", "Speed", "AnimClipName_1", "AnimClipName_2" };
	selectedSet.animClipName.assign(2, "");
	float buttonHeight = windowSize.y * 0.8f / (int)Options::Count - 4.f;
	options.first = new OptionButtons();
	options.first->SetPos({ windowSize.x * 0.7f, 0.f });
	options.second = new OptionButtons();
	options.second->SetPos({ windowSize.x * 0.7f + 204.f, 0.f });
	for (int i = 0; i < (int)Options::Count; ++i)
	{
		Button2* button1 = new Button2();
		Button2* button2 = new Button2();
		options.first->AddButton(button1, opts[i], { 0.f, 0.f, 200.f, buttonHeight });
		options.second->AddButton(button2, "", { 0.f, 0.f, windowSize.x * 0.3f - 202.f, buttonHeight });
		button2->MousePointerOn = bind(&Button2::ChangeFillColor, button2);
		button2->MousePointerOff = bind(&Button2::ChangeFillColor, button2);

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
				}
				button2->ClickOn = bind(&OptionButtons::SetActive, buttons, true);
				optButtons.push_back(buttons);
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
				}
				button2->ClickOn = bind(&OptionButtons::SetActive, buttons, true);
				optButtons.push_back(buttons);
			}
			break;
		case Options::AttackShape:
			{
				OptionButtons* buttons = new OptionButtons();
				buttons->SetActive(false);
				buttons->SetPos(button2->GetPos() + Vector2f(0.f, buttonHeight));
				vector<string> btnStr = { "Range", "Rotate", "Wave" };
				for (int j = 0; j < btnStr.size(); ++j)
				{
					Button2* newButton = new Button2();
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color(163, 204, 162, 255));
					newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
					newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
					newButton->ClickOn = bind(&SelectOption::ApplyOptBtn, this, (Options)i, buttons, newButton);
				}
				button2->ClickOn = bind(&OptionButtons::SetActive, buttons, true);
				optButtons.push_back(buttons);
			}
			break;
		case Options::MoveType:
			{
				OptionButtons* buttons = new OptionButtons();
				buttons->SetActive(false);
				buttons->SetPos(button2->GetPos() + Vector2f(0.f, buttonHeight));
				vector<string> btnStr = { "OneWay", "BackAndForth" };
				for (int j = 0; j < btnStr.size(); ++j)
				{
					Button2* newButton = new Button2();
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color(163, 204, 162, 255));
					newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
					newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
					newButton->ClickOn = bind(&SelectOption::ApplyOptBtn, this, (Options)i, buttons, newButton);
				}
				button2->ClickOn = bind(&OptionButtons::SetActive, buttons, true);
				optButtons.push_back(buttons);
			}
			break;
		case Options::PlayerAction:
			{
				OptionButtons* buttons = new OptionButtons();
				buttons->SetActive(false);
				buttons->SetPos(button2->GetPos() + Vector2f(0.f, buttonHeight));
				vector<string> btnStr = { "NormalSpell", "PBAoE", "JumpSlash" };
				for (int j = 0; j < btnStr.size(); ++j)
				{
					Button2* newButton = new Button2();
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color(163, 204, 162, 255));
					newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
					newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
					newButton->ClickOn = bind(&SelectOption::ApplyOptBtn, this, (Options)i, buttons, newButton);
				}
				button2->ClickOn = bind(&OptionButtons::SetActive, buttons, true);
				optButtons.push_back(buttons);
			}
			break;
		case Options::DmgType:
			{
				OptionButtons* buttons = new OptionButtons();
				buttons->SetActive(false);
				buttons->SetPos(button2->GetPos() + Vector2f(0.f, buttonHeight));
				vector<string> btnStr = { "Once", "Periodic" };
				for (int j = 0; j < btnStr.size(); ++j)
				{
					Button2* newButton = new Button2();
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color(163, 204, 162, 255));
					newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
					newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
					newButton->ClickOn = bind(&SelectOption::ApplyOptBtn, this, (Options)i, buttons, newButton);
				}
				button2->ClickOn = bind(&OptionButtons::SetActive, buttons, true);
				optButtons.push_back(buttons);
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
				textBoxs.push_back(newBox);
			}
			break;
		}
	}
	vector<string> menus[] = { {"New", "Load"}, {"Apply", "Save"} };
	for (int i = 0; i < menus->size(); ++i)
	{
		OptionButtons* newMenu = new OptionButtons();
		newMenu->SetAxis(OptionButtons::Axis::Horizontal);
		newMenu->SetPos({ windowSize.x * 0.7f, windowSize.y * 0.85f + buttonHeight * i });
		for (int j = 0; j < menus[i].size(); ++j)
		{
			auto newButton = new Button2();
			newMenu->AddButton(newButton, menus[i][j], { 0.f, 0.f, windowSize.x * 0.15f, buttonHeight });
			newButton->MousePointerOn = bind(&Button2::DefaultMouseOn, newButton);
			newButton->MousePointerOff = bind(&Button2::DefaultMouseOff, newButton);
			if (menus[i][j] == "New")
				newButton->ClickOn = bind(&SelectOption::Reset, this);
			else if (menus[i][j] == "Load")
				;
			else if (menus[i][j] == "Apply")
				newButton->ClickOn = bind(&SelectOption::SetPlayer1stSkill, this);
			else if(menus[i][j] == "Save")
				newButton->ClickOn = bind(&SelectOption::SaveSetToCSV, this);
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
		buttons->SetActive(false);
	}
	for (auto& box : textBoxs)
	{
		box->Reset();
		box->SetActive(false);
	}
	selectedSet.Reset();
}

void SelectOption::Update(float dt)
{
	Object::Update(dt);

	if (InputMgr::GetKeyDown(Keyboard::F5))
		Load("DragonArc");
	else if (InputMgr::GetKeyDown(Keyboard::F6))
		Load("FireFull");
	else if (InputMgr::GetKeyDown(Keyboard::F7))
		Load("FireBall");

	for (auto& buttons : menuButtons)
	{
		if (buttons->GetActive())
			buttons->Update(dt);
	}

	for (auto& buttons : optButtons)
	{
		if (buttons->GetActive())
		{
			buttons->Update(dt);
			return;
		}
	}
	for (auto& box : textBoxs)
	{
		if (box->GetActive())
		{
			box->Update(dt);
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
		if (buttons->GetActive())
			buttons->Draw(window);
	}
	for (auto& box : textBoxs)
	{
		if(box->GetActive())
			box->Draw(window);
	}
	for (auto& buttons : menuButtons)
	{
		if (buttons->GetActive())
			buttons->Draw(window);
	}
}

void SelectOption::ApplyText(Options opt, TextBox* box)
{
	string str = box->GetString();
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
		default:
			break;
		}
	}
	catch (exception expn)
	{
		// 잘못된 입력임을 나타내는 창 생성
		string lastStr = options.second->GetButtons()[(int)opt]->GetText()->GetString();
		box->SetString(lastStr);
		return;
	}
	box->SetString(str);
	if (str.empty())
		options.second->GetButtons()[(int)opt]->GetText()->SetString("");
	else
		options.second->GetButtons()[(int)opt]->GetText()->SetString(str);
	options.second->GetButtons()[(int)opt]->Reposition();
}

void SelectOption::ApplyOptBtn(Options opt, OptionButtons* opts, Button2* btn)
{
	auto buttons = opts->GetButtons();
	int i = 0;
	while (i < buttons.size())
	{
		if (buttons[i] == btn)
			break;
		++i;
	}
	if (i == buttons.size())
		return;

	switch (opt)
	{
	case SelectOption::Options::Element:
		selectedSet.element = (Skill::Element)i;
		break;
	case SelectOption::Options::AttackType:
		selectedSet.attackType = (Skill::AttackType)i;
		break;
	case SelectOption::Options::AttackShape:
		selectedSet.attackShape = (Projectile::AttackShape)i;
		break;
	case SelectOption::Options::MoveType:
		selectedSet.moveType = (Projectile::MoveType)i;
		break;
	case SelectOption::Options::PlayerAction:
		selectedSet.playerAction = (Player::SkillAction)i;
		break;
	case SelectOption::Options::DmgType:
		selectedSet.dmgType = (Projectile::DamageType)i;
		break;
	default:
		break;
	}
	opts->SetActive(false);
	options.second->GetButtons()[(int)opt]->GetText()->SetString(btn->GetText()->GetString());
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
	for (auto button : options.second->GetButtons())
	{
		if (button->GetText()->GetString().empty())
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
				case SelectOption::Options::MoveType:
					doc.SetCell(j, i, (int)selectedSet.moveType);
					break;
				case SelectOption::Options::PlayerAction:
					doc.SetCell(j, i, (int)selectedSet.playerAction);
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
			case SelectOption::Options::MoveType:
				doc.SetCell(j, row, (int)selectedSet.moveType);
				break;
			case SelectOption::Options::PlayerAction:
				doc.SetCell(j, row, (int)selectedSet.playerAction);
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
	Player* player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("player");
	player->GetSkills()[0]->SetSkill(selectedSet);
}

void SelectOption::Load(const string& skillName)
{
	SkillTable* table = DATATABLE_MGR->Get<SkillTable>(DataTable::Types::Skill);
	table->Load();
	selectedSet = table->Get(skillName);
	for (int i = 0; i < (int)SelectOption::Options::Count; ++i)
	{
		switch ((SelectOption::Options)i)
		{
		case SelectOption::Options::Element:
			options.second->GetButtons()[i]->GetText()->SetString(optButtons[0]->GetButtons()[(int)selectedSet.element]->GetText()->GetString());
			break;
		case SelectOption::Options::AttackType:
			options.second->GetButtons()[i]->GetText()->SetString(optButtons[1]->GetButtons()[(int)selectedSet.attackType]->GetText()->GetString());
			break;
		case SelectOption::Options::AttackShape:
			options.second->GetButtons()[i]->GetText()->SetString(optButtons[2]->GetButtons()[(int)selectedSet.attackShape]->GetText()->GetString());
			break;
		case SelectOption::Options::MoveType:
			options.second->GetButtons()[i]->GetText()->SetString(optButtons[3]->GetButtons()[(int)selectedSet.moveType]->GetText()->GetString());
			break;
		case SelectOption::Options::PlayerAction:
			options.second->GetButtons()[i]->GetText()->SetString(optButtons[4]->GetButtons()[(int)selectedSet.playerAction]->GetText()->GetString());
			break;
		case SelectOption::Options::DmgType:
			options.second->GetButtons()[i]->GetText()->SetString(optButtons[5]->GetButtons()[(int)selectedSet.dmgType]->GetText()->GetString());
			break;
		case SelectOption::Options::SkillName:
			textBoxs[0]->SetString(selectedSet.skillName);
			options.second->GetButtons()[i]->GetText()->SetString(selectedSet.skillName);
			break;
		case SelectOption::Options::AttackCntLim:
			textBoxs[1]->SetString(to_string(selectedSet.attackCntLim));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.attackCntLim));
			break;
		case SelectOption::Options::AttackInterval:
			textBoxs[2]->SetString(to_string(selectedSet.attackInterval));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.attackInterval));
			break;
		case SelectOption::Options::Distance:
			textBoxs[3]->SetString(to_string(selectedSet.distance));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.distance));
			break;
		case SelectOption::Options::Amplitude:
			textBoxs[4]->SetString(to_string(selectedSet.amplitude));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.amplitude));
			break;
		case SelectOption::Options::Frequency:
			textBoxs[5]->SetString(to_string(selectedSet.frequency));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.frequency));
			break;
		case SelectOption::Options::SkillDelay:
			textBoxs[6]->SetString(to_string(selectedSet.skillDelay));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.skillDelay));
			break;
		case SelectOption::Options::SkillCoolDown:
			textBoxs[7]->SetString(to_string(selectedSet.skillCoolDown));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.skillCoolDown));
			break;
		case SelectOption::Options::DmgRatio:
			textBoxs[8]->SetString(to_string(selectedSet.dmgRatio));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.dmgRatio));
			break;
		case SelectOption::Options::DmgDelay:
			textBoxs[9]->SetString(to_string(selectedSet.dmgDelay));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.dmgDelay));
			break;
		case SelectOption::Options::Duration:
			textBoxs[10]->SetString(to_string(selectedSet.duration));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.duration));
			break;
		case SelectOption::Options::Speed:
			textBoxs[11]->SetString(to_string(selectedSet.speed));
			options.second->GetButtons()[i]->GetText()->SetString(to_string(selectedSet.speed));
			break;
		case SelectOption::Options::AnimClipName1:
			textBoxs[12]->SetString(selectedSet.animClipName[0]);
			options.second->GetButtons()[i]->GetText()->SetString(selectedSet.animClipName[0]);
			break;
		case SelectOption::Options::AnimClipName2:
			textBoxs[13]->SetString(selectedSet.animClipName[1]);
			options.second->GetButtons()[i]->GetText()->SetString(selectedSet.animClipName[1]);
			break;
		default:
			break;
		}
	}
	for (auto button : options.second->GetButtons())
	{
		button->Reposition();
	}
}
