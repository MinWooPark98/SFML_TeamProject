#pragma once
#include "Skill.h"

class Skill;

using namespace std;
using namespace sf;

class SkillSet 
{
private:
	list<Skill*> usingSkills;
	list<Skill*> unusingSkills;
	list<Skill*>::reverse_iterator currSkillIt;

	Object* subject;
	Skill::SubjectType subType;

	bool isSingleSkill;
	bool isOnCoolDown;
	bool newCoolDownEntered;
	bool newCoolDownApplied;
	float newCoolDown;
	float timer;

	string iconDir;
	string skillSetName;

public:
	SkillSet();
	~SkillSet();

	void ResetSkills();
	void Restart();
	void Set(const string& setName);
	void SetOnlyOneSkill(const Skill::Set& set);
	void SetSubject(Object* subject, Skill::SubjectType subType);
	bool Do();
	void Update(float dt);
	void Draw(RenderWindow& window);

	bool IsSingleSkill() const { return isSingleSkill; }
	bool IsOnCoolDown() const { return isOnCoolDown; }
	bool NewCoolDownApplied() const { return newCoolDownApplied; }
	float GetCoolDown() const { return newCoolDown; }
	float GetTimer() const { return timer; }
	list<Skill*> GetUsingSkills() const { return usingSkills; }

	Skill* GetCurrSkill();
	string GetIconDir() { return iconDir; }
	string GetSkillSetName() const { return skillSetName; }
};

