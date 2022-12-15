#pragma once
#include "Item.h"

class Relic : public Item
{
public:
	enum class Condition
	{
		None = -1,
		HpLower,
		HpUpper,
		NumOfItems,
		OnHit,
		WhileDashing,
	};
	struct RelicInfo
	{
		int id;
		string name;
		Values values;
		Condition condition;
		float conditionValue;
		float duration;
		int goldPrice;
		int platinumPrice;
		string iconDir;
		string intro;
	};
protected:
	RelicInfo relicInfo;

	bool applyValues;
	float timer;

public:
	Relic();
	virtual ~Relic() {}

	bool GetApplyValues() { return applyValues; }

	const Values& GetValues() { return relicInfo.values; }
	void SetRelicInfo(const RelicInfo& info);
	const RelicInfo& GetRelicInfo() { return relicInfo; }

	virtual void Update(float dt) override;

	void CheckHpLower();
	void CheckHpUpper();
	void CheckNumOfItems();
	void CheckOnHit(float dt);
	void CheckWhileDashing();
};

