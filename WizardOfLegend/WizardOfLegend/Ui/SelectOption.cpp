#include "SelectOption.h"
#include "../GameObject/TextObj.h"
#include "../GameObject/Button2.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/Framework.h"

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
	float buttonHeight = windowSize.y * 0.7f / (int)Options::Count;
	for (int i = 0; i < (int)Options::Count; ++i)
	{
		pair<Button2*, Button2*> option = { new Button2(), new Button2() };
		option.first->Init();
		option.first->UseText();
		option.first->SetText("fonts/NotoSansKR-Bold.otf", 20, Color::White, opts[i]);
		option.first->SetHitBox({ 0.f, 0.f, 200.f, buttonHeight }, Color(150, 150, 150, 255));
		option.first->GetHitBox().setOutlineThickness(2.f);
		option.first->SetDevMode(true);
		option.first->SetPos({ windowSize.x * 0.7f, (buttonHeight + 4.f) * i });
		option.first->SetOrigin(Origins::TL);
		option.first->SetUI(true);

		option.second->Init();
		option.second->UseText();
		option.second->SetText("fonts/NotoSansKR-Bold.otf", 20, Color::White, "Select");
		option.second->SetHitBox({ 0.f, 0.f, windowSize.x - (option.first->GetHitBounds().left + option.first->GetHitBounds().width), buttonHeight}, Color(150, 150, 150, 255));
		option.second->GetHitBox().setOutlineThickness(2.f);
		option.second->SetDevMode(true);
		option.second->SetPos({ windowSize.x * 0.7f + 200.f, (buttonHeight + 4.f) * i });
		option.second->SetOrigin(Origins::TL);
		option.second->SetUI(true);
		option.second->MousePointerOn = bind(&Button2::DefaultMouseOn, option.second);
		option.second->MousePointerOff = bind(&Button2::DefaultMouseOff, option.second);
		options.push_back(option);

		switch ((Options)i)
		{
		case Options::Element:
		case Options::AttackType:
		case Options::AttackShape:
		case Options::MoveType:
		case Options::PlayerAction:
		case Options::DmgType:
			break;
		default:
			{
				TextBox* newBox = new TextBox();
				newBox->Init();
				newBox->SetHitBox({ 0.f, 0.f, option.second->GetHitBounds().width * 0.95f, buttonHeight * 0.8f }, Color(255, 255, 255, 255));
				newBox->SetPos(option.second->GetPos() + Vector2f(5.f, buttonHeight * 0.1f));
				newBox->SendWritten = bind(&SelectOption::ApplyOption, this, (Options)i, newBox);
				option.second->ClickOn = bind(&TextBox::SetActive, newBox, true);
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
}

void SelectOption::Update(float dt)
{
	Object::Update(dt);
	for (auto& option : options)
	{
		option.first->Update(dt);
		option.second->Update(dt);
	}
	for (auto& box : textBoxs)
	{
		if (box->GetActive())
			box->Update(dt);
	}
}

void SelectOption::Draw(RenderWindow& window)
{
	Object::Draw(window);
	for (auto& option : options)
	{
		option.first->Draw(window);
		option.second->Draw(window);
	}
	for (auto& box : textBoxs)
	{
		if(box->GetActive())
			box->Draw(window);
	}
}

void SelectOption::ApplyOption(Options opt, TextBox* box)
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
		box->SetString(options[(int)opt].second->GetText()->GetString());
		return;
	}
	box->SetString(str);
	options[(int)opt].second->GetText()->SetString(str);
	options[(int)opt].second->Reposition();
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
