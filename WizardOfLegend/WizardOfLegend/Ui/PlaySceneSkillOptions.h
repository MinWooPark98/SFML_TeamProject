#pragma once
#include "../GameObject/Object.h"

class DataTableList;
class Button2;

class PlaySceneSkillOptions : public Object
{
protected:
	list<Object*> selectSkillSet;
	vector<DataTableList*> skillSetLists;
	vector<Button2*> currSkillSets;
	int vecIdx;

public:
	PlaySceneSkillOptions();
	virtual ~PlaySceneSkillOptions();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void SelectSkillSet(int idx);
	void SetPlayerSkillSet(const string& skillSetName);
};
