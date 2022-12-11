#include "RelicTable.h"

RelicTable::RelicTable()
	:DataTable(DataTable::Types::Relic)
{
	fileName = "tables/RelicTable.csv";
}

RelicTable::~RelicTable()
{
}

const Relic::RelicInfo& RelicTable::Get(int id)
{
	for (auto& outermost : table)
	{
		auto find = outermost.second.find(id);
		if (find != outermost.second.end())
			return find->second;
	}
	throw invalid_argument("wrong value");
}

const map<int, Relic::RelicInfo>& RelicTable::GetInfoList(Locked locked)
{
	auto find = table.find(locked);
	if (find == table.end())
		throw invalid_argument("wrong value");
	return find->second;
}

void RelicTable::Unlock(int id)
{
	vector<int> key = relicLocked.GetColumn<int>(0);
	for (int j = 0; j < key.size(); ++j)
	{
		if (key[j] == id)
		{
			relicLocked.SetCell(1, j, (int)Locked::Unlocked);
			relicLocked.Save("tables/ItemLocked.csv");
			return;
		}
	}
}

void RelicTable::Release()
{
	table.clear();
}

bool RelicTable::Load()
{
	Release();

	map<int, Locked> lockedTable;
	string lockedFileName = "tables/ItemLocked.csv";
	relicLocked.Load(lockedFileName, rapidcsv::LabelParams(0, -1));
	{
		auto columnCount = relicLocked.GetColumnCount();
		auto rowCount = relicLocked.GetRowCount();
		vector<int> key = relicLocked.GetColumn<int>(0);
		vector<int> locked = relicLocked.GetColumn<int>(1);
		for (int j = 0; j < rowCount; ++j)
		{
			if (lockedTable.find(key[j]) != lockedTable.end())
			{
				cout << "duplicate values exist" << endl;
				return false;
			}
			lockedTable[key[j]] = (Locked)locked[j];
		}
	}

	rapidcsv::Document doc(fileName, rapidcsv::LabelParams(0, -1));
	auto columnCount = doc.GetColumnCount();
	auto rowCount = doc.GetRowCount();
	vector<int> id = doc.GetColumn<int>(0);
	vector<string> name = doc.GetColumn<string>(1);
	vector<float> speed = doc.GetColumn<float>(2);
	vector<float> atkDmg = doc.GetColumn<float>(3);
	vector<float> dmg = doc.GetColumn<float>(4);
	vector<float> maxHp = doc.GetColumn<float>(5);
	vector<float> evasionRate = doc.GetColumn<float>(6);
	vector<float> criticalRate = doc.GetColumn<float>(7);
	vector<float> criticalRatio = doc.GetColumn<float>(8);
	vector<int> condition = doc.GetColumn<int>(9);
	vector<float> conditionValue = doc.GetColumn<float>(10);
	vector<float> duration= doc.GetColumn<float>(11);
	vector<int> price = doc.GetColumn<int>(12);
	vector<string> iconDir = doc.GetColumn<string>(13);
	vector<string> intro = doc.GetColumn<string>(14);
	for (int j = 0; j < rowCount; ++j)
	{
		for (auto& outermost : table)
		{
			if (outermost.second.find(id[j]) != outermost.second.end())
			{
				cout << "duplicate values exist" << endl;
				return false;
			}
		}
		Locked locked = Locked::Locked;
		if (lockedTable.find(id[j]) == lockedTable.end())
		{
			auto row = relicLocked.GetRowCount();
			relicLocked.SetCell(0, row, id[j]);
			relicLocked.SetCell(1, row, (int)Locked::Locked);
			relicLocked.Save(lockedFileName);
		}
		else
			locked = lockedTable[id[j]];
		table[locked].insert({id[j], {id[j], name[j], {speed[j], atkDmg[j], dmg[j], maxHp[j], evasionRate[j], criticalRate[j], criticalRatio[j]}, (Relic::Condition)condition[j], conditionValue[j], duration[j], price[j], iconDir[j], intro[j]}});
	}
	return true;
}
