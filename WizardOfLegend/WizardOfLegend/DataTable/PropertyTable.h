#pragma once
#include "DataTable.h"
#include <map>

class PropertyTable : public DataTable
{
protected:
	struct Property
	{
		float goldProbability;
		int goldDropNumber;
		int minGold;
		int maxGold;
		float platinumProbability;
		int platinumDropNumber;
	};
	map<string, Property> table;

public:
	PropertyTable();
	virtual ~PropertyTable();

	const Property& Get(const string& objType);

	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};