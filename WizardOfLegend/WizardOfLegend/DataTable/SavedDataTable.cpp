#include "SavedDataTable.h"

SavedDataTable::SavedDataTable()
	:DataTable(Types::SavedData)
{
	fileName = "tables/SavedData.csv";
}

SavedDataTable::~SavedDataTable()
{
}

void SavedDataTable::Release()
{
	data.hoodId = 200;
	data.relicId = 0;
	data.skillIds.clear();
}

bool SavedDataTable::Load()
{
	Release();
	savedData.Load(fileName, rapidcsv::LabelParams(0, -1));
	data.hoodId = savedData.GetCell<int>(0, 0);
	try
	{
		data.relicId = savedData.GetCell<int>(1, 0);
	}
	catch (exception& expn)
	{
		data.relicId = -1;
	}
	for (int i = 2; i < 6; ++i)
	{
		data.skillIds.push_back(savedData.GetCell<string>(i, 0));
	}
	return true;
}

void SavedDataTable::ChangeHood(int hoodId)
{
	savedData.SetCell(0, 0, hoodId);
	savedData.Save(fileName);
}

void SavedDataTable::ChangeRelic(int relicID)
{
	savedData.SetCell(1, 0, relicID);
	savedData.Save(fileName);
}

void SavedDataTable::ChangeSkills(const vector<string>& skillIds)
{
	for (int i = 0; i < skillIds.size(); ++i)
	{
		savedData.SetCell(i + 2, 0, skillIds[i]);
	}
	savedData.Save(fileName);
}
