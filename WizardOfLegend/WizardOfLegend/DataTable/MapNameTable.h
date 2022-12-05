#pragma once
#include "DataTable.h"
#include <list>

class MapNameTable : public DataTable
{
public:
	MapNameTable();
	virtual ~MapNameTable();
	
	virtual void Release() override;
	virtual bool Load() override;
};

