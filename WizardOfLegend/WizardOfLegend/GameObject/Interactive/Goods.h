#pragma once
#include "Interactive.h"

class TextObj;

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

	TextObj* price;

	Payment payment;
	Types type;

	SpriteObj infoPanel;

public:
	Goods(Payment payment, Types type);
	virtual ~Goods();

	virtual void Init() override;
	virtual void Draw(RenderWindow& window) override;

	virtual void Approach() override;
	virtual void ApproachEnd() override;

	void ForSale(bool sale);
	virtual void Saled() {}
};
