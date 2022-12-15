#pragma once
#include "DataTable.h"
#include "../3rd/rapidcsv.h"

class SavedDataTable : public DataTable
{
public:
	struct Data
	{
		int hoodId;
		int relicId;
		vector<string> skillIds;
	};
protected:
	int tutorialCleared;
	Data data;
	rapidcsv::Document savedData;

public:
	SavedDataTable();
	virtual ~SavedDataTable();

	Data& Get() { return data; }
	int TutorialCleared() { return tutorialCleared; }

	virtual void Release() override;
	virtual bool Load() override;
	
	void ChangeHood(int hoodId);
	void ChangeRelic(int relicID);
	void ChangeSkills(const vector<string>& skillIds);
	void ChangeTutorialCleared();

	string fileName;
};

