#include "SelectOption.h"
#include "../GameObject/TextObj.h"
#include "../GameObject/Button2.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/Framework.h"
#include "../Framework/InputMgr.h"
#include "../Scene/SceneMgr.h"
#include <fstream>

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
	float buttonHeight = windowSize.y * 0.7f / (int)Options::Count;
	options.first = new OptionButtons();
	options.first->SetPos({ windowSize.x * 0.7f, 0.f });
	options.second = new OptionButtons();
	options.second->SetPos({ windowSize.x * 0.7f + 204.f, 0.f });
	for (int i = 0; i < (int)Options::Count; ++i)
	{
		Button2* button1 = new Button2();
		Button2* button2 = new Button2();
		options.first->AddButton(button1, opts[i], { 0.f, 0.f, 200.f, buttonHeight });
		options.second->AddButton(button2, "select", { 0.f, 0.f, windowSize.x * 0.3f - 202.f, buttonHeight });
		button2->MousePointerOn = bind(&Button2::DefaultMouseOn, button2);
		button2->MousePointerOff = bind(&Button2::DefaultMouseOff, button2);

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
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color::Cyan);
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
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color::Cyan);
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
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color::Cyan);
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
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color::Cyan);
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
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color::Cyan);
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
					buttons->AddButton(newButton, btnStr[j], button2->GetHitBounds(), Color::White, Color::Cyan);
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
}

void SelectOption::Release()
{
	Object::Release();
}

void SelectOption::Reset()
{
	Object::Reset();
	if (options.first != nullptr)
		options.first->Reset();
	if (options.second != nullptr)
		options.second->Reset();
	for (auto& buttons : optButtons)
	{
		buttons->Reset();
	}
	for (auto& box : textBoxs)
	{
		box->Reset();
	}
	selectedSet.Reset();
}

void SelectOption::Update(float dt)
{
	Object::Update(dt);

	if (InputMgr::GetKeyDown(Keyboard::F1))
	{
		Player* player = (Player*)SCENE_MGR->GetCurrentScene()->FindGameObj("player");
		player->GetSkills()[0]->SetSkill(selectedSet);
	}

	if (InputMgr::GetKeyDown(Keyboard::F2))
		SaveSetToCSV();

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
		{
			buttons->Draw(window);
		}
	}
	for (auto& box : textBoxs)
	{
		if(box->GetActive())
			box->Draw(window);
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
		if (lastStr == "select")
			box->SetString("");
		else
			box->SetString(lastStr);
		return;
	}
	box->SetString(str);
	if (str.empty())
		options.second->GetButtons()[(int)opt]->GetText()->SetString("select");
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
