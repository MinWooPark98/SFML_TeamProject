#pragma once
#include "DataTable.h"
#include <map>
#include <list>
#include "../GameObject/SkillSet.h"
#include "../3rd/rapidcsv.h"

class SkillSetTable : public DataTable
{
public:
	enum class Locked
	{
		Locked,
		Unlocked,
	};
	struct SetInfo
	{
		float newCoolDown;
		string iconDir;
		list<string> skillNames;
	};
protected:
	map<Locked, map<Skill::Element, map<string, SetInfo>>> table;
	rapidcsv::Document skillSetLocked;

public:
	SkillSetTable();
	virtual ~SkillSetTable();

	const SetInfo& Get(const string& setName);
	const map<Skill::Element, map<string, SetInfo>> GetElementLists(Locked locked);
	const map<string, SetInfo> Get(Locked locked, Skill::Element elem);
	const Skill::Element GetElement(const string& name);
	const map<Locked, map<Skill::Element, map<string, SetInfo>>>& GetTable() { return table; }
	void Unlock(const string& name);

	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};

