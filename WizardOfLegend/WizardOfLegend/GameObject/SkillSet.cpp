#include "SkillSet.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/SkillSetTable.h"
#include "../Scene/SceneMgr.h"
#include "../Ui/PlayUiMgr.h"

SkillSet::SkillSet()
	:subject(nullptr), subType(Skill::SubjectType::None), element(Skill::Element::Fire), isSingleSkill(false), isOnCoolDown(false), newCoolDownEntered(false), newCoolDownApplied(false), newCoolDown(0.f), timer(0.f)
{
	for (int i = 0; i < 3; ++i)
	{
		unusingSkills.push_back(new Skill());
	}
}

SkillSet::~SkillSet()
{
}

void SkillSet::ResetSkills()
{
	isOnCoolDown = false;
	newCoolDownEntered = false;
	newCoolDownApplied = false;
	newCoolDown = 0.f;
	timer = 0.f;
	for (auto skill : usingSkills)
	{
		skill->Reset();
		unusingSkills.push_back(skill);
	}
	usingSkills.clear();
	skillSetName.clear();
}

void SkillSet::Restart()
{
	if (isOnCoolDown)
		return;
	isOnCoolDown = true;
	currSkillIt = usingSkills.rend();
	if (!isSingleSkill)
	{
		for (auto skill : usingSkills)
		{
			skill->Reprepare();
		}
	}
	Do();
}

void SkillSet::Set(const string& setName)
{
	ResetSkills();
	skillSetName = setName;
	SkillSetTable* table = DATATABLE_MGR->Get<SkillSetTable>(DataTable::Types::SkillSet);
	auto& skillSetInfo = table->Get(setName);
	element = table->GetElement(setName);
	newCoolDown = skillSetInfo.newCoolDown;
	if (!Utils::EqualFloat(newCoolDown, -1.f))
	{
		newCoolDownEntered = true;
		newCoolDownApplied = true;
	}
	iconDir = skillSetInfo.iconDir;
	auto& skillNames = skillSetInfo.skillNames;
	if (skillNames.size() == 1)
		isSingleSkill = true;
	else
		isSingleSkill = false;
	for (auto& skillName : skillNames)
	{
		if (unusingSkills.empty())
		{
			for (int i = 0; i < usingSkills.size(); ++i)
			{
				Skill* newSkill = new Skill();
				newSkill->SetSubject(subject, subType);
				unusingSkills.push_back(newSkill);
			}
		}
		auto skill = unusingSkills.front();
		unusingSkills.pop_front();
		skill->SetSkill(skillName);
		if (!newCoolDownEntered)
		{
			if (isSingleSkill && skill->GetSetting()->attackType == Skill::AttackType::SaveAttacks)
			{
				newCoolDown = 0.f;
				newCoolDownApplied = false;
			}
			else
			{
				float coolDown = skill->GetSetting()->skillCoolDown;
				if (newCoolDown < coolDown)
				{
					newCoolDown = coolDown;
					newCoolDownApplied = true;
				}
			}
		}
		usingSkills.push_back(skill);
	}
}

void SkillSet::SetOnlyOneSkill(const Skill::Set& set)
{
	ResetSkills();
	skillSetName = set.skillName;
	isSingleSkill = true;
	element = set.element;
	if (set.attackType == Skill::AttackType::SaveAttacks)
	{
		newCoolDown = 0.f;
		newCoolDownApplied = false;
	}
	else
	{
		newCoolDown = set.skillCoolDown;
		newCoolDownApplied = true;
	}
	auto skill = unusingSkills.front();
	unusingSkills.pop_front();
	skill->SetSkill(set);
	usingSkills.push_back(skill);
}

void SkillSet::SetSubject(Object* subject, Skill::SubjectType subType)
{
	this->subject = subject;
	this->subType = subType;
	for (auto skill : unusingSkills)
	{
		skill->SetSubject(subject, subType);
	}
	for (auto skill : usingSkills)
	{
		skill->SetSubject(subject, subType);
	}
}

bool SkillSet::Do()
{
	if (currSkillIt == usingSkills.rbegin())
		return false;
	(*(--currSkillIt))->Do();
	return true;
}

void SkillSet::Update(float dt)
{
	if (isOnCoolDown)
	{
		timer += dt;
		if (timer >= newCoolDown)
		{
			isOnCoolDown = false;
			timer = 0.f;
		}
	}
	for (auto skill : usingSkills)
	{
		skill->Update(dt);
	}
}

void SkillSet::Draw(RenderWindow& window)
{
	for (auto skill : usingSkills)
	{
		skill->Draw(window);
	}
}

Skill* SkillSet::GetCurrSkill()
{
	if(currSkillIt == usingSkills.rend())
		return nullptr;
	return *currSkillIt;
}
