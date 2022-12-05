#include "SkillCoolDownUi.h"
#include "../GameObject/SkillSet.h"
#include "../GameObject/TextObj.h"

SkillCoolDownUi::SkillCoolDownUi()
	:text(nullptr), skillSet(nullptr)
{
}

SkillCoolDownUi::~SkillCoolDownUi()
{
}

void SkillCoolDownUi::Init()
{
	shape.setPointCount(7);
	Object::Init();
	text = new TextObj();
	text->Init();
	text->SetFont(*RESOURCE_MGR->GetFont("fonts/MunroSmall.ttf"));
	text->SetOrigin(Origins::MC);
	text->SetSize(40);
	text->SetFillColor(Color::White);
	text->SetOutlineColor(Color::Black);
	text->SetOutlineThickness(1.f);
}

void SkillCoolDownUi::Reset()
{
	Object::Reset();
	for (int i = 0; i < shape.getPointCount(); ++i)
	{
		shape.setPoint(i, position);
	}
	shape.setFillColor(Color(0, 0, 0, 155));
}

void SkillCoolDownUi::Update(float dt)
{
	Object::Update(dt);
	if (skillSet == nullptr || skillSet->GetSkillSetName().empty())
		return;
	float coolRatio = 0.f;
	if (!skillSet->NewCoolDownApplied())
	{
		auto skill = skillSet->GetUsingSkills().back();
		float totalRatio =skill->GetSkillTimer() / (skill->GetSetting()->skillCoolDown / skill->GetSetting()->attackCntLim);
		coolRatio = totalRatio - (int)totalRatio;
		str = to_string(skill->GetSetting()->attackCntLim - skill->GetAttackCnt());
		text->SetString(str);
		text->SetOrigin(Origins::BC);
		if (skill->GetAttackCnt() == 0)
		{
			for (int i = 0; i < shape.getPointCount(); ++i)
			{
				shape.setPoint(i, position);
			}
			return;
		}
	}
	else
	{
		coolRatio = skillSet->GetTimer() / skillSet->GetCoolDown();
		str = to_string(skillSet->GetCoolDown() - skillSet->GetTimer());
		str = str.substr(0, str.find('.') + 2);
		text->SetString(str);
		text->SetOrigin(Origins::BC);
		if (!skillSet->IsOnCoolDown())
		{
			for (int i = 0; i < shape.getPointCount(); ++i)
			{
				shape.setPoint(i, position);
			}
			text->SetActive(false);
			return;
		}
	}
	text->SetActive(true);
	if (coolRatio < 0.125f)
	{
		shape.setPoint(0, position);
		shape.setPoint(1, Vector2f(position.x + 32.f * coolRatio / 0.125, position.y - 32.f));
		shape.setPoint(2, Vector2f(position.x + 32.f, position.y - 32.f));
		shape.setPoint(3, Vector2f(position.x + 32.f, position.y + 32.f));
		shape.setPoint(4, Vector2f(position.x - 32.f, position.y + 32.f));
		shape.setPoint(5, Vector2f(position.x - 32.f, position.y - 32.f));
		shape.setPoint(6, Vector2f(position.x, position.y - 32.f));
	}
	else if (coolRatio <= 0.375f)
	{
		auto point = Vector2f(position.x + 32.f, position.y - 32.f + 64.f * (coolRatio - 0.125f) / 0.25f);
		shape.setPoint(1, point);
		shape.setPoint(2, point);
	}
	else if (coolRatio <= 0.625f)
	{
		auto point = Vector2f(position.x + 32.f - 64.f * (coolRatio - 0.375) / 0.25f, position.y + 32.f);
		shape.setPoint(1, point);
		shape.setPoint(2, point);
		shape.setPoint(3, point);
	}
	else if (coolRatio <= 0.875f)
	{
		auto point = Vector2f(position.x - 32.f, position.y + 32.f - 64.f * (coolRatio - 0.625) / 0.25f);
		shape.setPoint(1, point);
		shape.setPoint(2, point);
		shape.setPoint(3, point);
		shape.setPoint(4, point);
	}
	else
	{
		auto point = Vector2f(position.x - 32.f + 32.f * (coolRatio - 0.875) / 0.125f, position.y - 32.f);
		shape.setPoint(1, point);
		shape.setPoint(2, point);
		shape.setPoint(3, point);
		shape.setPoint(4, point);
		shape.setPoint(5, point);
	}
}

void SkillCoolDownUi::Draw(RenderWindow& window)
{
	Object::Draw(window);
	window.draw(shape);
	if(text != nullptr && text->GetActive())
		text->Draw(window);
}

void SkillCoolDownUi::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	text->SetPos(pos);
}
