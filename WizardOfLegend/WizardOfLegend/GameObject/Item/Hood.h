#pragma once
#include "Item.h"
class Hood : public Item
{
public:
	struct HoodInfo
	{
		int id;
		string name;
		int paletteIdx;
		Values values;
		int price;
		string intro;
	};
protected:
	HoodInfo hoodInfo;

public:
	Hood();
	virtual ~Hood();

	const Values& GetValues() { return hoodInfo.values; }
	void SetHoodInfo(const HoodInfo& info) { hoodInfo = info; }

	void SetPaletteIdx(int idx) { hoodInfo.paletteIdx = idx; }
	int GetPaletteIdx() { return hoodInfo.paletteIdx; }
};

