#include "DataTableMgr.h"
#include "SkillTable.h"
#include "SkillSetTable.h"
#include "FinalBossSkillTable.h"

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
    DataTable* table = new SkillTable();
    table->Load();
    tables.insert({ DataTable::Types::Skill, table });

    table = new SkillSetTable();
    table->Load();
    tables.insert({ DataTable::Types::SkillSet, table });

    table = new FinalBossSkillTable();
    table->Load();
    tables.insert({ DataTable::Types::FinalBossSkill, table });
}

void DataTableMgr::Release()
{
    for (auto& pair : tables)
    {
        delete pair.second;
    }
    tables.clear();
}