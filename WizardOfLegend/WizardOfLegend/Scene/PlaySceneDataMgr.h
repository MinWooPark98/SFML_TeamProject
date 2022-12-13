#pragma once
#include "../3rd/SingleTon.h"
#include <vector>
#include <string>

using namespace std;

class PlaySceneDataMgr : public Singleton<PlaySceneDataMgr>
{
public:
	struct Data
	{
		int hoodId = 200;
		vector<int> relicIds;
		vector<string> skillSetNames;
		int currHp = 525;
		int gold = 0;
	};
protected:
	Data data;

public:
	PlaySceneDataMgr();
	virtual ~PlaySceneDataMgr();

	void Save();
	void Load();

	void SaveHoodId(int id) { data.hoodId = id; }
	int GetHoodId() { return data.hoodId; }
	void SaveRelicIds(const vector<int>& ids) { data.relicIds = ids; }
	const vector<int>& GetRelicIds() { return data.relicIds; }
	void SaveSkillSets(const vector<string>& skillSetNames) { data.skillSetNames = skillSetNames; }
	const vector<string>& GetSkillSetNames() { return data.skillSetNames; }
};
#define PLAYSCENE_DATAMGR (PlaySceneDataMgr::GetInstance())
