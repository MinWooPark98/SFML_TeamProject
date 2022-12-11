#pragma once
#include "Interactive.h"

class Goods : public Interactive
{
public:
	enum class Payment
	{
		Gold,
		Platinum,
	};
	enum class Types
	{
		Hood,
		Relic,
		Skill,
	};

protected:
	bool forSale;

	Payment payment;
	Types type;

	SpriteObj infoPanel;

public:
	Goods(Payment payment, Types type);
	virtual ~Goods();

	virtual void Init() override;

	void ForSale(bool sale);
	virtual void Saled() {}
};
