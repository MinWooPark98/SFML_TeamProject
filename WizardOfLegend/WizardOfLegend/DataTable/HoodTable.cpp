#include "HoodTable.h"

HoodTable::HoodTable()
	:DataTable(DataTable::Types::Hood)
{
	fileName = "tables/HoodTable.csv";
}

HoodTable::~HoodTable()
{
}

const Hood::HoodInfo& HoodTable::Get(int id)
{
	for (auto& outermost : table)
	{
		auto find = outermost.second.find(id);
		if (find != outermost.second.end())
			return find->second;
	}
	throw invalid_argument("wrong value");
}

const map<int, Hood::HoodInfo>& HoodTable::GetInfoList(Locked locked)
{
	auto find = table.find(locked);
	if (find == table.end())
		throw invalid_argument("wrong value");
	return find->second;
}

void HoodTable::Unlock(int id)
{
	vector<int> key = hoodLocked.GetColumn<int>(0);
	for (int j = 0; j < key.size(); ++j)
	{
		if (key[j] == id)
		{
			hoodLocked.SetCell(1, j, (int)Locked::Unlocked);
			hoodLocked.Save("tables/HoodLocked.csv");
			Load();
			return;
		}
	}
}

void HoodTable::Release()
{
	table.clear();
}

bool HoodTable::Load()
{
	Release();

	map<int, Locked> lockedTable;
	string lockedFileName = "tables/HoodLocked.csv";
	hoodLocked.Load(lockedFileName, rapidcsv::LabelParams(0, -1));
	{
		auto columnCount = hoodLocked.GetColumnCount();
		auto rowCount = hoodLocked.GetRowCount();
		vector<int> key = hoodLocked.GetColumn<int>(0);
		vector<int> locked = hoodLocked.GetColumn<int>(1);
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
	vector<int> paletteIdx = doc.GetColumn<int>(2);
	vector<float> speed = doc.GetColumn<float>(3);
	vector<float> atkDmg = doc.GetColumn<float>(4);
	vector<float> dmg = doc.GetColumn<float>(5);
	vector<float> maxHp = doc.GetColumn<float>(6);
	vector<float> evasionRate = doc.GetColumn<float>(7);
	vector<float> criticalRate = doc.GetColumn<float>(8);
	vector<float> criticalRatio = doc.GetColumn<float>(9);
	vector<int> goldPrice = doc.GetColumn<int>(10);
	vector<int> platinumPrice = doc.GetColumn<int>(11);
	vector<string> intro = doc.GetColumn<string>(12);
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
			auto row = hoodLocked.GetRowCount();
			hoodLocked.SetCell(0, row, id[j]);
			hoodLocked.SetCell(1, row, (int)Locked::Locked);
			hoodLocked.Save(lockedFileName);
		}
		else
			locked = lockedTable[id[j]];
		table[locked].insert({ id[j], {id[j], name[j], paletteIdx[j], {speed[j], atkDmg[j], dmg[j], maxHp[j], evasionRate[j], criticalRate[j], criticalRatio[j]}, goldPrice[j], platinumPrice[j], intro[j]}});
	}
	return true;
}
