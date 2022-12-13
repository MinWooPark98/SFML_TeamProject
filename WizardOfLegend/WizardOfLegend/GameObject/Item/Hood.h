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
		int goldPrice;
		int platinumPrice;
		string intro;
	};
protected:
	HoodInfo hoodInfo;

public:
	Hood();
	virtual ~Hood();

	const Values& GetValues() { return hoodInfo.values; }
	void SetHoodInfo(const HoodInfo& info) { hoodInfo = info; }
	const HoodInfo& GetHoodInfo() { return hoodInfo; }

	void SetPaletteIdx(int idx) { hoodInfo.paletteIdx = idx; }
	int GetPaletteIdx() { return hoodInfo.paletteIdx; }
};

