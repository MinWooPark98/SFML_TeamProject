#include "ItemTable.h"
#include "../3rd/rapidcsv.h"

ItemTable::ItemTable()
	:DataTable(DataTable::Types::Item)
{
	fileName = "tables/ItemTable.csv";
}

ItemTable::~ItemTable()
{
}

const Item::Info& ItemTable::Get(int id)
{
	for (auto& outermost : table)
	{
		auto find = outermost.second.find(id);
		if (find != outermost.second.end())
			return find->second;
	}
	throw invalid_argument("wrong value");
}

const map<int, Item::Info> ItemTable::GetInfoList(Locked locked)
{
	auto find = table.find(locked);
	if (find == table.end())
		throw invalid_argument("wrong value");
	return find->second;
}

void ItemTable::Release()
{
	table.clear();
}

bool ItemTable::Load()
{
	Release();

	map<int, Locked> lockedTable;
	string lockedFileName = "tables/ItemLocked.csv";
	rapidcsv::Document docLocked(lockedFileName, rapidcsv::LabelParams(0, -1));
	{
		auto columnCount = docLocked.GetColumnCount();
		auto rowCount = docLocked.GetRowCount();
		vector<int> key = docLocked.GetColumn<int>(0);
		vector<int> locked = docLocked.GetColumn<int>(1);
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
	vector<int> type = doc.GetColumn<int>(1);
	vector<string> name = doc.GetColumn<string>(2);
	vector<float> speed = doc.GetColumn<float>(3);
	vector<float> atkDmg = doc.GetColumn<float>(4);
	vector<float> dmg = doc.GetColumn<float>(5);
	vector<float> maxHp = doc.GetColumn<float>(6);
	vector<float> evasionRate = doc.GetColumn<float>(7);
	vector<float> criticalRate = doc.GetColumn<float>(8);
	vector<float> criticalRatio = doc.GetColumn<float>(9);
	vector<int> condition = doc.GetColumn<int>(10);
	vector<float> conditionValue = doc.GetColumn<float>(11);
	vector<float> duration= doc.GetColumn<float>(12);
	vector<int> price = doc.GetColumn<int>(13);
	vector<string> iconDir = doc.GetColumn<string>(14);
	vector<string> intro = doc.GetColumn<string>(15);
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
			auto row = docLocked.GetRowCount();
			docLocked.SetCell(0, row, id[j]);
			docLocked.SetCell(1, row, (int)Locked::Locked);
			docLocked.Save(lockedFileName);
		}
		else
			locked = lockedTable[id[j]];
		table[locked].insert({id[j], {id[j], (Item::Types)type[j], name[j], {speed[j], atkDmg[j], dmg[j], maxHp[j], evasionRate[j], criticalRate[j], criticalRatio[j]}, (Item::Condition)condition[j], conditionValue[j], duration[j], price[j], iconDir[j], intro[j]}});
	}
	return true;
}
