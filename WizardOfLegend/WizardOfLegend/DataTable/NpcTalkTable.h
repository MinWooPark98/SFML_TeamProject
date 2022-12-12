#pragma once
#include "DataTable.h"
#include <map>

class NpcTalkTable : public DataTable
{
protected:
	struct NpcType
	{
		string npcName;
		vector<string> talks;
	};
	map<string, NpcType> table;

public:
	NpcTalkTable();
	virtual ~NpcTalkTable();

	const NpcType& Get(const string& objType);
	const map<string, NpcType>& GetTable() { return table; };
	virtual void Release() override;
	virtual bool Load() override;

	string fileName;
};

