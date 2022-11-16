#include "SelectOption.h"
#include "../GameObject/TextObj.h"
#include "../GameObject/Button2.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/Framework.h"
#include "UiMgr.h"

SelectOption::SelectOption(UiMgr* mgr)
	:uiMgr(mgr)
{
}

SelectOption::~SelectOption()
{
}

void SelectOption::Init()
{
	auto windowSize = (Vector2f)FRAMEWORK->GetWindowSize();
	vector<string> opts = { "SkillName", "Element", "AttackType", "AttackCntLim", "AttackInterval", "Distance", "AttackShape", "Amplitude", "Frequency", "MoveType", "PlayerAction", "SkillDelay", "SkillCoolDown", "DmgRatio", "DmgType", "DmgDelay", "Duration", "Speed", "AnimClipName_1", "AnimClipName_2" };
	int optSize = opts.size();
	float buttonHeight = windowSize.y * 0.7f / optSize;
	for (int i = 0; i < optSize; ++i)
	{
		pair<Button2*, Button2*> option = { new Button2(), new Button2() };
		option.first->Init();
		option.first->UseText();
		option.first->SetText("fonts/NotoSansKR-Bold.otf", 20, Color::White, opts[i]);
		option.first->SetHitBox({ 0.f, 0.f, 200.f, buttonHeight }, Color(150, 150, 150, 255));
		option.first->GetHitBox().setOutlineThickness(2.f);
		option.first->SetDevMode(true);
		option.first->SetPos({ windowSize.x * 0.7f, buttonHeight * i });
		option.first->SetOrigin(Origins::TL);
		option.first->SetUI(true);

		option.second->Init();
		option.second->UseText();
		option.second->SetText("fonts/NotoSansKR-Bold.otf", 20, Color::White);
		option.second->SetHitBox({ 0.f, 0.f, windowSize.x - (option.first->GetHitBounds().left + option.first->GetHitBounds().width), buttonHeight}, Color(150, 150, 150, 255));
		option.second->GetHitBox().setOutlineThickness(2.f);
		option.second->SetDevMode(true);
		option.second->SetPos({ windowSize.x * 0.7f + 200.f, buttonHeight * i });
		option.second->SetOrigin(Origins::TL);
		option.second->SetUI(true);
		option.second->MousePointerOn = bind(&Button2::DefaultMouseOn, option.second);
		option.second->MousePointerOff = bind(&Button2::DefaultMouseOff, option.second);
		//option.second->MouseOn = bind()
		options.push_back(option);
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
}

void SelectOption::Draw(RenderWindow& window)
{
	Object::Draw(window);
	for (auto& option : options)
	{
		option.first->Draw(window);
		option.second->Draw(window);
	}
}
