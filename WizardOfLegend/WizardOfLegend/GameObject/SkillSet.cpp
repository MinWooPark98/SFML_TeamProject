#include "SkillSet.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/SkillSetTable.h"

SkillSet::SkillSet()
	:subject(nullptr), subType(Skill::SubjectType::None)
{
	for (int i = 0; i < 3; ++i)
	{
		unusingSkills.push_back(new Skill());
	}
}

SkillSet::~SkillSet()
{
}

void SkillSet::Restart()
{
	currSkillIt = usingSkills.rend();
}

void SkillSet::Set(const string& setName)
{
	for (auto skill : usingSkills)
	{
		unusingSkills.push_back(skill);
	}
	usingSkills.clear();

	SkillSetTable* table = DATATABLE_MGR->Get<SkillSetTable>(DataTable::Types::SkillSet);
	auto& skillNames = table->Get(setName);
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
		usingSkills.push_back(skill);
	}
	Restart();
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
