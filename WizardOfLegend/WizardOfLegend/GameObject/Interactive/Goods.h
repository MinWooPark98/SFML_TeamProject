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

	TextObj* priceText;
	int price;

	Payment payment;
	Types type;

	SpriteObj* infoPanel;
	TextObj* goodsNameText;
	string goodsName;
	TextObj* goodsInfoText;
	string goodsInfo;

public:
	Goods(Payment payment, Types type);
	virtual ~Goods();

	virtual void Init() override;
	virtual void Draw(RenderWindow& window) override;

	virtual void Approach() override;
	virtual void ApproachEnd() override;

	void ForSale(bool sale);
	virtual void Sale();
	virtual void Saled() {}

	void SetGoodsName(const string& name);
	void SetGoodsInfo(const string& info);
	void SetPrice(int price);

	virtual void SetPos(const Vector2f& pos) override;
};
