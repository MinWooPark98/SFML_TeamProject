#include "DataTableMgr.h"
#include "LanguageTable.h"
#include "SkillTable.h"

DataTableMgr::DataTableMgr(const DataTableMgr& ref)
{
}

DataTableMgr& DataTableMgr::operator=(const DataTableMgr& ref)
{
    return *this;
}

DataTableMgr::DataTableMgr()
{
    Init();
}

DataTableMgr::~DataTableMgr()
{
    Release();
}

void DataTableMgr::Init()
{
    DataTable* table = new LanguageTable();
    table->Load();
    tables.insert({ DataTable::Types::Language, table });

    table = new SkillTable();
    table->Load();
    tables.insert({ DataTable::Types::Skill, table });
}

void DataTableMgr::Release()
{
    for (auto& pair : tables)
    {
        delete pair.second;
    }
    tables.clear();
}