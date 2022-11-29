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

	bool isOnCoolDown;
	bool newCoolDownEntered;
	float newCoolDown;
	float timer;

public:
	SkillSet();
	~SkillSet();

	void Restart();
	void Set(const string& setName);
	void SetOnlyOneSkill(const Skill::Set& set);
	void SetSubject(Object* subject, Skill::SubjectType subType);
	bool Do();
	void Update(float dt);
	void Draw(RenderWindow& window);

	Skill* GetCurrSkill() { return *currSkillIt; }
};

