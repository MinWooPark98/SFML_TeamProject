#include "DataTableMgr.h"
#include "StatTable.h"
#include "SkillTable.h"
#include "SkillSetTable.h"
#include "SkillSetIntroTable.h"
#include "FinalBossSkillTable.h"
#include "MapNameTable.h"
#include "PlatinumTable.h"
#include "PropertyTable.h"
#include "RelicTable.h"
#include "HoodTable.h"
#include "NpcTalkTable.h"
#include "SavedDataTable.h"

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
    DataTable* table = new StatTable();
    table->Load();
    tables.insert({ DataTable::Types::Stat, table });

    table = new SkillTable();
    table->Load();
    tables.insert({ DataTable::Types::Skill, table });

    table = new SkillSetTable();
    table->Load();
    tables.insert({ DataTable::Types::SkillSet, table });

    table = new SkillSetIntroTable();
    table->Load();
    tables.insert({ DataTable::Types::SkillSetIntro, table });

    table = new FinalBossSkillTable();
    table->Load();
    tables.insert({ DataTable::Types::FinalBossSkill, table });

    table = new MapNameTable();
    table->Load();
    tables.insert({ DataTable::Types::MapName, table });

    table = new PlatinumTable();
    table->Load();
    tables.insert({DataTable::Types::Platinum, table});

    table = new PropertyTable();
    table->Load();
    tables.insert({DataTable::Types::MonsterProperty, table});

    table = new RelicTable();
    table->Load();
    tables.insert({ DataTable::Types::Relic, table });

    table = new HoodTable();
    table->Load();
    tables.insert({ DataTable::Types::Hood, table });

    table = new NpcTalkTable();
    table->Load();
    tables.insert({ DataTable::Types::NpcTalk, table });

    table = new SavedDataTable();
    table->Load();
    tables.insert({ DataTable::Types::SavedData, table });
}

void DataTableMgr::Release()
{
    for (auto& pair : tables)
    {
        delete pair.second;
    }
    tables.clear();
}