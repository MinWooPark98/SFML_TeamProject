#pragma once
#include "Goods.h"
#include "../Item/Hood.h"

class GoodsHood : public Goods
{
protected:
	SpriteObj mannequin;
	SpriteObj hoodDisplay;

	Hood::HoodInfo info;

public:
	GoodsHood(Payment payment);
	virtual ~GoodsHood();

	virtual void Init();
	virtual void Draw(RenderWindow& window);

	void SetInfo(const Hood::HoodInfo& info);
	virtual void Saled() override;
};

